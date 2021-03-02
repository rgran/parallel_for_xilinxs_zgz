#!/bin/bash

# Author: Kris Nikov - kris.nikov@bris.ac.uk
# Date: 23 Jul 2019 
# Description: MSSLEEP script for data collection and output in readable format for plotting scripts
 

if [[ "$#" -eq 0 ]]; then
  echo "This program requires inputs. Type -h for help." >&2
  exit 1
fi

#Requires getops, but this should not be an issue since ints built in bash
while getopts ":n:m:c:p:l:k:d:s:it:h" opt;
do
    case $opt in
          h)
               echo "Available flags and options (everything but the -n, -m and -s flags is purely cosmetic for output consistency between MSSLEEP and the other benchmarks):" >&2
               echo "-n [INT] -> Select number of executions."
               echo "-m [INT] -> Sleep time in ms."
               echo "-c [0-4] -> [COSMETIC] Select the number of CPU cores."
               echo "-p [0-4] -> [COSMETIC] Select the number of HP port connected accelerators. These work with non-cacheable memory using special memory buffers."
               echo "-l [0-4] -> [COSMETIC] Select the number of HPC port connected accelerators. These work with cacheable memory."  
               echo "-k [INT] -> [COSMETIC] Select the compute chunk size."
               echo "-i -> [COSMETIC] Enable ioctl calls to the interrupt handler driver. This improves accelerator performance by reducing spinlock of the issuing thread. Default is disabled."
               echo "-s [FILE] -> Select the save file for the results. If no file selected, results will be displayed in terminal."
               echo "Mandatory options are: -n [INT] -m [FILE] -c [0-4] -p [0-4] -l [0-2] -k [INT]"
               exit 0 
               ;;
 		#Select the number of runs               
		n)
			if [[ -n $runs ]]; then
				echo "Invalid input: option -n has already been used!" >&2
				exit 1
			fi
			runs="$OPTARG"
               ;;
 		#Select the sleeptime               
		m)
			if [[ -n $sleeptime ]]; then
				echo "Invalid input: option -m has already been used!" >&2
				exit 1
			fi
			sleeptime="$OPTARG"
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
               ;;
 		#Select the number of HPC port accelerators               
		l)
			if [[ -n $hpcacc ]]; then
				echo "Invalid input: option -l has already been used!" >&2
				exit 1
			fi
			hpcacc="$OPTARG"
               ;;               
 		#Select the chunk size               
		k)
			if [[ -n $chunk ]]; then
				echo "Invalid input: option -k has already been used!" >&2
				exit 1
			fi
			chunk="$OPTARG"
               ;;             
          i)
               if [[ -n $ioctl ]]; then
				echo "Invalid input: option -i has already been used!" >&2
				exit 1
			fi
			ioctl=1
               ioctl_en="Y"
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

#Make sure sleeptime has been specified
if [[ -z $sleeptime ]]; then
    	echo "Nothing to run. Expected -m flag!" >&2
    	exit 1
else
     #Make sure the specified runs number is a positive integer greater than 50ms
     if ! [[ $sleeptime =~ ^[0-9]+$ && $sleeptime -ge 50 ]]; then
          echo "Invalid input: specified value for sleeptime -m $sleeptime needs to be a positive integer number greater than the sensor sampling rate (50ms)!" >&2
          exit 1
     fi
fi

#Make sure we have specified a correct number of cpu cores
if [[ -z $cpuc ]]; then
    	echo "Nothing to run. Expected -c flag!" >&2
    	exit 1
else
     #Make sure the specified runs number is a positive integer
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
     #Make sure the specified runs number is a positive integer
     if ! [[ $chunk =~ ^[0-9]+$ ]]; then
          echo "Invalid input: specified value for compute chunk size needs to be an integer number!" >&2
          exit 1
     fi
fi

#Make sure we have specified a correct number compute chunk size
if [[ -z $ioctl ]]; then
    	ioctl=0
     ioctl_en="N"
     echo "Using default value for ioctl -> $ioctl (no ioctl calls)!"     
fi

echo "Begin runs!" >&1
#Run the workload and collect the output
for ((n=0;n<$runs;n++))
do
     mssleep_output+=$(./MSSLEEP $sleeptime 2> /dev/null)
done

#Extract relevant informaton from mssleep output. These will depend on output mode.
#Rows on CPU
IFS=";" read -a cpu_rows <<< $(echo -e "$mssleep_output" | awk -v SEP=' ' 'BEGIN{FS=SEP}{if ($1=="Total" && $2=="n." && $5=="CPU:" ){ print $6 }}' | tr "\n" ";" | head -c -1)
#Rows on FPGA
IFS=";" read -a fpga_rows <<< $(echo -e "$mssleep_output" | awk -v SEP=' ' 'BEGIN{FS=SEP}{if ($1=="Total" && $2=="n." && $5=="FPGA:" ){ print $6 }}' | tr "\n" ";" | head -c -1)
#CPU Power
IFS=";" read -a cpu_power <<< $(echo -e "$mssleep_output" | awk -v SEP=' ' 'BEGIN{FS=SEP}{if ($1=="Processing" && $2=="System" && $4=="POWER" ){ print $6 }}' | tr "\n" ";" | head -c -1)
#FPGA Power    
IFS=";" read -a fpga_power <<< $(echo -e "$mssleep_output" | awk -v SEP=' ' 'BEGIN{FS=SEP}{if ($1=="Programmable" && $2=="Logic" && $4=="POWER" ){ print $6 }}' | tr "\n" ";" | head -c -1)
#CPU Energy
IFS=";" read -a cpu_energy <<< $(echo -e "$mssleep_output" | awk -v SEP=' ' 'BEGIN{FS=SEP}{if ($1=="Processing" && $2=="System" && $4=="ENERGY" ){ print $6 }}' | tr "\n" ";" | head -c -1)
#FPGA Energy
IFS=";" read -a fpga_energy <<< $(echo -e "$mssleep_output" | awk -v SEP=' ' 'BEGIN{FS=SEP}{if ($1=="Programmable" && $2=="Logic" && $4=="ENERGY" ){ print $6 }}' | tr "\n" ";" | head -c -1)

#Output
header="Run[#]\tCPU Cores[#]\tFPGA HP Accelerators[#]\tFPGA HPC Accelerators[#]\tChunk Size[#]\tIOCTL Enabled\tExecution Time[ms]\tRows on CPU[#]\tRows on FPGA[#]\tScheduler Type\tCPU Power[W]\tFPGA Power[W]\tCPU Energy[J]\tFPGA Energy[J]"
data="\$((\$i+1))\t$cpuc\t$hpacc\t$hpcacc\t\$chunk\t$ioctl_en\t\$sleeptime\t\${cpu_rows[\$i]}\t\${fpga_rows[\$i]}\tMultiDynamic\t\${cpu_power[\$i]}\t\${fpga_power[\$i]}\t\${cpu_energy[\$i]}\t\${fpga_energy[\$i]}"

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

echo -e "====================" >&1
echo "Script Done!" >&1