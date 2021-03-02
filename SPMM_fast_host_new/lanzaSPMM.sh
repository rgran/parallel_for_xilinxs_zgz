#!/bin/bash
#---------------------------

bench=SPMM_fast_MULTIFASTFIT

echo "blocking test -----------------------------------------"
echo `date`

prog=./${bench}
for freq in 25 200 ; do
for CPUs in 0 1 2 3 4; do
for GPUs in 0 1 2 3 4; do
if [ ${CPUs} -gt 0 ] || [ ${GPUs} -gt 0 ]
then
    	for i in 1 2 3 4 5; do
    	
	    	echo "-------------------------------------"
	    	echo ${prog} ratio: ${ratio} rep: ${i}
 	    	echo "-------------------------------------"

		${prog} matrices/mixtank_new.mtx ${CPUs} 0 ${GPUs} 0.95 1 1200 ${freq}

	    	if [ -f ~/break ] ; then
			exit
	    	fi

		done	
fi
    done 
done
mv SPMM_FastFit_all.txt SPMM_FastFit_all_${freq}.txt
done
#~ mkdir ${prog}_dir
#~ mv *.txt ${prog}_dir
