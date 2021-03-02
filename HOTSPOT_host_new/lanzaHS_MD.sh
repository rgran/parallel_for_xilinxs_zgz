#!/bin/bash
#---------------------------

bench=HOTSPOT_MD

echo "blocking test -----------------------------------------"
echo `date`

prog=./${bench}
for freq in 200 25 ; do
for nc in 0 1 2 3 4 ; do
for nf in 0 4; do
if [ ${nc} -gt 0 ] || [ ${nf} -gt 0 ]
then
#16384000
for BLOCK in  2 4 8 16 32 64 128 256 512 1024; do
    	for i in 1 2 3 4 5; do
    	
	    	echo "--------------------------------------------------"
	    	echo ${prog} freq: ${freq} cores: ${nc}_${nf} chunk: ${BLOCK} rep: ${i}
 	    	echo "--------------------------------------------------"

	    #	${prog} otro.pdf ${CPUs} 0 ${GPUs} 0 1 25 1200
		${prog} 1024 inputs/temp_1024.data inputs/power_1024.data 1 ${nc} 0 ${nf} ${BLOCK} 1 256 1200 ${freq}



	    	if [ -f ~/break ] ; then
			exit
	    	fi

		done	
    done 
cat  HOTSPOT_MultiDyn_${CPUs}_${GPUs}.txt >>  HOTSPOT_MultiDyn_${CPUs}_${GPUs}_${freq}.txt 
fi
done
done
done

#~ mkdir ${prog}_dir
#~ mv *.txt ${prog}_dir
