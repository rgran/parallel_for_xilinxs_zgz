/*
#ifdef FASTFIT
#include "FastFit.h"
#endif
 #ifdef FASTFIT
        p.initiationInterval=1;
        p.frequency = (float) atof(argv[4]);
		if (argc > 5)
			p.thobjective = (float) atof(argv[5]);
#endif
#ifdef FASTFIT
        FastFit * hs = FastFit::getInstance(&p);
#endif
*/

//============================================================================
// Name			: FastFit.h
// Author		: Antonio Vilches
// Version		: 1.0
// Date			: 02 / 01 / 2014
// Copyright	: Department. Computer's Architecture (c)
// Description	: Fastfit scheduler implementation
//============================================================================

//#define NOOCLPROF
#define FINALPHASE
#define MODA  // Calculo del TH medio de las CPUs (avgCpuThroughput) usando la moda estadistica por intervalos mezclada con la media por intervalos
//#define TRACER
//#define DEEP_CPU_REPORT
//#define DEEP_GPU_REPORT
#define NDEBUG
#define OCL_PROFILING  // for profiling kernel execution with OCL events

#include <cstdlib>
#include <iostream>
#include <fstream>

#ifdef DEEP_GPU_EX_REPORT  // only based on kernel execution times (OCL events)
#define OCL_PROFILING
#endif

#ifdef BSCODE
#include "Scheduler_bs.h"
#else
#include "Scheduler.h"
#endif
#include "tbb/pipeline.h"
#include "tbb/tick_count.h"
#include <tbb/enumerable_thread_specific.h>
#include <math.h>

// #define _GNU_SOURCE already defined
#include <sys/syscall.h>
#include <sys/types.h>

#ifdef Win32
#include "PCM_Win/windriver.h"
//#else
//#include "cpucounters.h"
#endif

#ifdef TRACER
#include "pfortrace.h"
PFORTRACER tracer("FastFit.json");
#endif

//#define DEBUG

using namespace std;
using namespace tbb;

using  PrivFloat_t = tbb::enumerable_thread_specific<float>;

/*****************************************************************************
* Defines
* **************************************************************************/
#define CPU 0
#define GPU 1
#define GPU_OFF -100 //Arbitrary value
#define SEP "\t"

/*****************************************************************************
* types
* **************************************************************************/
typedef struct{
	int numcpus;
	int numgpus;
	int gpuChunk;
	char benchName[100];
	char kernelName[100];
	int initiationInterval;
	float frequency;
	int elemPerIter;
	float thobjective;
	pid_t main_thread_id;
	size_t CHUNKGPUALIGN=1;
	size_t CHUNKCPUALIGN=1;
	size_t INITCHUNK=1;
	bool WARMUPFPGA=true;
} Params;

/*****************************************************************************
* Global variables
* **************************************************************************/
__thread int chunkCPU;      // chunk de CPU individual por thread
int chunkGPU;               // chunk unico de GPU/ACC (Dynamic)
__thread float fG = 0.0;
#define NACC 16             // numero maximo de aceleradores
float gpuThroughput[NACC];  // throughput de cada acelerador
float cpuThroughput =0.0;
PrivFloat_t cpuThroughputETS;  // throughput de la cpu asociada al thread actual
float avgCpuThroughput = 0.0;
int maxChunkCPU = 0;
constexpr static float AVG_ALFA = 0.99;
__thread int myAccId = 0;   // identificador del acelerador controlado pro el thread actual
bool recordThroughput[NACC];    // para no guardar throughput del ultimo trozo posiblemente en regimen suboptimo
int nAcc;                   // numero inicial de aceleradores (para reparto inicial)

constexpr static size_t fGinit=8;  // multiplicador inicial del accelerador respecto a las cpus

#ifdef MODA
#define BINS 10
float maxThBin;	// maximo TH del penultimo bin (el ultimo bin incluye desde ese TH hacia arriba)
float limits[BINS]; // limites de TH de los BINS
unsigned int freq[BINS+1]; // frecuencia de ocurrencia de los BINS
float means[BINS+1]; // media de TH de los TH de ese bin

void initModaBins(float _maxThBin) {
	maxThBin = _maxThBin;
	float f = maxThBin / (float)BINS;
	limits[0] = f;
	freq[0] = 0;
	means[0] = 0.0f;
	for (int i = 1; i < BINS ; i++) {
		limits[i] = limits[i-1] + f;
		freq[i] = 0;
		means[i] = 0.0f;
	}
	freq[BINS] = 0;
	means[BINS] = 0.0f;
}

unsigned int getModaBin(float th) {  // devuelve bin de un determinado TH
	unsigned int bin = 0;
	while (th > limits[bin] && bin < BINS)
		bin++;
	return bin;
}

void addModaTH(float th) { // añade un nuevo TH y actualiza todas las estructuras
	const float alfa = 0.9;
	unsigned int bin = getModaBin(th);
	freq[bin]++;
	if (means[bin] == 0) {
		means[bin] = th;
	} else {
		means[bin] = alfa * means[bin] + (1-alfa) * th;
	}
}

float getModa() { // devuelve valor de la moda (valor medio del bin con mayor frecuencia)
	unsigned int max = freq[0];
	int maxpos = 0;
	for (int i = 1; i <= BINS; i++) {
		if (freq[i] > max) {
			max = freq[i];
			maxpos = i;
		}
	}
	return means[maxpos];
}
#endif

// Model Variables
double F_global;
double DL_global;
double NE_global;
double II_global;
double first_time;
int first_chunk;

// ALIGN
size_t CHUNKGPUALIGN=1;   // los chunks de ACC seran siempre multiplos de este numero (si no no se envia a ACC)
size_t CHUNKCPUALIGN=1;    // los chunks de CPU seran multiplos de este numero a exepción del ultimo trozo del espacio de it.
constexpr static size_t MINCHUNKCPU = 10;
constexpr static size_t INITCHUNKCPU = 10;

size_t ALIGN(size_t A, size_t B) {
	return std::max(B, static_cast<size_t>(round(A/static_cast<float>(B)) * B));
}

size_t ALIGNFLOOR(size_t A, size_t B) {
	return static_cast<size_t>(floor(A/static_cast<float>(B)) * B);
}

size_t ALIGNGPU(size_t A) {
	return ALIGN(A, CHUNKGPUALIGN);
}

size_t ALIGNFLOORGPU(size_t A) {
	return ALIGNFLOOR(A, CHUNKGPUALIGN);
}

size_t ALIGNCPU(size_t A) {
	return ALIGN(A, CHUNKCPUALIGN);
}


// To calculate scheduling partition overhead
tick_count end_tc;

#ifdef DEEP_CPU_REPORT
ofstream deep_cpu_report;
#include <tbb/spin_mutex.h>
using myMutex_t = spin_mutex;
myMutex_t deepCPUMutex;
#endif

#ifdef DEEP_GPU_REPORT
ofstream deep_gpu_report;
#include <tbb/spin_mutex.h>
using myMutex_t = spin_mutex;
myMutex_t deepGPUMutex;
#endif

#ifdef DEEP_GPU_EX_REPORT
ofstream deep_gpu_ex_report;
#endif

/*****************************************************************************
* Heterogeneous Scheduler
* **************************************************************************/
/*Bundle class: This class is used to store the information that items need while walking throught pipeline's stages.*/
#define FREQFAC 1 // numero de veces mas frecuente bloques de CPU que de ACC
class Bundle {
public:
	int begin;
	int end;
	int type; // 0 -> CPU, >0 -> Id del acelerador para el computo del bundle
	Bundle() {};
};

/*My serial filter class represents the partitioner of the engine. This class selects a device and a rubrange of iterations*/
class MySerialFilter: public filter {
private:
	int begin;
	int end;
	int nCPUs;
	int nGPUs;
public:
	/*Class constructor, it only needs the first and last iteration indexes.*/
	MySerialFilter(int b, int e, int ncpus, int ngpus) : filter(true) {
		begin = b;
		end = e;
		nCPUs = ncpus;
		nGPUs = ngpus;
	}

	/* calcula trozo para FPGA cuando no hay suficiente para cores+FPGA basandose
	en modelo de TH para la FPGA */
	int calculateLastAccChunk(int remain, double THcores) {
		#ifdef DEBUG
		printf("rem %d F %f DL %f NE %f II %f, THcores %f\n", remain, F_global, DL_global, NE_global, II_global, THcores);
		#endif
		return ((double)remain * F_global - DL_global * NE_global * THcores)/(II_global * NE_global * THcores + F_global);
	}

	/* calcula trozo para FPGA cuando no hay suficiente para cores+FPGA aproximando
	tiempo ejecucion en FPGA lineal entre (habiendo medido tiempo para ck inicial 1 y teniendo
	el tiempo para el ultimo chunkGPU) */
	int calculateLastAccChunk2(int remain, double THcores, double time2, int chk2) {
		double m = (time2 - first_time) / double(chk2 - first_chunk);
		double newck = (m*(double)first_chunk - first_time)*THcores + (double)remain;
		newck = newck / (m*THcores + 1.0);
		return (int)newck;
	}

	float totalCPUThroughput() {
		float totTh = 0.0;
		for (PrivFloat_t::const_iterator i=cpuThroughputETS.begin(); i!=cpuThroughputETS.end();  ++i)  {
			totTh += *i;
			}
		return totTh;
	}

	float calculateAvgCPUTH(float newTH) {
		if (newTH > 0) {
			#ifndef MODA
			if (avgCpuThroughput == 0) {
				avgCpuThroughput = newTH;
			} else {
				//float miAlfa = AVG_ALFA * (float)chunkCPU / (float)maxChunkCPU;
				//avgCpuThroughput = (1-miAlfa) * avgCpuThroughput + miAlfa * cpuThroughputr;
				avgCpuThroughput = AVG_ALFA * avgCpuThroughput + (1-AVG_ALFA) * newTH;
				//if (newTH < avgCpuThroughput)
				//	avgCpuThroughput = newTH;
			}
			#else // MODA
			addModaTH(newTH);
			avgCpuThroughput = getModa();
			#endif
		}
	}

#ifndef FINALPHASE
	/*Mandatory operator method, TBB rules*/
	void * operator()(void *) {
		Bundle *bundle = new Bundle();

		/*If there are remaining iterations*/
		if (begin < end) {
			bundle->begin = begin;
			if (nAcc && !myAccId) { // asignar al thread un acelerador si hay libre y no tiene ya uno
				myAccId = nAcc--; // se puede modificar nAcc porque esta etapa es serie
			}
			int remainAlign = ALIGNFLOORGPU(end-begin);
			if ( myAccId && remainAlign){
				// vemos si es un thread con acelerador asociado
				// y si puede coger un trozo alineado (distinto de 0)
				#ifdef TRACER
				char cad[256];
				sprintf(cad, "SACC%d", myAccId);
				tracer.nodeStart(0, cad);
				#endif
				//ACC WORK
				int auxend = begin+chunkGPU;
				if (auxend > end) {
					recordThroughput[myAccId] = false;
					auxend = begin+remainAlign;
				}
				bundle->end = auxend;
				begin = bundle->end;
				bundle->type = myAccId;  // el tipo ahora lleva el id del acelerador (aunque no haria falta ya que esta asociado al thread)
				#ifdef TRACER
				tracer.nodeStop(0, cad);
				#endif
				return bundle;
			} else {
				#ifdef TRACER
				tracer.nodeStart(0, "SCPU");
				#endif
				//CPU WORK
				/*Calculating chunkCPU*/
				// buscamos al thr. maximo de los acceleradores para sincronizarnos con el
				float maxGpuThroughput = gpuThroughput[1];
				for (int i=2; i < nGPUs+1; i++)
					maxGpuThroughput = std::max(maxGpuThroughput, gpuThroughput[i]);
				// calculamos moving average del cpuThroughput
				//if (chunkCPU > maxChunkCPU) maxChunkCPU = chunkCPU;
				PrivFloat_t::reference cpuThroughputr = cpuThroughputETS.local();
				calculateAvgCPUTH(cpuThroughputr);
				if(maxGpuThroughput > 0 && avgCpuThroughput > 0){
					fG = maxGpuThroughput/avgCpuThroughput;
					chunkCPU = ((end-begin < chunkGPU)? end-begin : chunkGPU) / (FREQFAC * fG) ;
					chunkCPU = std::min((float)chunkCPU, (float)(end-begin)/((float)nCPUs + fG));
				} else {
					chunkCPU = INITCHUNKCPU; //min((int)(chunkGPU / fGinit), (int)((end-begin)/100));
				}
				if(chunkCPU < MINCHUNKCPU) chunkCPU=MINCHUNKCPU;  // aqui si se establece minimo para CPU
				if(nGPUs==0) //implement guided scheduling between the cores
				{
					chunkCPU=max((end-begin)/(nCPUs), 1);
				}
				chunkCPU = ALIGNCPU(chunkCPU);
				#ifdef DEBUG
				printf("New CPU SIZE: %d THcpu: %f avgTHcpu: %f THgpu: %f fG %f\n", chunkCPU, cpuThroughputr, avgCpuThroughput, maxGpuThroughput, fG);
				#endif
				/*Taking a iteration chunk for CPU*/
				bundle->end = std::min(begin + chunkCPU, end);
				begin = bundle->end;
				bundle->type = CPU;
				#ifdef TRACER
				tracer.nodeStop(0, "SCPU");
				#endif
				return bundle;
			}
		}
		return NULL;
	} // end operator
#else  // with FINALPHASE ***********************************************************
	/*Mandatory operator method, TBB rules*/
	void * operator()(void *) {
		Bundle *bundle = new Bundle();

		/*If there are remaining iterations*/
		if (begin < end) {
			bundle->begin = begin;
			if (nAcc && !myAccId) { // asignar al thread un acelerador si hay libre y no tiene ya uno
				myAccId = nAcc--; // se puede modificar nAcc porque esta etapa es serie
			}
			if (myAccId){
				// vemos si es un thread con acelerador asociado
				#ifdef TRACER
				char cad[256];
				sprintf(cad, "SACC%d", myAccId);
				tracer.nodeStart(0, cad);
				#endif
				//ACC WORK
				// vemos si hay trabajo para todos los workers
				float CPUsTH = totalCPUThroughput();
				//float CPUsTH = avgCpuThroughput * nCPUs;
				int nextChunk = ALIGNFLOORGPU(chunkGPU);
				if (gpuThroughput[myAccId] != 0 && CPUsTH != 0 && (end - begin) * gpuThroughput[myAccId] / (gpuThroughput[myAccId]+CPUsTH) < chunkGPU) {
					// no hay suficiente
					recordThroughput[myAccId] = false;
					//nextChunk = calculateLastAccChunk((end-begin), CPUsTH);
					nextChunk = calculateLastAccChunk2((end-begin), CPUsTH, (double)chunkGPU/gpuThroughput[myAccId], chunkGPU);
					nextChunk = ALIGNFLOORGPU(nextChunk);
					#ifdef DEBUG
					printf("New ACC SIZE: %d (begin %d THACC %f THCPUs %f)\n", nextChunk, begin, gpuThroughput[myAccId], CPUsTH);
					#endif
				}
				if (nextChunk > 0) { // se le puede dar chunk alineado
					bundle->end = std::min(begin + nextChunk, end);
					begin = bundle->end;
					bundle->type = myAccId;
					#ifdef TRACER
					tracer.nodeStop(0, cad);
					#endif
					return bundle;
				} else { // no se le puede dar chunk alineado, dar a CPU .. ultimos trozos
					float maxGpuThroughput = gpuThroughput[myAccId];
					if(maxGpuThroughput > 0 && avgCpuThroughput > 0){
						fG = maxGpuThroughput/avgCpuThroughput;
						chunkCPU = ((end-begin < chunkGPU)? end-begin : chunkGPU) / (FREQFAC * fG) ;
						chunkCPU = std::min((float)chunkCPU, (float)(end-begin)/((float)nCPUs + fG));
					} else {
						chunkCPU = INITCHUNKCPU; //min((int)(chunkGPU / fGinit), (int)((end-begin)/100));
					}
					if(chunkCPU < MINCHUNKCPU) chunkCPU=MINCHUNKCPU;  // aqui si se establece minimo para CPU
					if(nGPUs==0) //implement guided scheduling between the cores
					{
						chunkCPU=max((end-begin)/(nCPUs), 1);
					}
					chunkCPU = ALIGNCPU(chunkCPU);
					/*Taking a iteration chunk for CPU*/
					bundle->end = std::min(begin + chunkCPU, end);
					begin = bundle->end;
					bundle->type = CPU;
					#ifdef TRACER
					tracer.nodeStop(0, cad);
					#endif
					return bundle;
				}
			} else { //CPU WORK
				#ifdef TRACER
				tracer.nodeStart(0, "SCPU");
				#endif
				/*Calculating chunkCPU*/
				// buscamos al thr. maximo de los acceleradores para sincronizarnos con el
				float maxGpuThroughput = gpuThroughput[1];
				for (int i=2; i < nGPUs+1; i++)
					maxGpuThroughput = std::max(maxGpuThroughput, gpuThroughput[i]);
				// calculamos moving average del cpuThroughput
				//if (chunkCPU > maxChunkCPU) maxChunkCPU = chunkCPU;
				PrivFloat_t::reference cpuThroughputr = cpuThroughputETS.local();
				calculateAvgCPUTH(cpuThroughputr);
				if(maxGpuThroughput > 0 && avgCpuThroughput > 0){
					fG = maxGpuThroughput/avgCpuThroughput;
					chunkCPU = ((end-begin < chunkGPU)? end-begin : chunkGPU) / (FREQFAC * fG) ;
					chunkCPU = std::min((float)chunkCPU, (float)(end-begin)/((float)nCPUs + fG));
				} else {
					chunkCPU = INITCHUNKCPU; //min((int)(chunkGPU / fGinit), (int)((end-begin)/100));
				}
				if(chunkCPU < MINCHUNKCPU) chunkCPU=MINCHUNKCPU;  // aqui si se establece minimo para CPU
				if(nGPUs==0) //implement guided scheduling between the cores
				{
					chunkCPU=max((end-begin)/(nCPUs), 1);
				}
				chunkCPU = ALIGNCPU(chunkCPU);
				#ifdef DEBUG
				printf("New CPU SIZE: %d THcpu: %f avgTHcpu: %f THgpu: %f fG %f\n", chunkCPU, cpuThroughputr, avgCpuThroughput, maxGpuThroughput, fG);
				#endif
				/*Taking a iteration chunk for CPU*/
				bundle->end = std::min(begin + chunkCPU, end);
				begin = bundle->end;
				bundle->type = CPU;
				#ifdef TRACER
				tracer.nodeStop(0, "SCPU");
				#endif
				return bundle;
			}
		}
		return NULL;
	} // end operator
#endif
};

/*MyParallelFilter class is the executor component of the engine, it executes the subrange onto the device selected by SerialFilter*/
template <class B>
class MyParallelFilter: public filter {
private:
	B *body;

public:
	/*Class' constructor*/
	//template <class B>
	MyParallelFilter(B *b) : filter(false) {
		body = b;
	}

	/*Operator function*/
	void * operator()(void * item) {
		//variables
		Bundle *bundle = (Bundle*) item;

		if(bundle->type != CPU){
			// ACC WORK
			#ifndef NDEBUG
			cerr << "launchGPU(): begin: " << bundle->begin << " end: " << bundle->end << endl;
			#endif
			#ifdef TRACER
			char cad[256];
			sprintf(cad, "ACC%d", bundle->type);
			tracer.nodeStart(bundle->type, cad);
			#endif
			tick_count start_tc = tick_count::now();

			// Version multi-acelerador
			//body->sendObjectToGPU(bundle->begin, bundle->end, NULL, bundle->type);
			//body->OperatorGPU(bundle->begin, bundle->end, NULL, bundle->type);
			//body->getBackObjectFromGPU(bundle->begin, bundle->end, NULL, bundle->type);
			body->sendObjectToGPU(bundle->begin, bundle->end, NULL);
			#ifdef DEEP_GPU_EX_REPORT
			cl_event event_kernel;
			body->OperatorGPU(bundle->begin, bundle->end, &event_kernel);
			#else
			body->OperatorGPU(bundle->begin, bundle->end, NULL);
			#endif
			body->getBackObjectFromGPU(bundle->begin, bundle->end, NULL);
			#ifdef BSCODE
			clFinish(accumulate_queue);
			#else
			clFinish(command_queue);
			#endif

			end_tc = tick_count::now();
			#ifdef TRACER
			tracer.nodeStop(bundle->type, cad);
			#endif
			float time =(end_tc - start_tc).seconds()*1000;
			if (recordThroughput[bundle->type])
			gpuThroughput[bundle->type] = (bundle->end - bundle->begin) / time ;
			#ifdef DEBUG
			printf("FPGA SIZE:%d  TH: %f time: %f\n", bundle->end - bundle->begin, (bundle->end - bundle->begin) / time, time);
			#endif
			#ifdef DEEP_GPU_REPORT
			{
				myMutex_t::scoped_lock myLock(deepGPUMutex);
				deep_gpu_report << bundle->end-bundle->begin << "\t" << (bundle->end - bundle->begin) / time << endl;
			}
			#endif
			#ifdef DEEP_GPU_EX_REPORT
			cl_ulong tgx1, tgx2;
			cl_int error = clGetEventProfilingInfo(event_kernel, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &tgx1, NULL);
			if(error != CL_SUCCESS) {
				cerr << "Error profiling kernel execution time (start)";
				exit(error);
			}
			error = clGetEventProfilingInfo(event_kernel, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &tgx2, NULL);
			if(error != CL_SUCCESS) {
				cerr << "Error profiling kernel execution time (end)";
				exit(error);
			}
			clReleaseEvent(event_kernel);
			double timex;
			if (tgx2 > tgx1) {
				timex = (tgx2 - tgx1) / 1000000.0; // ms
				deep_gpu_ex_report << bundle->end-bundle->begin << "\t" << (bundle->end - bundle->begin) / timex << endl;
			} else {
				cerr << "Error profiling kernel execution time" << endl;
				//exit(-1);
			}
			#endif
		}else{
			// CPU WORK
			#ifndef NDEBUG
			cerr << "launchCPU(): begin: " << bundle->begin << " end: " << bundle->end << endl;
			#endif
			#ifdef TRACER
			tracer.nodeStart(10, "CPU");
			#endif
			tick_count start = tick_count::now();
			body->OperatorCPU(bundle->begin, bundle->end);
			tick_count end = tick_count::now();
			#ifdef TRACER
			tracer.nodeStop(10, "CPU");
			#endif
			float time =(end-start).seconds()*1000;
			PrivFloat_t::reference cpuThroughputr = cpuThroughputETS.local();
			cpuThroughputr = (bundle->end - bundle->begin) / time;
			chunkCPU = (bundle->end - bundle->begin);
			#ifdef DEBUG
			printf("CPU SIZE:%d  TH: %f time: %f\n", bundle->end - bundle->begin, (bundle->end - bundle->begin) / time, time);
			#endif
			#ifdef DEEP_CPU_REPORT
			{
				myMutex_t::scoped_lock myLock(deepCPUMutex);
				deep_cpu_report << bundle->end-bundle->begin << "\t" << (bundle->end - bundle->begin) / time
				<< "\t" << avgCpuThroughput << endl;
			}
			#endif
		}
		/*Deleting bundle to avoid memory leaking*/
		delete bundle;
		return NULL;
	}
};
//end class

/*My serial filter class represents the partitioner of the engine. This class selects a device and a rubrange of iterations*/
class MySerialFilterProf: public filter {
private:
	int begin;
	int end;
	int nCPUs;
	int nGPUs;
	int profChunk[2];
	int actChunk;
public:
	/*Class constructor, it only needs the first and last iteration indexes.*/
	MySerialFilterProf(int b, int e, int ncpus, int ngpus, int _profChunk1, int _profChunk2) : filter(true) {
		begin = b;
		end = e;
		nCPUs = ncpus;
		nGPUs = ngpus;
		profChunk[0] = _profChunk1;
		profChunk[1] = _profChunk2;
		actChunk = 0;
	}

	/*Mandatory operator method, TBB rules*/
	void * operator()(void *) {
		Bundle *bundle = new Bundle();

		/*If there are remaining iterations*/
		if (begin < end) {
			bundle->begin = begin;
			if (nAcc && !myAccId) { // asignar al thread un acelerador si hay libre y no tiene ya uno
				myAccId = nAcc--; // se puede modificar nAcc porque esta etapa es serie
			}
			if ( myAccId ){
				if (actChunk == 2) return NULL;  // if 2 chunks have been profiled, stop the pipeline
				// vemos si es un thread con acelerador asociado
				// y vemos si podemos coger trozo actual para profiling
				if (end-begin < profChunk[actChunk]) {
					cerr << "Not enough iterations to profile" << endl;
					exit(-1);
				}
				#ifdef TRACER
				char cad[256];
				sprintf(cad, "PACC%d", myAccId);
				tracer.nodeStart(0, cad);
				#endif
				//ACC WORK
				bundle->end = begin+profChunk[actChunk++];
				begin = bundle->end;
				bundle->type = myAccId;  // el tipo ahora lleva el id del acelerador (aunque no haria falta ya que esta asociado al thread)
				#ifdef TRACER
				tracer.nodeStop(0, cad);
				#endif
				return bundle;
			} else {
				#ifdef TRACER
				tracer.nodeStart(0, "PCPU");
				#endif
				//CPU WORK
				/*Taking a iteration chunk for CPU*/
				bundle->end = std::min(begin + (int)MINCHUNKCPU, end);
				begin = bundle->end;
				bundle->type = CPU;
				#ifdef TRACER
				tracer.nodeStop(0, "PCPU");
				#endif
				return bundle;
			}
		}
		return NULL;
	} // end operator
};

/*MyParallelFilter class is the executor component of the engine, it executes the subrange onto the device selected by SerialFilter*/
template <class B>
class MyParallelFilterProf: public filter {
private:
	B *body;
	int actChunk;

public:
	double times[2];
	/*Class' constructor*/
	//template <class B>
	MyParallelFilterProf(B *b) : filter(false) {
		body = b;
		actChunk = 0;
	}

	/*Operator function*/
	void * operator()(void * item) {
		//variables
		Bundle *bundle = (Bundle*) item;

		if(bundle->type != CPU){
			// ACC WORK
			#ifndef NDEBUG
			cerr << "launchGPU(): begin: " << bundle->begin << " end: " << bundle->end << endl;
			#endif
			#ifdef TRACER
			char cad[256];
			sprintf(cad, "PACC%d", bundle->type);
			tracer.nodeStart(bundle->type, cad);
			#endif

			#ifdef NOOCLPROF

			tick_count start_tc = tick_count::now();
			body->sendObjectToGPU(bundle->begin, bundle->end, NULL);
			body->OperatorGPU(bundle->begin, bundle->end, NULL);
			body->getBackObjectFromGPU(bundle->begin, bundle->end, NULL);
			#ifdef BSCODE
			clFinish(accumulate_queue);
			#else
			clFinish(command_queue);
			#endif
			double time = (tick_count::now() - start_tc).seconds() * 1000; // ms

			#else  //NOOCLPROF

			tick_count start_tc = tick_count::now();
			body->sendObjectToGPU(bundle->begin, bundle->end, NULL);
			cl_event event_kernel;
			body->OperatorGPU(bundle->begin, bundle->end, &event_kernel);
			body->getBackObjectFromGPU(bundle->begin, bundle->end, NULL);
			#ifdef BSCODE
			clFinish(accumulate_queue);
			#else
			clFinish(command_queue);
			#endif
			double auxtime = (tick_count::now() - start_tc).seconds(); // ms
			if (actChunk == 0) { // first chunk
				first_time = auxtime;
				first_chunk = bundle->end - bundle->begin;
			}

			cl_ulong tg1, tg2;
			cl_int error = clGetEventProfilingInfo(event_kernel, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &tg1, NULL);
			if(error != CL_SUCCESS) {
				cerr << "Error profiling kernel execution time (start)";
				exit(error);
			}
			error = clGetEventProfilingInfo(event_kernel, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &tg2, NULL);
			if(error != CL_SUCCESS) {
				cerr << "Error profiling kernel execution time (end)";
				exit(error);
			}
			clReleaseEvent(event_kernel);
			double time;
			if (tg2 > tg1) {
				time = (tg2 - tg1) / 1000000.0; // ms
			} else {
				cerr << "Error profiling kernel execution time";
				exit(-1);
			}
			#endif //NOOCLPROF

			#ifdef TRACER
			tracer.nodeStop(bundle->type, cad);
			#endif

			times[actChunk++] = time;
			if (recordThroughput[bundle->type])
			gpuThroughput[bundle->type] = (bundle->end - bundle->begin) / time ;
			#ifdef DEBUG
			printf("FPGA SIZE:%d  TH: %f\n", bundle->end - bundle->begin, (bundle->end - bundle->begin) / time);
			#endif
			#ifdef DEEP_GPU_REPORT
			{
				myMutex_t::scoped_lock myLock(deepGPUMutex);
				deep_gpu_report << bundle->end-bundle->begin << "\t" << (bundle->end - bundle->begin) / time << endl;
			}
			#endif
		}else{
			// CPU WORK
			#ifndef NDEBUG
			cerr << "launchCPU(): begin: " << bundle->begin << " end: " << bundle->end << endl;
			#endif
			#ifdef TRACER
			tracer.nodeStart(10, "PCPU");
			#endif
			tick_count start = tick_count::now();
			body->OperatorCPU(bundle->begin, bundle->end);
			tick_count end = tick_count::now();
			#ifdef TRACER
			tracer.nodeStop(10, "PCPU");
			#endif
			float time =(end-start).seconds()*1000;
			PrivFloat_t::reference cpuThroughputr = cpuThroughputETS.local();
			cpuThroughputr = (bundle->end - bundle->begin) / time;
			#ifdef DEEP_CPU_REPORT
			{
				myMutex_t::scoped_lock myLock(deepCPUMutex);
				deep_cpu_report << bundle->end-bundle->begin << "\t" << (bundle->end - bundle->begin) / time << endl;
			}
			#endif
		}
		/*Deleting bundle to avoid memory leaking*/
		delete bundle;
		return NULL;
	}
};
//end class

/*FastFit Class: This scheduler version let us to split the workload in two subranges, one for GPU and one for CPUs*/
class FastFit : public Scheduler<FastFit, Params> {
	Params * pars;
private:
	/* Funcion para calcular el chunk optimo para FPGA basada en el tiempo de 1 iteracion*/
	/*
	int calculateChunk(double oneIterTime, int initiationInterval, float frequency, int elementPerIter, float percObjective) {
		float F = frequency / (float) elementPerIter;
		float CL = (oneIterTime * (double) F);
		float DL = CL - initiationInterval;
		return DL / (initiationInterval * (1.0 - percObjective) / percObjective);
	}*/

	/* Funcion para calcular el chunk optimo para FPGA basada en el tiempo un chunk de cualquier size*/
	int calculateChunk(double Time, int numIter, int initiationInterval, double frequency, double elementPerIter, double percObjective) {
		F_global = frequency;
		II_global = initiationInterval;
		double F = frequency / (double) elementPerIter;
		double CL = (Time * F);
		double DL = CL - ((double)numIter * (double)initiationInterval);
		if (DL < 0) return -1;
		DL_global = DL;
		return ( DL * percObjective ) / ((double)initiationInterval * (1.0 - percObjective));
	}

	double calculateNE(int initiationInterval, double frequency, int chunk1, int chunk2, double time1, double time2) {
		double NE = (frequency / (double)initiationInterval) * (time2 - time1)/(double)(chunk2 - chunk1);
		NE_global = NE;
		return NE;
	}

	template<class T>
	double profileOneChunk(int &begin, int end, int chunk, T* body) {
		cl_ulong tg1, tg2;
		cl_int error;

		if (begin+chunk > end) {
			cerr << "There is not enough iterations to profile: "
			<< begin << " + " << chunk << " = " << begin+chunk << " > "  << end << endl;
			exit(-1);
		}


		#ifdef NOOCLPROF

		tick_count start_tc = tick_count::now();
		body->sendObjectToGPU(begin, begin+chunk, NULL);
		body->OperatorGPU(begin, begin+chunk, NULL);
		body->getBackObjectFromGPU(begin, begin+chunk, NULL);
		#ifdef BSCODE
		clFinish(accumulate_queue);
		#else
		clFinish(command_queue);
		#endif
		double time = (tick_count::now() - start_tc).seconds();

		#else // NOOCLPROF

		body->sendObjectToGPU(begin, begin+chunk, NULL);
		cl_event event_kernel;
		body->OperatorGPU(begin, begin+chunk, &event_kernel);
		body->getBackObjectFromGPU(begin, begin+chunk, NULL);
		#ifdef BSCODE
		clFinish(accumulate_queue);
		#else
		clFinish(command_queue);
		#endif
		error = clGetEventProfilingInfo(event_kernel, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &tg1, NULL);
		if(error != CL_SUCCESS) {
			cerr << "Error profiling kernel execution time (start)";
			exit(error);
		}
		error = clGetEventProfilingInfo(event_kernel, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &tg2, NULL);
		if(error != CL_SUCCESS) {
			cerr << "Error profiling kernel execution time (end)";
			exit(error);
		}
		clReleaseEvent(event_kernel);
		double time;
		if (tg2 > tg1) {
			time = (tg2 - tg1) / 1000000000.0; // s
		} else {
			cerr << "Error profiling kernel execution time";
			exit(-1);
		}

		#endif //NOOCLPROF

		begin += chunk;
		return time;
	}

	template<class T>
	void profileTwoChunk(int &begin, int end, int chunk1, int chunk2, T* body, double &time1, double &time2) {
		if (begin+chunk1+chunk2 > end) {
			cerr << "There is not enough iterations to profile: "
			<< begin << " + " << chunk1 << " + " << chunk2 << " = "
			<< begin+chunk1+chunk2 << " > "  << end << endl;
			exit(-1);
		}

		pipeline pipe;
		MySerialFilterProf serial_filter(begin, end, nCPUs, nGPUs, chunk1, chunk2);
		MyParallelFilterProf<T> parallel_filter(body);
		pipe.add_filter(serial_filter);
		pipe.add_filter(parallel_filter);
		body->firsttime = true;
		/*Run the pipeline*/
		//pipe.run(nCPUs + nGPUs);
		pipe.run(nGPUs); // cpus stopped while profiling
		time1 = parallel_filter.times[0]/1000.0;
		time2 = parallel_filter.times[1]/1000.0;
		begin += chunk1 + chunk2;
		pipe.clear();
	}

public:
	/*This constructor just call his parent's contructor*/
	FastFit(Params *params) : Scheduler<FastFit, Params>(params),
	pars(params)
	{
		chunkCPU = 0;
		fG = 0.0;
		chunkGPU = 0.0;
		PrivFloat_t::reference cpuThroughputr = cpuThroughputETS.local();
		cpuThroughputr=0.0;
		avgCpuThroughput = 0.0;
		for (int i=0; i<= pars->numgpus; i++) {  // th de todos los ACC = 0
			gpuThroughput[i] = 0;
			recordThroughput[i] = true;
		}
		nAcc = pars->numgpus;       // numero inicial de ACC
		myAccId = 0;                // no acc asociado al thread
		CHUNKGPUALIGN=pars->CHUNKGPUALIGN;
		CHUNKCPUALIGN=pars->CHUNKCPUALIGN;
	}

	/*The main function to be implemented*/
	template<class T>
	void heterogeneous_parallel_for(int begin, int end, T* body){
		#ifndef NDEBUG
		cerr << "Heterogeneous Parallel For Dynamic " << nCPUs << " , " << nGPUs << ", " << chunkGPU << endl;
		#endif

		if (pars->thobjective < 0.5f || pars->thobjective > 1.0f)
			pars->thobjective = 0.90f;

		// Profiling if chunkGPU == 0
		bool profiling = !chunkGPU;
		if (profiling) {
			#ifdef TRACER
			tracer.nodeStart(11, "ProfACC");
			#endif

			for (int i=0; i<= pars->numgpus; i++) {  // th de todos los ACC = 0
				recordThroughput[i] = false;
			}
			body->firsttime = true;

			if(pars->WARMUPFPGA) {
				double time = profileOneChunk(begin, end, 1, body);
				cout << "  Time 1 it: " << time << endl;
			}

			double time1, time2;
			int chunk1 = ALIGNGPU(pars->INITCHUNK);//(end-begin)/200;
			int chunk2 = ALIGNGPU((end-begin)/20);
			profileTwoChunk(begin, end, chunk1, chunk2, body, time1, time2);
			cout << "Prof chunk1 " << chunk1 << " time " << time1 << endl;
			cout << "Prof chunk2 " << chunk2 << " time " << time2 << endl;

			#ifdef MODA
			initModaBins(chunk2/(time2*1000));
			#endif

			//first_time = time1;
			//first_chunk = chunk1;

			double NE = calculateNE(pars->initiationInterval, pars->frequency, chunk1, chunk2, time1, time2);
			int auxchk = calculateChunk(time1, chunk1, pars->initiationInterval, pars->frequency, NE, pars->thobjective);

			if (NE <= 0 || auxchk <= 0) {
				chunk2 = (end-begin)/10;
				time2 = profileOneChunk(begin, end, chunk2, body);
				cout << "2 Prof chunk2 " << chunk2 << " time " << time2 << endl;
				NE = calculateNE(pars->initiationInterval, pars->frequency, chunk1, chunk2, time1, time2);
				int auxchk = calculateChunk(time1, chunk1, pars->initiationInterval, pars->frequency, NE, pars->thobjective);
				if (NE <= 0 || auxchk <= 0) {
					cerr << "We can't calculate NE" << endl;
					exit(-1);
				}
			}
			chunkGPU = ALIGNGPU(calculateChunk(time1, chunk1, pars->initiationInterval, pars->frequency, NE, pars->thobjective));
			cout << " CHK1 " <<  chunkGPU << endl;
			cout << " CHK2 " << calculateChunk(time2, chunk2, pars->initiationInterval, pars->frequency, NE, pars->thobjective) << endl;
			cout << "Calculated Chunk " << chunkGPU << endl;
			cout << "  Time " << time1 << endl;
			cout << "  II " << pars->initiationInterval << endl;
			cout << "  Freq " << pars->frequency << endl;
			cout << "  Elem x Iter " << NE << endl;
			cout << "  %TH " << pars->thobjective << endl;

			#ifdef TRACER
			tracer.nodeStop(11, "ProfACC");
			#endif

		}

		// Iniciliazacion
		for (int i=0; i<= pars->numgpus; i++) {  // th de todos los ACC = 0
			recordThroughput[i] = true;
		}
#ifdef DEBUG
		cout << " ***** NEW Time Step ****************************" << endl;
#endif
		/*Preparing pipeline*/
		pipeline pipe;
		MySerialFilter serial_filter(begin, end, nCPUs, nGPUs);
		MyParallelFilter<T> parallel_filter(body);
		pipe.add_filter(serial_filter);
		pipe.add_filter(parallel_filter);
		//chunkCPU = chunkGPU * 0.2;
		if(fG==0.0) fG = 8; // 25 para Nbody?
		chunkCPU = INITCHUNKCPU; //min((int)(chunkGPU / fG), (int)((end-begin)/100));
		if(!chunkCPU) chunkCPU=1;
		body->firsttime = !profiling;

		#ifdef DEEP_CPU_REPORT
		char nombre[1024];
		sprintf(nombre, "%s_Fastfit_deep_CPU_report_GPU_%d.txt", pars->benchName, chunkGPU);


		deep_cpu_report.open(nombre, ios::out | ios::app);
		#endif
		#ifdef DEEP_GPU_REPORT
		{
			char nombre[1024];
			sprintf(nombre, "%s_Fastfit_deep_GPU_report_GPU_%d.txt", pars->benchName, chunkGPU);
			deep_gpu_report.open(nombre, ios::out | ios::app);
		}
		#endif
		#ifdef DEEP_GPU_EX_REPORT
		{
			char nombre[1024];
			sprintf(nombre, "%s_Fastfit_deep_GPU_ex_report_GPU_%d.txt", pars->benchName, chunkGPU);
			deep_gpu_ex_report.open(nombre, ios::out | ios::app);
		}
		#endif

		/*Run the pipeline*/
		pipe.run(nCPUs + nGPUs);
		pipe.clear();
		#ifdef DEEP_CPU_REPORT
		deep_cpu_report.close();
		#endif
		#ifdef DEEP_GPU_REPORT
		deep_gpu_report.close();
		#endif
		#ifdef DEEP_GPU_EX_REPORT
		deep_gpu_ex_report.close();
		#endif
	}

	/*this function print info to a Log file*/
	void saveResultsForBench(){

		char * execution_name = (char *)malloc(sizeof(char)*100);
		sprintf(execution_name, "_Fastfit_%d_%d.txt", nCPUs, nGPUs);
		strcat(pars->benchName, execution_name);

		/*Checking if the file already exists*/
		bool fileExists = isFile(pars->benchName);
		ofstream file(pars->benchName, ios::out | ios::app);
		if(!fileExists){
			printHeaderToFile(file);
		}

		cout << "TIEMPO TOTAL: " << runtime << endl;
		file << nCPUs << "\t" << nGPUs << "\t"  << chunkGPU << "\t" <<  runtime << "\t" << pars->thobjective << "\t"
		#ifdef PCM

		<< getPP0ConsumedJoules(sstate1, sstate2) << "\t" << getPP1ConsumedJoules(sstate1, sstate2) << "\t"
		<< getConsumedJoules(sstate1, sstate2) - getPP0ConsumedJoules(sstate1, sstate2) - getPP1ConsumedJoules(sstate1, sstate2) << "\t" <<  getConsumedJoules(sstate1, sstate2) << "\t"
		<< getL2CacheHits(sktstate1[0], sktstate2[0]) << "\t" << getL2CacheMisses(sktstate1[0], sktstate2[0]) << "\t" << getL2CacheHitRatio(sktstate1[0], sktstate2[0]) <<"\t"
		<< getL3CacheHits(sktstate1[0], sktstate2[0]) << "\t" << getL3CacheMisses(sktstate1[0], sktstate2[0]) << "\t" << getL3CacheHitRatio(sktstate1[0], sktstate2[0]) <<"\t"
		<< getCyclesLostDueL3CacheMisses(sstate1, sstate2)

		#endif
		#ifdef PMLIB
		<< energiaCPU << "\t"  << energiaFPGA << "\t"<< energiaFPGA +energiaCPU<< "\t"

		#endif
		<< endl;
		file.close();
		#ifndef NDEBUG
		cerr << nCPUs << "\t" << nGPUs << "\t"  << runtime << "\t"
		//		<< getPP0ConsumedJoules(sstate1, sstate2) << "\t" << getPP1ConsumedJoules(sstate1, sstate2) << "\t"
		//		<< getConsumedJoules(sstate1, sstate2) - getPP0ConsumedJoules(sstate1, sstate2) - getPP1ConsumedJoules(sstate1, sstate2) << "\t" <<  getConsumedJoules(sstate1, sstate2) << "\t"
		//		<< getL2CacheHits(sktstate1[0], sktstate2[0]) << "\t" << getL2CacheMisses(sktstate1[0], sktstate2[0]) << "\t" << getL2CacheHitRatio(sktstate1[0], sktstate2[0]) <<"\t"
		//		<< getL3CacheHits(sktstate1[0], sktstate2[0]) << "\t" << getL3CacheMisses(sktstate1[0], sktstate2[0]) << "\t" << getL3CacheHitRatio(sktstate1[0], sktstate2[0]) <<"\t"
		//		<< getCyclesLostDueL3CacheMisses(sstate1, sstate2)
		<< endl;
		#endif
	}

	void printHeaderToFile(ofstream &file){
		file << "N. CPUs" << SEP << "N. GPUs" << SEP << "Chunk" << SEP << "Time (ms)" << SEP << "%TH.Peak" << SEP
		#ifdef PCM
		<< "CPU Energy(J)" << SEP << "GPU Enegy(J)" << SEP << "Uncore Energy(J)" << SEP << "Total Energy (J)" << SEP
		<< "L2 Cache Hits" << SEP << "L2 Cache Misses" << SEP << "L2 Cache Hit Ratio" << SEP
		<< "L3 Cache Hits" << SEP << "L3 Cache Misses" << SEP << "L3 Cache Hit Ratio" << SEP << "Cycles lost Due to L3 Misses"

		#endif
		#ifdef PMLIB
		<< "CPU Energy (BBB)" << SEP << "FPGA Energy (BBB) " << SEP

		#endif
		<< endl;
	}
};