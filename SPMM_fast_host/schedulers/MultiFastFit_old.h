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
//#define FINALPHASE
#define MODA  // Calculo del TH medio de las CPUs (avgCpuThroughput) usando la moda estadistica por intervalos mezclada con la media por intervalos
//#define TRACER
//#define DEEP_CPU_REPORT
//#define DEEP_GPU_REPORT
#define NDEBUG
//#define DEBUG_CHUNKS_F
//#define DEBUG_CHUNKS_C

//#define DEBUG
//#define OCL_PROFILING  // for profiling kernel execution with OCL events

#include <cstdlib>
#include <iostream>
#include <fstream>


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

//#define _GNU_SOURCE already defined
#include <sys/syscall.h>
#include <sys/types.h>
#include <sched.h>
#include <asm/unistd.h>

/* already defined in  /energy_meter/thread_funcs.cpp
int set_thread_affinity_CORE(pthread_t th, int cpu)
{
    cpu_set_t mask;
    __CPU_ZERO_S(sizeof(cpu_set_t),&mask);
    __CPU_SET_S(cpu,sizeof(cpu_set_t), &mask);

    return pthread_setaffinity_np(th, sizeof(cpu_set_t), &mask);
}
*/


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

//__thread int chunkGPU;               // chunk unico de GPU/ACC (Dynamic)
int chunkGPU;

__thread float fG = 0.0;
#define NACC 16             // numero maximo de aceleradores
float gpuThroughput[NACC];  // throughput de cada acelerador
float cpuThroughput =0.0;
//PrivFloat_t cpuThroughputETS;  // throughput de la cpu asociada al thread actual
float cpuThroughputETS[NACC];
float avgCpuThroughput = 0.0;
int maxChunkCPU = 0;
constexpr static float AVG_ALFA = 0.99;
__thread int myAccId = 0;   // identificador del acelerador controlado pro el thread actual
__thread int myCPUId = 0;
bool recordThroughput[NACC];    // para no guardar throughput del ultimo trozo posiblemente en regimen suboptimo
int nAcc;                   // numero inicial de aceleradores (para reparto inicial)
int nCpu;

constexpr static size_t fGinit=8;  // multiplicador inicial del accelerador respecto a las cpus

/* NEW VARS ANDRES --
╔╗╔┌─┐┬ ┬  ╦  ╦┌─┐┬─┐┬┌─┐┌┐ ┬  ┌─┐┌─┐
║║║├┤ │││  ╚╗╔╝├─┤├┬┘│├─┤├┴┐│  ├┤ └─┐
╝╚╝└─┘└┴┘   ╚╝ ┴ ┴┴└─┴┴ ┴└─┘┴─┘└─┘└─┘
*/
	//Params * pars;
	bool fGvalid = false;
	
	long unsigned int bodies_C=0, bodies_F=0;

//Summarize GPU statistics
int itemsOnGPU = 0;
int totalIterationsGPU = 0;

int AccStage[NACC];    // Accelerator stages
int anyFPGArunning;  // number of FPGA cores running
int stable_fase=0;
/*
 * <0.- Stopped
 *  0.- warm up
 *  1.- first chunk
 *  2.- second chunk
 * 22.- pre-stable (out of model calculation)
 *  3.- calculate model
 *  4.- stable
 * */
int global_chunk1, global_chunk2, global_chunk22;
__thread int chunk1, chunk2;
__thread double time1, time2;
// ------------------

#define MAXCHUNK2TRIES 5
// Model Variables
__thread int chunk2tries;
__thread double F_global;
__thread double DL_global;
__thread double NE_global;
__thread double II_global;
__thread double first_time;
__thread int first_chunk;
//--------------------

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
	Params * pars;
public:
	/*Class constructor, it only needs the first and last iteration indexes.*/
	MySerialFilter(int b, int e, int ncpus, int ngpus, Params * para) : filter(true) {
		begin = b;
		end = e;
		nCPUs = ncpus;
		nGPUs = ngpus;
		pars = para;
	}
		void reset_iter_space(int b, int e){
		begin = b;
		end = e;
	}
	private:
	
	/* Funcion para calcular el chunk optimo para FPGA basada en el tiempo de 1 iteracion*/
	/*
	 * ╔═╗┌─┐┬  ┌─┐┬ ┬┬  ┌─┐┌┬┐┌─┐  ╔╦╗╔═╗╔╦╗╔═╗╦  
	 * ║  ├─┤│  │  │ ││  ├─┤ │ ├┤   ║║║║ ║ ║║║╣ ║  
	 * ╚═╝┴ ┴┴─┘└─┘└─┘┴─┘┴ ┴ ┴ └─┘  ╩ ╩╚═╝═╩╝╚═╝╩═╝
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
	int calculateLastAccChunk2 (int remain, double THcores, double time2, int chk2) {
		double m = (time2 - first_time) / double(chk2 - first_chunk);
		double newck = (m*(double)first_chunk - first_time)*THcores + (double)remain;
		newck = newck / (m*THcores + 1.0);
		return (int)newck;
	}

	float totalCPUThroughput() {
		float totTh = 0.0;
		for (int i=0; i<nCPUs;  i++)  {
			totTh += cpuThroughputETS[i];
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

	/*Mandatory operator method, TBB rules*/
	void * operator()(void *) {
		Bundle *bundle = new Bundle();

		/*If there are remaining iterations*/
		if (begin < end) {
			bundle->begin = begin;
			if (nAcc && !myAccId) { // asignar al thread un acelerador si hay libre y no tiene ya uno
				myAccId = nAcc--; // se puede modificar nAcc porque esta etapa es serie
				set_thread_affinity_CORE(pthread_self(), nAcc);
                cerr << "** FPGA thread: " << nAcc+1 << endl;
                chunk2tries=MAXCHUNK2TRIES;
			}
			else if(nCpu && !myCPUId)	
			{
				myCPUId=nCpu--;
				 cerr << "** CPU thread: " << nCpu+1 << endl;
			}
			
			if (myAccId && AccStage[myAccId]>=0){				// vemos si es un thread con acelerador asociado
				 /*
                .                 _____ _              ______ _____   _____
                .                |_   _( )            |  ____|  __ \ / ____|   /\
                .                  | | |/ _ __ ___    | |__  | |__) | |  __   /  \
                .                  | |   | '_ ` _ \   |  __| |  ___/| | |_ | / /\ \
                .                 _| |_  | | | | | |  | |    | |    | |__| |/ ____ \
                .                |_____| |_| |_| |_|  |_|    |_|     \_____/_/    \_\
                .
                 */
                double NE;
                int auxchk;
                //cerr << ">  FPGA " << myAccId << " Stage " << AccStage[myAccId] << endl;
				
				switch (AccStage[myAccId]) // AccStage is updated in FPGA operartor ??
				{
				case 0: // FPGA WARMP UP
					recordThroughput[myAccId] = false;
					bundle->begin = begin;
					bundle->end = begin+1;
					begin = bundle->end;
					bundle->type = myAccId;
					bodies_F+=bundle->end-bundle->begin;
					return bundle;
					//double time = profileOneChunk(begin, end, 1, body);
					//cout << "  Time 1 it: " << time << endl;
					break;
					
				case 1: // First chunk
				    chunk1=global_chunk1;
					recordThroughput[myAccId] = false;
					bundle->begin = begin;
					bundle->end = begin+chunk1;
					begin = bundle->end;
					bundle->type = myAccId;
					bodies_F+=bundle->end-bundle->begin;
					return bundle;
					break;
					
				case 2: // Second chunk
				    recordThroughput[myAccId] = false;
				    chunk2=global_chunk2;
					bundle->begin = begin;
					bundle->end = begin+chunk2;
					begin = bundle->end;
					bundle->type = myAccId;
					bodies_F+=bundle->end-bundle->begin;
					return bundle;
					break;
			   case 22: // chunk pre-stable
			        if(!stable_fase)
			        {
				    recordThroughput[myAccId] = false;
				    chunk2=global_chunk22;
					bundle->begin = begin;
					bundle->end = begin+chunk2;
					begin = bundle->end;
					bundle->type = myAccId;
					bodies_F+=bundle->end-bundle->begin;
					return bundle;
					break;
				    }
				 
				case 3: // Calculate model. Don't break and continue with case:4
					recordThroughput[myAccId] = true; // this is for good
					AccStage[myAccId]=4;
					
					if(!stable_fase)
					{
					#ifdef MODA
					initModaBins(chunk2/(time2*1000));
					#endif
		
					//first_time = time1;
					//first_chunk = chunk1;
		
					NE = calculateNE(pars->initiationInterval, pars->frequency, chunk1, chunk2, time1, time2);
					auxchk = calculateChunk(time1, chunk1, pars->initiationInterval, pars->frequency, NE, pars->thobjective);
		            
		            cerr << "Calculate model # " << 1+MAXCHUNK2TRIES-chunk2tries << endl;
					cerr << "Prof chunk1 " << chunk1 << " time " << time1 << endl;
					cerr << "Prof chunk2 " << chunk2 << " time " << time2 << endl;
		            cerr << "NE " << NE << endl;
					cerr << "Calculated CHNK " << auxchk << endl;
		
					if (NE <= 0 || auxchk <= 0) { // Second chunk error, new try
						if(chunk2tries>0) 
						{
						chunk2tries--;
						AccStage[myAccId]=2;
						chunk2 = ALIGNGPU(global_chunk2*(1+MAXCHUNK2TRIES-chunk2tries)); // chunk2 * (numtries+1)
						recordThroughput[myAccId] = false;
						bundle->begin = begin;
						bundle->end = begin+chunk2;
						begin = bundle->end;
						bundle->type = myAccId;
						return bundle;
					   }
					   else
					   {
						   cerr << "We can't calculate NE after " << MAXCHUNK2TRIES << " tries" << endl;
					       exit(-1);
					   }
					}
						
					chunkGPU = ALIGNGPU(calculateChunk(time1, chunk1, pars->initiationInterval, pars->frequency, NE, pars->thobjective));
					stable_fase=1;
					/*
					cerr << " CHK1 " <<  chunkGPU << endl;
					cerr << " CHK2 " << calculateChunk(time2, chunk2, pars->initiationInterval, pars->frequency, NE, pars->thobjective) << endl;
					cerr << "Calculated Chunk " << chunkGPU << endl;
					cerr << "  Time " << time1 << endl;
					cerr << "  II " << pars->initiationInterval << endl;
					cerr << "  Freq " << pars->frequency << endl;
					cerr << "  Elem x Iter " << NE << endl;
					cerr << "  %TH " << pars->thobjective << endl;
		*/
					#ifdef TRACER
					tracer.nodeStop(11, "ProfACC");
					#endif	
				}				
				
				case 4: // Stable stage
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
						bodies_F+=bundle->end-bundle->begin;
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
						bodies_C+=bundle->end-bundle->begin;
						return bundle;
					}
					break;
				
				}
			} else { //CPU WORK
				/*
                   _____ _____  _    _  __          ______  _____  _  __
                  / ____|  __ \| |  | | \ \        / / __ \|  __ \| |/ /
                 | |    | |__) | |  | |  \ \  /\  / / |  | | |__) | ' /
                 | |    |  ___/| |  | |   \ \/  \/ /| |  | |  _  /|  <
                 | |____| |    | |__| |    \  /\  / | |__| | | \ \| . \
                  \_____|_|     \____/      \/  \/   \____/|_|  \_\_|\_\

                */
				  if (anyFPGArunning) {
                    /*Calculating next chunkCPU*/
                    float maxGpuThroughput = gpuThroughput[1];
              /*      cerr << "FPGA TH: " << gpuThroughput[1];
                    for (int i=2; i < nGPUs+1; i++)
                    {
						 cerr << " , " << gpuThroughput[i];
                        maxGpuThroughput = std::max(maxGpuThroughput, gpuThroughput[i]);
					}
					cerr << endl;*/
                 //   PrivFloat_t::reference cpuThroughputr = cpuThroughputETS.local();
                 		

				    calculateAvgCPUTH(cpuThroughputETS[myCPUId]);
                    if(maxGpuThroughput >0 && avgCpuThroughput>0) {
                        fG = maxGpuThroughput/avgCpuThroughput;
                        fGvalid=true;
                    }
                    
                    //cerr << "maxGpuThroughput " << maxGpuThroughput <<  " avgCpuThroughput " << avgCpuThroughput << " fGvalid " << fGvalid << endl;

                    if (fGvalid) {
                        chunkCPU = max( min(chunkGPU/fG , (end - begin) / (fG + nCPUs)),  (float) MINCHUNKCPU);
                        chunkCPU = ALIGNCPU(chunkCPU);
                    }
                } else {
                    chunkCPU = max((end - begin) / (nCPUs), (int) MINCHUNKCPU);
                    chunkCPU = ALIGNCPU(chunkCPU);
                }
                if(chunkCPU==0) chunkCPU=MINCHUNKCPU;  // aqui si se establece minimo para CPU
#ifdef OTRO
                cerr << "Serial Filter CPU: " << chunkCPU << " Begin = " << begin << ", End = " << end << " fg " << fG << " chunkGPU " << chunkGPU << endl;
#endif
                //cout << "fg " << fG << " chunkCPU " << chunkCPU << endl;
                int auxEnd = begin + chunkCPU;
                auxEnd = (auxEnd > end) ? end : auxEnd;
                //   auxEnd = (end-auxEnd < MINIMUN_REMAIN) ? end : auxEnd; // arrastrar el último resto
                bundle->begin = begin;
                bundle->end = auxEnd;
                begin = auxEnd;
                bundle->type = CPU;
#ifdef DEBUG_CHUNKS
                cerr << "BUNDLE CPU: TYPE["<<bundle->type<<"] Begin,End:Size = "<< bundle->begin<< ", " << bundle->end << " : " << bundle->end -bundle->begin << endl;

#endif
				bodies_C+=bundle->end-bundle->begin;
                return bundle;
			}
		}
		return NULL;
	} // end operator

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
/*
╔═╗╔═╗╔═╗╔═╗  ┌─┐┌─┐┌─┐┬─┐┌─┐┌┬┐┌─┐┬─┐
╠╣ ╠═╝║ ╦╠═╣  │ │├─┘├┤ ├┬┘├─┤ │ │ │├┬┘
╚  ╩  ╚═╝╩ ╩  └─┘┴  └─┘┴└─┴ ┴ ┴ └─┘┴└─			
*/
			 tick_count start_tc = tick_count::now();

        body->OperatorGPU(bundle->begin, bundle->end, bundle->type);


        end_tc = tick_count::now();

        float time = (end_tc - start_tc).seconds()*1000;
        
      switch (AccStage[myAccId]) // AccStage is updated here. 
		{
			case 0:
				AccStage[myAccId]=1;
				break;
				
			case 1:
				time1=time/1000;
				first_time= time;  // ANDRES was  "= auxtime"
				first_chunk = bundle->end - bundle->begin;
				AccStage[myAccId]=2;
				break;
			
			case 2:
				time2=time/1000;
				AccStage[myAccId]=3;
				break;
				
		}
    //  cerr << "FPGA "<< bundle->type << "record: " << recordThroughput[bundle->type] << " TH: " << (bundle->end - bundle->begin) / time << endl;
     if (recordThroughput[bundle->type]) gpuThroughput[bundle->type] = (bundle->end - bundle->begin) / time ;
			#ifdef DEBUG
			printf("FPGA SIZE:%d  TH: %f time: %f\n", bundle->end - bundle->begin, (bundle->end - bundle->begin) / time, time);
			#endif

#ifdef OUTPUT_FREQ_HISTOGRAM
        float th = (bundle->end - bundle->begin) / ((end - start).seconds() * 1000);


        for (int i = bundle->begin; i < bundle->end; i++) {
            histogram << bundle->end - bundle->begin << "\t" << th << endl;
        }
#endif
        //Summarizing GPU Statistics
        totalIterationsGPU = totalIterationsGPU + (bundle->end - bundle->begin);
        itemsOnGPU = itemsOnGPU + 1;
/* from multihap
        //It doesn't update gpuThorughput in the last scheduling interval
        if (!stopConditionModeOn || (stopConditionModeOn && (bundle->begin == 0))) {

            gpuThroughput[bundle->type] = (bundle->end - bundle->begin) / time ;
            lastchunkGPU=bundle->end - bundle->begin;
            //gpuThroughput = (bundle->end - bundle->begin) / time;

            //corregir
            //if (cpuThroughput > 0) {
            //	fG = gpuThroughput / cpuThroughput;
            //	fGvalid = true;
            //	}
        }
*/
#if defined(DEBUG_CHUNKS_F) // || defined(DEBUG)
        cerr  << "=== Chunk FPGA["<< bundle->type << "]: " << bundle->end - bundle->begin << " BEGIN: " << bundle->begin << " TH: " << gpuThroughput[bundle->type]  /*((tg5-tg1)/1000000.0) */  << " it: " << bundle->begin << endl;
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
			//PrivFloat_t::reference cpuThroughputr = cpuThroughputETS.local();
					

			cpuThroughputETS[myCPUId] = (bundle->end - bundle->begin) / time;
			chunkCPU = (bundle->end - bundle->begin);

		// from multihap
      //  cpuThroughput = (bundle->end - bundle->begin) / ((end - start).seconds()*1000);
      //  lastcpuThroughput=cpuThroughput;

#if defined(DEBUG_CHUNKS_C) ///|| defined(DEBUG)

        cerr << "=== Chunk CPU: " << bundle->end - bundle->begin << " BEGIN: " << bundle->begin << " TH: " << cpuThroughputETS[myCPUId] /*((tg5-tg1)/1000000.0) */ << endl;
#endif


		
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

//end class

/*FastFit Class: This scheduler version let us to split the workload in two subranges, one for GPU and one for CPUs*/
class FastFit : public Scheduler<FastFit, Params> {

Params * pars;

public:
	/*This constructor just call his parent's contructor*/
	FastFit(Params *params) : Scheduler<FastFit, Params>(params),
	pars(params)
	{
		chunkCPU = 0;
		fG = 0.0;
		chunkGPU = 0.0;
		//PrivFloat_t::reference cpuThroughputr = cpuThroughputETS.local();
	//	cpuThroughputr=0.0;
		avgCpuThroughput = 0.0;
		for (int i=0; i<= pars->numgpus; i++) {  // th de todos los ACC = 0
			gpuThroughput[i] = 0;
			recordThroughput[i] = false;
			//if(pars->WARMUPFPGA) AccStage[i]=0;  else AccStage[i]=1;  // ANDRES init stage with/wout WARMUP
			AccStage[i]=22;
		}
		if(pars->WARMUPFPGA) AccStage[pars->numgpus]=0;  else AccStage[pars->numgpus]=1;  // ANDRES init stage with/wout WARMUP
			
		nAcc = pars->numgpus;       // numero inicial de ACC
		nCpu = pars->numcpus;
		for (int i=0; i<pars->numcpus;  i++)  {
			cpuThroughputETS[i]=0.0f;
			}
		//myAccId = 0;                // no acc asociado al thread
		CHUNKGPUALIGN=pars->CHUNKGPUALIGN;
		CHUNKCPUALIGN=pars->CHUNKCPUALIGN;
		stable_fase=0;
	}

/*
██████╗    ███████╗ ██████╗ ██████╗ 
██╔══██╗   ██╔════╝██╔═══██╗██╔══██╗
██████╔╝   █████╗  ██║   ██║██████╔╝
██╔═══╝    ██╔══╝  ██║   ██║██╔══██╗
██║        ██║     ╚██████╔╝██║  ██║
╚═╝        ╚═╝      ╚═════╝ ╚═╝  ╚═╝
*/
	/*The main function to be implemented*/
	template<class T>
	void heterogeneous_parallel_for(int begin, int end, T* body, const size_t iters = 1){

		#ifndef NDEBUG
		cerr << "Heterogeneous Parallel For Dynamic " << nCPUs << " , " << nGPUs << ", " << chunkGPU << endl;
		#endif

		if (pars->thobjective < 0.5f || pars->thobjective > 1.0f)
			pars->thobjective = 0.90f;
		
		// Profiling if chunkGPU == 0
		body->firsttime = true;  // is this used at all?
		for (int i=0; i<= pars->numgpus; i++) {  // th de todos los ACC = 0
			if(AccStage[i]<0) AccStage[i]=-AccStage[i];  // ANDRES reactivate stopped Accs
			//if(pars->WARMUPFPGA) AccStage[i]=0;  else AccStage[i]=1;  // ANDRES init stage with/wout WARMUP
			}
		anyFPGArunning= pars->numgpus; //ANDRES active Accs at the begining of pfor
		
		global_chunk1 = 1; //ALIGNGPU((end-begin)/200);//(end-begin)/200;
		global_chunk2 = ALIGNGPU((end-begin)/20); //5%
		global_chunk22 = ALIGNGPU((end-begin)/20); //5%
		
#ifdef DEBUG
		cout << " ***** NEW Time Step ****************************" << endl;
#endif
		/*Preparing pipeline*/
		pipeline pipe;
		MySerialFilter serial_filter(begin, end, nCPUs, nGPUs, pars);
		MyParallelFilter<T> parallel_filter(body);
		pipe.add_filter(serial_filter);
		pipe.add_filter(parallel_filter);
		//chunkCPU = chunkGPU * 0.2;
		if(fG==0.0) fG = 8; // 25 para Nbody?
		chunkCPU = INITCHUNKCPU; //min((int)(chunkGPU / fG), (int)((end-begin)/100));
		if(!chunkCPU) chunkCPU=1;
		

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
		/*Run the pipeline*/
		for (size_t i = 0; i < iters; ++i){
			pipe.run(nCPUs + nGPUs);
			serial_filter.reset_iter_space(begin, end);

		}
		//pipe.run(nCPUs + nGPUs);
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
