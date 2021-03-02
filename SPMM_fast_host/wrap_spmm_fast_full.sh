#!/bin/bash
#SPMM wrap script for multipe data collection
#KrNikov 2019

dmesg -c > /dev/null

#ioctl

./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 250 -i -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_250chunk_oldcpucode_ioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 500 -i -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_500chunk_oldcpucode_ioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 750 -i -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_750chunk_oldcpucode_ioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 1000 -i -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_1000chunk_oldcpucode_ioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 1250 -i -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_1250chunk_oldcpucode_ioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 1500 -i -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_1500chunk_oldcpucode_ioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 1750 -i -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_1750chunk_oldcpucode_ioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 2000 -i -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_2000chunk_oldcpucode_ioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 2250 -i -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_2250chunk_oldcpucode_ioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 2500 -i -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_2500chunk_oldcpucode_ioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 2750 -i -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_2750chunk_oldcpucode_ioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 3000 -i -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_3000chunk_oldcpucode_ioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 3250 -i -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_3250chunk_oldcpucode_ioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 3500 -i -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_3500chunk_oldcpucode_ioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 3750 -i -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_3750chunk_oldcpucode_ioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 4000 -i -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_4000chunk_oldcpucode_ioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 4250 -i -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_4250chunk_oldcpucode_ioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 4500 -i -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_4500chunk_oldcpucode_ioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 4750 -i -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_4750chunk_oldcpucode_ioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 5000 -i -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_5000chunk_oldcpucode_ioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 5250 -i -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_5250chunk_oldcpucode_ioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 5500 -i -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_5500chunk_oldcpucode_ioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 5750 -i -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_5750chunk_oldcpucode_ioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 6000 -i -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_6000chunk_oldcpucode_ioctl.data

#noioctl

# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 1 -p 0 -k 250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/cpu/1c_0hp_0hpc_250chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 1 -p 0 -k 500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/cpu/1c_0hp_0hpc_500chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 1 -p 0 -k 750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/cpu/1c_0hp_0hpc_750chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 1 -p 0 -k 1000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/cpu/1c_0hp_0hpc_1000chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 1 -p 0 -k 1250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/cpu/1c_0hp_0hpc_1250chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 1 -p 0 -k 1500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/cpu/1c_0hp_0hpc_1500chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 1 -p 0 -k 1750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/cpu/1c_0hp_0hpc_1750chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 1 -p 0 -k 2000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/cpu/1c_0hp_0hpc_2000chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 1 -p 0 -k 2250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/cpu/1c_0hp_0hpc_2250chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 1 -p 0 -k 2500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/cpu/1c_0hp_0hpc_2500chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 1 -p 0 -k 2750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/cpu/1c_0hp_0hpc_2750chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 1 -p 0 -k 3000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/cpu/1c_0hp_0hpc_3000chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 1 -p 0 -k 3250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/cpu/1c_0hp_0hpc_3250chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 1 -p 0 -k 3500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/cpu/1c_0hp_0hpc_3500chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 1 -p 0 -k 3750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/cpu/1c_0hp_0hpc_3750chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 1 -p 0 -k 4000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/cpu/1c_0hp_0hpc_4000chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 1 -p 0 -k 4250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/cpu/1c_0hp_0hpc_4250chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 1 -p 0 -k 4500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/cpu/1c_0hp_0hpc_4500chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 1 -p 0 -k 4750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/cpu/1c_0hp_0hpc_4750chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 1 -p 0 -k 5000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/cpu/1c_0hp_0hpc_5000chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 1 -p 0 -k 5250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/cpu/1c_0hp_0hpc_5250chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 1 -p 0 -k 5500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/cpu/1c_0hp_0hpc_5500chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 1 -p 0 -k 5750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/cpu/1c_0hp_0hpc_5750chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 1 -p 0 -k 6000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/cpu/1c_0hp_0hpc_6000chunk_noioctl.data

# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 0 -k 250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_0hp_0hpc_250chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 0 -k 500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_0hp_0hpc_500chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 0 -k 750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_0hp_0hpc_750chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 0 -k 1000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_0hp_0hpc_1000chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 0 -k 1250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_0hp_0hpc_1250chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 0 -k 1500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_0hp_0hpc_1500chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 0 -k 1750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_0hp_0hpc_1750chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 0 -k 2000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_0hp_0hpc_2000chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 0 -k 2250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_0hp_0hpc_2250chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 0 -k 2500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_0hp_0hpc_2500chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 0 -k 2750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_0hp_0hpc_2750chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 0 -k 3000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_0hp_0hpc_3000chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 0 -k 3250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_0hp_0hpc_3250chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 0 -k 3500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_0hp_0hpc_3500chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 0 -k 3750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_0hp_0hpc_3750chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 0 -k 4000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_0hp_0hpc_4000chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 0 -k 4250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_0hp_0hpc_4250chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 0 -k 4500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_0hp_0hpc_4500chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 0 -k 4750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_0hp_0hpc_4750chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 0 -k 5000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_0hp_0hpc_5000chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 0 -k 5250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_0hp_0hpc_5250chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 0 -k 5500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_0hp_0hpc_5500chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 0 -k 5750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_0hp_0hpc_5750chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 0 -k 6000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_0hp_0hpc_6000chunk_noioctl.data

# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 1 -k 250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/fpga/0c_1hp_0hpc_250chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 1 -k 500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/fpga/0c_1hp_0hpc_500chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 1 -k 750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/fpga/0c_1hp_0hpc_750chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 1 -k 1000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/fpga/0c_1hp_0hpc_1000chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 1 -k 1250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/fpga/0c_1hp_0hpc_1250chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 1 -k 1500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/fpga/0c_1hp_0hpc_1500chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 1 -k 1750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/fpga/0c_1hp_0hpc_1750chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 1 -k 2000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/fpga/0c_1hp_0hpc_2000chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 1 -k 2250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/fpga/0c_1hp_0hpc_2250chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 1 -k 2500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/fpga/0c_1hp_0hpc_2500chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 1 -k 2750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/fpga/0c_1hp_0hpc_2750chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 1 -k 3000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/fpga/0c_1hp_0hpc_3000chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 1 -k 3250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/fpga/0c_1hp_0hpc_3250chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 1 -k 3500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/fpga/0c_1hp_0hpc_3500chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 1 -k 3750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/fpga/0c_1hp_0hpc_3750chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 1 -k 4000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/fpga/0c_1hp_0hpc_4000chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 1 -k 4250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/fpga/0c_1hp_0hpc_4250chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 1 -k 4500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/fpga/0c_1hp_0hpc_4500chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 1 -k 4750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/fpga/0c_1hp_0hpc_4750chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 1 -k 5000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/fpga/0c_1hp_0hpc_5000chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 1 -k 5250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/fpga/0c_1hp_0hpc_5250chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 1 -k 5500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/fpga/0c_1hp_0hpc_5500chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 1 -k 5750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/fpga/0c_1hp_0hpc_5750chunk_noioctl.data
# ./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 1 -k 6000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/ideal/fpga/0c_1hp_0hpc_6000chunk_noioctl.data

./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 4 -k 250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/0c_4hp_0hpc_250chunk_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 4 -k 500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/0c_4hp_0hpc_500chunk_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 4 -k 750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/0c_4hp_0hpc_750chunk_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 4 -k 1000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/0c_4hp_0hpc_1000chunk_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 4 -k 1250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/0c_4hp_0hpc_1250chunk_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 4 -k 1500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/0c_4hp_0hpc_1500chunk_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 4 -k 1750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/0c_4hp_0hpc_1750chunk_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 4 -k 2000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/0c_4hp_0hpc_2000chunk_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 4 -k 2250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/0c_4hp_0hpc_2250chunk_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 4 -k 2500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/0c_4hp_0hpc_2500chunk_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 4 -k 2750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/0c_4hp_0hpc_2750chunk_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 4 -k 3000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/0c_4hp_0hpc_3000chunk_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 4 -k 3250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/0c_4hp_0hpc_3250chunk_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 4 -k 3500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/0c_4hp_0hpc_3500chunk_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 4 -k 3750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/0c_4hp_0hpc_3750chunk_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 4 -k 4000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/0c_4hp_0hpc_4000chunk_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 4 -k 4250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/0c_4hp_0hpc_4250chunk_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 4 -k 4500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/0c_4hp_0hpc_4500chunk_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 4 -k 4750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/0c_4hp_0hpc_4750chunk_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 4 -k 5000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/0c_4hp_0hpc_5000chunk_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 4 -k 5250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/0c_4hp_0hpc_5250chunk_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 4 -k 5500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/0c_4hp_0hpc_5500chunk_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 4 -k 5750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/0c_4hp_0hpc_5750chunk_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 0 -p 4 -k 6000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/0c_4hp_0hpc_6000chunk_noioctl.data

./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_250chunk_oldcpucode_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_500chunk_oldcpucode_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_750chunk_oldcpucode_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 1000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_1000chunk_oldcpucode_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 1250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_1250chunk_oldcpucode_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 1500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_1500chunk_oldcpucode_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 1750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_1750chunk_oldcpucode_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 2000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_2000chunk_oldcpucode_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 2250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_2250chunk_oldcpucode_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 2500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_2500chunk_oldcpucode_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 2750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_2750chunk_oldcpucode_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 3000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_3000chunk_oldcpucode_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 3250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_3250chunk_oldcpucode_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 3500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_3500chunk_oldcpucode_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 3750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_3750chunk_oldcpucode_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 4000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_4000chunk_oldcpucode_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 4250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_4250chunk_oldcpucode_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 4500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_4500chunk_oldcpucode_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 4750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_4750chunk_oldcpucode_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 5000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_5000chunk_oldcpucode_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 5250 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_5250chunk_oldcpucode_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 5500 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_5500chunk_oldcpucode_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 5750 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_5750chunk_oldcpucode_noioctl.data
./spmm_fast.sh -n 7 -m matrices/mixtank_new.mtx -c 4 -p 4 -k 6000 -s results/200mhz_hp4_outputfix_transmat_cpupower_fullresults/runtime/4c_4hp_0hpc_6000chunk_oldcpucode_noioctl.data