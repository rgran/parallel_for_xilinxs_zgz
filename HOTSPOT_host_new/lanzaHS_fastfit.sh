#!/bin/bash
#---------------------------

bench=measure_sleep_power
#bench=HOTSPOT_FF

echo "blocking test -----------------------------------------"
echo `date`

prog=./${bench}

for nc in 0 1 2 3 4; do
  for nf in 0 1 2 3 4; do
    if [ ${nc} -ne 0 ] || [ ${nf} -ne 0 ] ; then     
    	for i in 1 2 3 4 5; do
	    	echo "-------------------------------------"
	    	echo ${prog} nc: ${nc} nf: ${nf} ratio: 0.0 rep: ${i}
 	    	echo "-------------------------------------"

		${prog} 1024 inputs/temp_1024.data  inputs/power_1024.data 1 ${nc} 0 ${nf} 0.95 1 256 1200 200

	    	if [ -f ~/break ] ; then
			exit
	    	fi

		done	
    fi
  done
done 

for nc in 0 1 2 3 4; do
  for nf in 0 1 2 3 4; do
    if [ ${nc} -ne 0 ] || [ ${nf} -ne 0 ] ; then     
    	for i in 1 2 3 4 5; do
	    	echo "-------------------------------------"
	    	echo ${prog} nc: ${nc} nf: ${nf} ratio: 0.0 rep: ${i}
 	    	echo "-------------------------------------"

		${prog} 1024 inputs/temp_1024.data  inputs/power_1024.data 1 ${nc} 0 ${nf} 0.95 1 256 1200 25

	    	if [ -f ~/break ] ; then
			exit
	    	fi

		done	
    fi
  done
done 
   
exit
 
    for ratio in 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1; do
    	for i in 1 2 3 4 5; do
	    	echo "-------------------------------------"
	    	echo ${prog} ratio: ${ratio} rep: ${i}
 	    	echo "-------------------------------------"

		${prog} 1024 inputs/temp_1024.data  inputs/power_1024.data 1 4 0 4 ${ratio} 0 256 1200 200

	    	if [ -f ~/break ] ; then
			exit
	    	fi

		done	
    done 

for ratio in 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9  1; do
    	for i in 1 2 3 4 5; do
	    	echo "-------------------------------------"
	    	echo ${prog} ratio: ${ratio} rep: ${i}
 	    	echo "-------------------------------------"

		${prog} 1024 inputs/temp_1024.data  inputs/power_1024.data 1 4 0 4 ${ratio} 1 256 1200 25

	    	if [ -f ~/break ] ; then
			exit
	    	fi

		done	
    done 
    
    for ratio in 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1; do
    	for i in 1 2 3 4 5; do
	    	echo "-------------------------------------"
	    	echo ${prog} ratio: ${ratio} rep: ${i}
 	    	echo "-------------------------------------"

		${prog} 1024 inputs/temp_1024.data  inputs/power_1024.data 1 4 0 4 ${ratio} 0 256 1200 25

	    	if [ -f ~/break ] ; then
			exit
	    	fi

		done	
    done 

#~ mkdir ${prog}_dir
#~ mv *.txt ${prog}_dir
