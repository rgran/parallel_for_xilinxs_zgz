#!/bin/bash
#---------------------------

bench=SPMM_MD
#matriz=mixtank_new.mtx
#matriz=thread.mtx
matriz=spmsrtls.mtx

echo "blocking test -----------------------------------------"
echo `date`

prog=./${bench}
for freq in 200 25 ; do
for nc in 0 1 2 3 4 ; do
for nf in 0 4; do
if [ ${nc} -gt 0 ] || [ ${nf} -gt 0 ]
then
#16384000
for BLOCK in  8 16 32 64 128 256 512 1024 2048 4092 8192; do
    	for i in 1 2 3 4 5; do
    	
	    	echo "--------------------------------------------------"
	    	echo ${prog} freq: ${freq} cores: ${nc}_${nf} chunk: ${BLOCK} rep: ${i}
 	    	echo "--------------------------------------------------"

		#${prog} 1024 inputs/temp_1024.data inputs/power_1024.data 1 ${nc} 0 ${nf} ${BLOCK} 1 256 1200 ${freq}
		${prog} matrices/${matriz} ${nc} 0 ${nf} ${BLOCK} 1 1200 ${freq}


	    	if [ -f ~/break ] ; then
			exit
	    	fi

		done	
    done 
cat  SPMM_MultiDyn_${nc}_${nf}.txt >>  SPMM_MultiDyn_${nc}_${nf}_${freq}.txt 
rm  SPMM_MultiDyn_${nc}_${nf}.txt 
fi
done
done
done

#~ mkdir ${prog}_dir
#~ mv *.txt ${prog}_dir
