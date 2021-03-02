#!/bin/bash
#---------------------------

bench=HOTSPOT_ORACLE

echo "blocking test -----------------------------------------"
echo `date`

prog=./${bench}

for nc in 4; do
  for nf in 4; do
    for ratio in 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1; do
    	for i in 1 2 3 4 5; do
	    	echo "-------------------------------------"
	    	echo ${prog} ratio: ${ratio} rep: ${i}
 	    	echo "-------------------------------------"

		${prog} 1024 inputs/temp_1024.data  inputs/power_1024.data 1 ${nc} 0 ${nf} ${ratio} 1 256 1200 200 

	    	if [ -f ~/break ] ; then
			exit
	    	fi

		done	
    done
  done
done 

for nc in 3; do
  for nf in 4; do
    for ratio in 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1; do
    	for i in 1 2 3 4 5; do
	    	echo "-------------------------------------"
	    	echo ${prog} ratio: ${ratio} rep: ${i}
 	    	echo "-------------------------------------"

		${prog} 1024 inputs/temp_1024.data  inputs/power_1024.data 1 ${nc} 0 ${nf} ${ratio} 1 256 1200 200 

	    	if [ -f ~/break ] ; then
			exit
	    	fi

		done	
    done
  done
done 
 
for nc in 4; do
  for nf in 4; do
    for ratio in 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1; do
    	for i in 1 2 3 4 5; do
	    	echo "-------------------------------------"
	    	echo ${prog} ratio: ${ratio} rep: ${i}
 	    	echo "-------------------------------------"

		${prog} 1024 inputs/temp_1024.data  inputs/power_1024.data 1 ${nc} 0 ${nf} ${ratio} 1 256 1200 25

	    	if [ -f ~/break ] ; then
			exit
	    	fi

		done	
    done
  done
done 
 
for nc in 3; do
  for nf in 4; do
    for ratio in 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1; do
    	for i in 1 2 3 4 5; do
	    	echo "-------------------------------------"
	    	echo ${prog} ratio: ${ratio} rep: ${i}
 	    	echo "-------------------------------------"

		${prog} 1024 inputs/temp_1024.data  inputs/power_1024.data 1 ${nc} 0 ${nf} ${ratio} 1 256 1200 25

	    	if [ -f ~/break ] ; then
			exit
	    	fi

		done	
    done
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
