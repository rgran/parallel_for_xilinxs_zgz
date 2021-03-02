#!/bin/bash
#HOTSPOT control script for easier data collection
#KrNikov 2019

if [[ "$#" -eq 0 ]]; then
  echo "This program requires inputs. Type -h for help." >&2
  exit 1
fi

#Requires getops, but this should not be an issue since ints built in bash
while getopts "hn:r:f:a:b:ec:p:l:k:d:s:it:u:v:" opt;
do
    case $opt in
          h)
               echo "Available flags and options:"
               echo "-n [INT] -> Select number of executions."
               echo "-f [INT] -> Select square buffer dimention (up to and including 8192)." # before the maximum value was 6500
               echo "-a [FILE] -> Select the temperature input file."
               echo "-b [FILE] -> Select the power input file."
               echo "-e -> Enable continuous read. Default is OFF."
               echo "-c [0-4] -> Select the number of CPU cores."
               echo "-p [0-4] -> Select the number of HP port connected accelerators. These work with non-cacheable memory using special memory buffers."
               echo "-l [0-4] -> Select the number of HPC port connected accelerators. These work with cacheable memory."  
               echo "-k [INT] -> Select the compute chunk size."
               echo "-d [1-2] -> Select the scheduler type. 1 - MultiDynamic; 2 - Multihap;"
               echo "-i -> Enable ioctl calls to the interrupt handler driver. This improves accelerator performance by reducing spinlock of the issuing thread. Default is disabled."
               echo "-s [FILE] -> Select the save file for the results. If no file selected, results will be displayed in terminal."
               echo "-t [FILE] -> Select the save file for the output."
               echo "-r [INT] -> Number of repetitions of the parallel for."
	       echo "-u [100|200|300|400|600|1200] -> frequency for the cpu."
	       echo "-v [25|40|50|100|150|200] -> frequency for the fpga."
	       echo "Mandatory options are: -n [INT] -a [FILE] -b [FILE] -c [0-4] -p [0-4] -l [0-2] -k [INT] -r [INT] -u [ranged_INT] -v [ranged_INT]"
               exit 0 
               ;;
 		#Select the number of runs               
		n)
			if [[ -n $runs ]]; then
				echo "Invalid input: option -n has already been used!" >&2
				exit 1
			fi
			runs="$OPTARG"
			echo $runs
               ;;
 		#Select the buffer size               
		f)
			if [[ -n $bufs ]]; then
				echo "Invalid input: option -f has already been used!" >&2
				exit 1
			fi
			bufs="$OPTARG"
               ;;               
 		#Select the temperatures               
		a)
			if [[ -n $temps ]]; then
				echo "Invalid input: option -a has already been used!" >&2
				exit 1
			fi
			temps="$OPTARG"
               ;;
          #Select the powers               
          b)
			if [[ -n $powers ]]; then
				echo "Invalid input: option -b has already been used!" >&2
				exit 1
			fi
			powers="$OPTARG"
               ;;
          e)
               if [[ -n $contin ]]; then
				echo "Invalid input: option -e has already been used!" >&2
				exit 1
			fi
			contin=1
               ;;                
 		#Select the number of cpu cores               
		c)
			if [[ -n $cpuc ]]; then
				echo "Invalid input: option -c has already been used!" >&2
				exit 1
			fi
			cpuc="$OPTARG"
               ;;
 		#Select the number of HP port accelerators               
		p)
			if [[ -n $hpacc ]]; then
				echo "Invalid input: option -p has already been used!" >&2
				exit 1
			fi
			hpacc="$OPTARG"
			binary=HOTSPOT_hp
               ;;
 		#Select the number of HPC port accelerators               
		l)
			if [[ -n $hpcacc ]]; then
				echo "Invalid input: option -l has already been used!" >&2
				exit 1
			fi
			hpcacc="$OPTARG"
			binary=HOTSPOT_hpc
               ;;               
 		#Select the chunk size               
		k)
			if [[ -n $chunk ]]; then
				echo "Invalid input: option -k has already been used!" >&2
				exit 1
			fi
			chunk="$OPTARG"
               ;;
 		#Select the sched type              
		d)
			if [[ -n $sched ]]; then
				echo "Invalid input: option -d has already been used!" >&2
				exit 1
			fi
			sched="$OPTARG"
               ;;               
          i)
               if [[ -n $ioctl ]]; then
				echo "Invalid input: option -i has already been used!" >&2
				exit 1
			fi
			ioctl=1
               ioctl_en="Y"
               ;;               
	r)
		if [[ -n $repts ]]; then
			echo "Invalid input: option -r has already been used!" >&2
			exit 1
		fi
		repts="$OPTARG"

#		echo $repts
              ;;
 	u)
		if [[ -n $freq_cpu ]]; then
			echo "Invalid input: option -fc has already been used!" >&2
			exit 1
		fi
		freq_cpu="$OPTARG"

#echo $freq_cpu
              ;;
 	v)
		if [[ -n $freq_fpga ]]; then
			echo "Invalid input: option -ff has already been used!" >&2
			exit 1
		fi
		freq_fpga="$OPTARG"

#		echo $freq_fpga
              ;;
 #Specify the save file, if no save file is chosen the results are printed on terminal
		s)
			if [[ -n $save_file ]]; then
			    	echo "Invalid input: option -s has already been used!" >&2
			    	exit 1                
			fi
			if [[ -e "$OPTARG" ]]; then
			    	#wait on user input here (Y/N)
			    	#if user says Y set writing directory to that
			    	#if no then exit and ask for better input parameters
			    	echo "-s $OPTARG already exists. Continue writing in file? (Y/N)" >&1
			    	while true;
			    	do
					read USER_INPUT
					if [[ "$USER_INPUT" == Y || "$USER_INPUT" == y ]]; then
				    		echo "Using existing file $OPTARG" >&1
				    		break
					elif [[ "$USER_INPUT" == N || "$USER_INPUT" == n ]]; then
				    		echo "Cancelled using save file $OPTARG Program exiting." >&1
				    		exit 0                            
					else
				    		echo "Invalid input: $USER_INPUT !(Expected Y/N)" >&2
						echo "Please enter correct input: " >&2
					fi
			    	done
			    	save_file="$OPTARG"
			else
		    		#file does not exist, set mkdir flag.
		    		save_file="$OPTARG"
			fi
               ;;         
 		#Specify the save file, if no save file is chosen the results are printed on terminal
		t)
			if [[ -n $out_file ]]; then
			    	echo "Invalid input: option -t has already been used!" >&2
			    	exit 1                
			fi
			if [[ -e "$OPTARG" ]]; then
			    	#wait on user input here (Y/N)
			    	#if user says Y set writing directory to that
			    	#if no then exit and ask for better input parameters
			    	echo "-t $OPTARG already exists. Continue writing in file? (Y/N)" >&1
			    	while true;
			    	do
					read USER_INPUT
					if [[ "$USER_INPUT" == Y || "$USER_INPUT" == y ]]; then
				    		echo "Using existing file $OPTARG" >&1
				    		break
					elif [[ "$USER_INPUT" == N || "$USER_INPUT" == n ]]; then
				    		echo "Cancelled using save file $OPTARG Program exiting." >&1
				    		exit 0                            
					else
				    		echo "Invalid input: $USER_INPUT !(Expected Y/N)" >&2
						echo "Please enter correct input: " >&2
					fi
			    	done
			    	out_file="$OPTARG"
			else
		    		#file does not exist, set mkdir flag.
		    		out_file="$OPTARG"
			fi
               ;;                  
          :)
               echo "Option: -$OPTARG requires an argument" >&2
               exit 1
               ;;
          \?)
               echo "Invalid option: -$OPTARG" >&2
               exit 1
               ;;
    esac
done

#Critical checks

#Make sure we have specified a correct number of runs
if [[ -z $runs ]]; then
    	echo "Nothing to run. Expected -n flag!" >&2
    	exit 1
else
     #Make sure the specified runs number is a positive integer
     if ! [[ $runs =~ ^[0-9]+$ && $runs -ge 1 ]]; then
          echo "Invalid input: specified value for number of runs -n $runs needs to be a positive integer number!" >&2
          exit 1
     fi
fi

#Make sure we have specified the correct buffer size
if [[ -z $bufs ]]; then
    	echo "Nothing to run. Expected -f flag!" >&2
    	exit 1
else
     #Make sure the specified buffer size number is a positive integer
     if ! [[ $bufs =~ ^[0-9]+$ && $bufs -ge 1 && $bufs -le 8192 ]]; then
          echo "Invalid input: specified value for number of buffer size -f $bufs needs to be a positive integer number between 1 and 8192!" >&2
          exit 1
     fi
fi

#Make sure temperatures file has been specified
if [[ -z $temps ]]; then
    	echo "Nothing to run. Expected -a flag!" >&2
    	exit 1
else
     #Make sure the selected temperatures file exists
     if ! [[ -e "$temps" ]]; then
          echo "-a $temps does not exist. Please enter a valid temperatures file!" >&2
          exit 1
     fi
fi

#Make sure powers file has been specified
if [[ -z $powers ]]; then
    	echo "Nothing to run. Expected -b flag!" >&2
    	exit 1
else
     #Make sure the selected powers file exists
     if ! [[ -e "$powers" ]]; then
          echo "-b $powers does not exist. Please enter a valid powers file!" >&2
          exit 1
     fi
fi

#Check if continuous read is enabled. Default is off.
if [[ -z $contin ]]; then
    	contin=0
     echo "Using default value for continuous reading -> $contin (Read files just once. If buffer is bigger, program will exit.)!"     
fi

#Make sure we have specified a correct number of cpu cores
if [[ -z $cpuc ]]; then
    	echo "Nothing to run. Expected -c flag!" >&2
    	exit 1
else
     #Make sure the specified cpu number is a positive integer
     if ! [[ $cpuc =~ ^[0-9]+$ && $cpuc -ge 0 && $cpuc -le 4 ]]; then
          echo "Invalid input: specified value for number of cpu cores -c $cpuc needs to be an integer number in the range [0-4]!" >&2
          exit 1
     fi
fi

#Make sure we have specified a correct number of HP or HPC accelerators
if [[ -z $hpacc && -z $hpcacc ]]; then
    	echo "Nothing to run. Expected -p or -l flag!" >&2
    	exit 1
else
     if [[ -n $hpacc && -z $hpcacc ]]; then 
          #Make sure the specified HP accelerator number is a positive integer within range
          if ! [[ $hpacc =~ ^[0-9]+$ && $hpacc -ge 0 && $hpacc -le 4 ]]; then
               echo "Invalid input: specified value for number of HP port accelerators -p $hpacc needs to be an integer number in the range [0-4]!" >&2
               exit 1
          fi
          hpcacc=0
     elif [[ -z $hpacc && -n $hpcacc ]]; then
          #Make sure the specified HP accelerator number is a positive integer within range  
          if ! [[ $hpcacc =~ ^[0-9]+$ && $hpcacc -ge 0 && $hpcacc -le 4 ]]; then
               echo "Invalid input: specified value for number of HPC port accelerators -l $hpcacc needs to be an integer number in the range [0-6]!" >&2
               exit 1
          fi
          hpacc=0
     else
          echo "Invalid input: please use either -p flag for HP accelerators or -l flag for HPC accelerators!" >&2
          exit 1
     fi
fi

#Make sure we have specified a correct number compute chunk size
if [[ -z $chunk ]]; then
    	echo "Nothing to run. Expected -k flag!" >&2
    	exit 1
else
     #Make sure the specified chunk number is a positive integer
     if ! [[ $chunk =~ ^[0-9]+$ ]]; then
          echo "Invalid input: specified value for compute chunk size needs to be an integer number!" >&2
          exit 1
     fi
fi

#Make sure we have specified a scheduler type
if [[ -z $sched ]]; then
    	echo "Nothing to run. Expected -d flag!" >&2
    	exit 1
else
     #Make sure the specified scheduler type number is a positive integer
     if ! [[ $sched =~ ^[0-9]+$ && $sched -ge 1 && $sched -le 2 ]]; then
          echo "Invalid input: specified value for scheduler type needs to be either 1 or 2 for MultiDynamic or MultiHap!" >&2
          exit 1
     elif [[ $sched == 1 ]]; then
          scheduler="MultiDynamic"     
          if ! [[ $chunk -ge 1 ]]; then
               echo "Invalid input: specified value for compute chunk size needs to be a positive integer number when used with MultiDynamic schedueler!" >&2
               exit 1
          fi
     else
          scheduler="MultiHap" 
          if ! [[ $chunk -eq 0 ]]; then
               echo "Invalid input: specified value for compute chunk size needs to be 0 for MultiHap scheduler!" >&2
               exit 1
          fi
     fi
fi




#Make sure we have specified a correct number compute chunk size
if [[ -z $ioctl ]]; then
    	ioctl=0
     ioctl_en="N"
     echo "Using default value for ioctl -> $ioctl (no ioctl calls)!"     
fi

#echo ${freq_cpu}
#echo ${freq_fpga}


echo "Begin runs!" >&1
#Run the workload and collect the output
for ((n=0;n<$runs;n++))
do
     if [[ -z $out_file ]]; then
#hotspot_output+=$(./$binary $bufs $temps $powers $contin $cpuc $hpacc $hpcacc $chunk $ioctl 2> /dev/null)
          echo ./$binary $bufs $temps $powers $contin $cpuc $hpacc $hpcacc $chunk $ioctl $repts $freq_cpu $freq_fpga
hotspot_output+=$(./$binary $bufs $temps $powers $contin $cpuc $hpacc $hpcacc $chunk $ioctl $repts $freq_cpu $freq_fpga)
	#//echo $hotspot_output
#echo "./$binary $bufs $temps $powers $contin $cpuc $hpacc $hpcacc $chunk $ioctl 2> /dev/null"
          #exit
     else
#    hotspot_output+=$(./$binary $bufs $temps $powers $contin $cpuc $hpacc $hpcacc $chunk $ioctl $out_file 2> /dev/null)
          echo ./$binary $bufs $temps $powers $contin $cpuc $hpacc $hpcacc $chunk $ioctl $repts $freq_cpu $freq_fpga $out_file
hotspot_output+=$(./$binary $bufs $temps $powers $contin $cpuc $hpacc $hpcacc $chunk $ioctl $repts $freq_cpu $freq_fpga $out_file)
	#//echo $hotspot_output
   	fi
done

#Extract relevant informaton from hotspot output. These will depend on output mode.
#Exec. Time
IFS=";" read -a exec_time <<< $(echo -e "$hotspot_output" | awk -v SEP=' ' 'BEGIN{FS=SEP}{if ($1=="CLOCK_REALTIME"){ print $3 * 1000 }}' | tr "\n" ";" | head -c -1)
#Workload on CPU
IFS=";" read -a cpu_work <<< $(echo -e "$hotspot_output" | awk -v SEP=' ' 'BEGIN{FS=SEP}{if ($1=="Total" && $2=="workload" && $4=="CPU:" ){ print $5 }}' | tr "\n" ";" | head -c -1)
#Workload on FPGA
IFS=";" read -a fpga_work <<< $(echo -e "$hotspot_output" | awk -v SEP=' ' 'BEGIN{FS=SEP}{if ($1=="Total" && $2=="workload" && $4=="FPGA:" ){ print $5 }}' | tr "\n" ";" | head -c -1)
#CPU Power
IFS=";" read -a cpu_power <<< $(echo -e "$hotspot_output" | awk -v SEP=' ' 'BEGIN{FS=SEP}{if ($1=="Processing" && $2=="System" && $4=="POWER" ){ print $6 }}' | tr "\n" ";" | head -c -1)
#FPGA Power    
IFS=";" read -a fpga_power <<< $(echo -e "$hotspot_output" | awk -v SEP=' ' 'BEGIN{FS=SEP}{if ($1=="Programmable" && $2=="Logic" && $4=="POWER" ){ print $6 }}' | tr "\n" ";" | head -c -1)
#CPU Energy
IFS=";" read -a cpu_energy <<< $(echo -e "$hotspot_output" | awk -v SEP=' ' 'BEGIN{FS=SEP}{if ($1=="Processing" && $2=="System" && $4=="ENERGY" ){ print $6 }}' | tr "\n" ";" | head -c -1)
#FPGA Energy
IFS=";" read -a fpga_energy <<< $(echo -e "$hotspot_output" | awk -v SEP=' ' 'BEGIN{FS=SEP}{if ($1=="Programmable" && $2=="Logic" && $4=="ENERGY" ){ print $6 }}' | tr "\n" ";" | head -c -1)
#Chunk size for MultiHap scheduler
[[ $sched == 2 ]] && IFS=";" read -a chunk_arr <<< $(echo -e "$hotspot_output" | awk -v SEP=' ' 'BEGIN{FS=SEP}{if ($1=="Final" && $2=="FPGA" && $3=="Chunk:" ){ print $4 }}' | tr "\n" ";" | head -c -1)

hotspot_output=""

#Output
header="Run[#]\tCPU Cores[#]\tFPGA HP Accelerators[#]\tFPGA HPC Accelerators[#]\tCPU Freq[#]\tFPGA Freq[#]\tChunk Size[#]\tIOCTL Enabled\tExecution Time[ms]\tWorkload on CPU[#]\tWorkload on FPGA[#]\tScheduler Type\tCPU Power[W]\tFPGA Power[W]\tCPU Energy[J]\tFPGA Energy[J]"
[[ $sched == 1 ]] && data="\$((\$i+1))\t$cpuc\t$hpacc\t$hpcacc\t$freq_cpu\t$freq_fpga\t\$chunk\t$ioctl_en\t\${exec_time[\$i]}\t\${cpu_work[\$i]}\t\${fpga_work[\$i]}\t$scheduler\t\${cpu_power[\$i]}\t\${fpga_power[\$i]}\t\${cpu_energy[\$i]}\t\${fpga_energy[\$i]}"
[[ $sched == 2 ]] && data="\$((\$i+1))\t$cpuc\t$hpacc\t$hpcacc\t$freq_cpu\t$freq_fpga\t\${chunk_arr[\$i]}\t$ioctl_en\t\${exec_time[\$i]}\t\${cpu_work[\$i]}\t\${fpga_work[\$i]}\t$scheduler\t\${cpu_power[\$i]}\t\${fpga_power[\$i]}\t\${cpu_energy[\$i]}\t\${fpga_energy[\$i]}"

#Output to file or terminal. First header, then data depending on model
#If per-frequency models, iterate frequencies then print
#If full frequency just print the one model
if [[ -z $save_file ]]; then
	echo -e "====================" >&1
	echo -e "$header"
	echo -e "--------------------" >&1
else
	echo -e "$header" > "$save_file"
fi
for ((n=0;i<$runs;i++))
do
	if [[ -z $save_file ]]; then 
		echo -e "$(eval echo "$(echo -e "$data")")" | tr " " "\t"
	else
		echo -e "$(eval echo "$(echo -e "$data")")" | tr " " "\t" >> "$save_file"
	fi
done

#Cleanup
rm *.txt
rm sds_trace_data.dat

echo -e "====================" >&1
echo "Script Done!" >&1
