#!/bin/bash
#---------------------------

bench=SPMM_fast_MULTIFASTFIT

#matriz=mixtank_new.mtx
#matriz=thread.mtx
matriz=spmsrtls.mtx

echo "blocking test -----------------------------------------"
echo `date`

prog=./${bench}



#for nc in 3 4; do
for nc in 3; do
#  for nf in 0 1 2 3 4; do
  for nf in 2; do
    if [ ${nc} -ne 0 ] || [ ${nf} -ne 0 ] ; then     
    	for i in 1 2 3 4 5; do
	    	echo "-------------------------------------"
	    	echo ${prog} nc: ${nc} nf: ${nf} ratio: 0.0 rep: ${i}
 	    	echo "-------------------------------------"
#establezco a 0.9 el punto objetivo para el scheduler fast fit
		${prog} matrices/${matriz} ${nc} 0 ${nf} 0.5 1 1200 200

	    	if [ -f ~/break ] ; then
			exit
	    	fi

		done	
    fi
  done
done 

mv SPMM_FastFit_all.txt SPMM_FastFit_all_200MHz.txt   

exit

for nc in 0 1 2 3 4; do
  for nf in 0 1 2 3 4; do
    if [ ${nc} -ne 0 ] || [ ${nf} -ne 0 ] ; then     
    	for i in 1 2 3 4 5; do
	    	echo "-------------------------------------"
	    	echo ${prog} nc: ${nc} nf: ${nf} ratio: 0.0 rep: ${i}
 	    	echo "-------------------------------------"
#establezco a 0.9 el punto objetivo para el scheduler fast fit
		${prog} matrices/${matriz} ${nc} 0 ${nf} 0.9 1 1200 25

	    	if [ -f ~/break ] ; then
			exit
	    	fi

		done	
    fi
  done
done 

mv SPMM_FastFit_all.txt SPMM_FastFit_all_25MHz.txt   
exit
 

#~ mkdir ${prog}_dir
#~ mv *.txt ${prog}_dir
