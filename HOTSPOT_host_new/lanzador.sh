#!/bin/bash
#Script para lanzar un banco de pruebas

n_veces=5 #numero de veces que se repite el experimento
dimension=1024 #dimension de la matriz
temp_file=inputs/temp_1024.data
power_file=inputs/power_1024.data
scheduler_type=2 #1stands for MultiDynamic and 2 Multihap
rept_parallel_for=256 #numero de veces que ser repite la planificación del parallel for
result_dir="/root/parallel_for_xilinx/HOTSPOT_host/results"
current_time=$(date +%x_%T | sed 's/[\/:]/_/g')
base_name="${result_dir}/HOTSPOT_cpuidle_${current_time}"
freq_basename="/sys/devices/system/cpu/cpufreq/policy0"


function change_online_cores {
  online_cores=$(($1 - 1))

  for i in {1..3}
  do
    if [ "$i" -le "${online_cores}" ]; then
        echo "1" > "/sys/devices/system/cpu/cpu${i}/online"
    else
        echo "0" > "/sys/devices/system/cpu/cpu${i}/online"
    fi
  done
}

# ensure userspace governor
echo "userspace" > "${freq_basename}/scaling_governor"

for core in {1..4}; do
#  change_online_cores ${core}
  
  for acc in {0..4}; do 

#    for chunk_size in 2 4 8 16 32 64 128; do
	chunk_size=0

# for freq_fpga in 25 40 50 100 150 200; do
       for freq_fpga in 200; do
#      for freq_fpga in  200; do

#       for freq_cpu in 299999 399999 599999 1199999; do
#       for freq_cpu in 300 400 600 1200; do
        for freq_cpu in 1200; do
#         echo "${freq_cpu}" > "${freq_basename}/scaling_setspeed"		 
         ./clk_fpga_set ${freq_fpga} ${freq_cpu}
         #change_online_cores ${core}
#         for freq_cpu in 1200; do
	  out_file="${base_name}_${core}C_${acc}A_${chunk_size}CHUNK_${freq_cpu}FC_${freq_fpga}FF"
          ./hotspot.sh -n ${n_veces} -f ${dimension} -a ${temp_file} -b ${power_file} \
	      -e -c ${core} -l ${acc} -k ${chunk_size} -d ${scheduler_type} -i -r ${rept_parallel_for} -u ${freq_cpu} -v ${freq_fpga} -s "${out_file}"
          echo ./hotspot.sh -n ${n_veces} -f ${dimension} -a ${temp_file} -b ${power_file} \
	      -e -c ${core} -l ${acc} -k ${chunk_size} -d ${scheduler_type} -i -r ${rept_parallel_for} -u ${freq_cpu} -v ${freq_fpga} -s "${out_file}"
	  #change_online_cores 4
	done
      done
##    done
  done
done
# return to the default state
change_online_cores 4 

# return to ondemand
echo "ondemand" > "${freq_basename}/scaling_governor"

