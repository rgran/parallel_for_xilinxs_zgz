#!/bin/bash
#GEMM wrap script for multipe data collection
#KrNikov 2019

dmesg -c > /dev/null

# #######
# #IOCTL#
# #######

# #CPUHPC
# ./gemm.sh -n 7 -f 1024 -c 4 -l 4 -k 2 -d 1 -i -s results/8_morecpuwork/CPUHPC/IOCTL/4c_0hp_4hpc_2chunk_ioctl.data
# ./gemm.sh -n 7 -f 1024 -c 4 -l 4 -k 4 -d 1 -i -s results/8_morecpuwork/CPUHPC/IOCTL/4c_0hp_4hpc_4chunk_ioctl.data
# ./gemm.sh -n 7 -f 1024 -c 4 -l 4 -k 8 -d 1 -i -s results/8_morecpuwork/CPUHPC/IOCTL/4c_0hp_4hpc_8chunk_ioctl.data
# ./gemm.sh -n 7 -f 1024 -c 4 -l 4 -k 16 -d 1 -i -s results/8_morecpuwork/CPUHPC/IOCTL/4c_0hp_4hpc_16chunk_ioctl.data
# ./gemm.sh -n 7 -f 1024 -c 4 -l 4 -k 32 -d 1 -i -s results/8_morecpuwork/CPUHPC/IOCTL/4c_0hp_4hpc_32chunk_ioctl.data
# ./gemm.sh -n 7 -f 1024 -c 4 -l 4 -k 64 -d 1 -i -s results/8_morecpuwork/CPUHPC/IOCTL/4c_0hp_4hpc_64chunk_ioctl.data
# ./gemm.sh -n 7 -f 1024 -c 4 -l 4 -k 128 -d 1 -i -s results/8_morecpuwork/CPUHPC/IOCTL/4c_0hp_4hpc_128chunk_ioctl.data
# ./gemm.sh -n 7 -f 1024 -c 4 -l 4 -k 256 -d 1 -i -s results/8_morecpuwork/CPUHPC/IOCTL/4c_0hp_4hpc_256chunk_ioctl.data
# ./gemm.sh -n 7 -f 1024 -c 4 -l 4 -k 512 -d 1 -i -s results/8_morecpuwork/CPUHPC/IOCTL/4c_0hp_4hpc_512chunk_ioctl.data
# ./gemm.sh -n 7 -f 1024 -c 4 -l 4 -k 1024 -d 1 -i -s results/8_morecpuwork/CPUHPC/IOCTL/4c_0hp_4hpc_1024chunk_ioctl.data

# #########
# #NOIOCTL#
# #########

# #CPUHPC
# ./gemm.sh -n 7 -f 1024 -c 4 -l 4 -k 2 -d 1 -s results/8_morecpuwork/CPUHPC/NOIOCTL/4c_0hp_4hpc_2chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 4 -l 4 -k 4 -d 1 -s results/8_morecpuwork/CPUHPC/NOIOCTL/4c_0hp_4hpc_4chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 4 -l 4 -k 8 -d 1 -s results/8_morecpuwork/CPUHPC/NOIOCTL/4c_0hp_4hpc_8chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 4 -l 4 -k 16 -d 1 -s results/8_morecpuwork/CPUHPC/NOIOCTL/4c_0hp_4hpc_16chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 4 -l 4 -k 32 -d 1 -s results/8_morecpuwork/CPUHPC/NOIOCTL/4c_0hp_4hpc_32chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 4 -l 4 -k 64 -d 1 -s results/8_morecpuwork/CPUHPC/NOIOCTL/4c_0hp_4hpc_64chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 4 -l 4 -k 128 -d 1 -s results/8_morecpuwork/CPUHPC/NOIOCTL/4c_0hp_4hpc_128chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 4 -l 4 -k 256 -d 1 -s results/8_morecpuwork/CPUHPC/NOIOCTL/4c_0hp_4hpc_256chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 4 -l 4 -k 512 -d 1 -s results/8_morecpuwork/CPUHPC/NOIOCTL/4c_0hp_4hpc_512chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 4 -l 4 -k 1024 -d 1 -s results/8_morecpuwork/CPUHPC/NOIOCTL/4c_0hp_4hpc_1024chunk_noioctl.data

# #CPU
./gemm.sh -n 7 -f 1024 -c 4 -p 0 -k 2 -d 1 -s results/8_morecpuwork/CPU/4c_0hp_0hpc_2chunk_noioctl.data
./gemm.sh -n 7 -f 1024 -c 4 -p 0 -k 4 -d 1 -s results/8_morecpuwork/CPU/4c_0hp_0hpc_4chunk_noioctl.data
./gemm.sh -n 7 -f 1024 -c 4 -p 0 -k 8 -d 1 -s results/8_morecpuwork/CPU/4c_0hp_0hpc_8chunk_noioctl.data
./gemm.sh -n 7 -f 1024 -c 4 -p 0 -k 16 -d 1 -s results/8_morecpuwork/CPU/4c_0hp_0hpc_16chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 4 -p 0 -k 32 -d 1 -s results/8_morecpuwork/CPU/4c_0hp_0hpc_32chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 4 -p 0 -k 64 -d 1 -s results/8_morecpuwork/CPU/4c_0hp_0hpc_64chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 4 -p 0 -k 128 -d 1 -s results/8_morecpuwork/CPU/4c_0hp_0hpc_128chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 4 -p 0 -k 256 -d 1 -s results/8_morecpuwork/CPU/4c_0hp_0hpc_256chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 4 -p 0 -k 512 -d 1 -s results/8_morecpuwork/CPU/4c_0hp_0hpc_512chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 4 -p 0 -k 1024 -d 1 -s results/8_morecpuwork/CPU/4c_0hp_0hpc_1024chunk_noioctl.data

# #HPC
# ./gemm.sh -n 7 -f 1024 -c 0 -l 4 -k 2 -d 1 -s results/8_morecpuwork/HPC/0c_0hp_4hpc_2chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 0 -l 4 -k 4 -d 1 -s results/8_morecpuwork/HPC/0c_0hp_4hpc_4chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 0 -l 4 -k 8 -d 1 -s results/8_morecpuwork/HPC/0c_0hp_4hpc_8chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 0 -l 4 -k 16 -d 1 -s results/8_morecpuwork/HPC/0c_0hp_4hpc_16chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 0 -l 4 -k 32 -d 1 -s results/8_morecpuwork/HPC/0c_0hp_4hpc_32chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 0 -l 4 -k 64 -d 1 -s results/8_morecpuwork/HPC/0c_0hp_4hpc_64chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 0 -l 4 -k 128 -d 1 -s results/8_morecpuwork/HPC/0c_0hp_4hpc_128chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 0 -l 4 -k 256 -d 1 -s results/8_morecpuwork/HPC/0c_0hp_4hpc_256chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 0 -l 4 -k 512 -d 1 -s results/8_morecpuwork/HPC/0c_0hp_4hpc_512chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 0 -l 4 -k 1024 -d 1 -s results/8_morecpuwork/HPC/0c_0hp_4hpc_1024chunk_noioctl.data

# #######
# #IDEAL#
# #######

# #CPU
# ./gemm.sh -n 7 -f 1024 -c 1 -p 0 -k 2 -d 1 -s results/8_morecpuwork/IDEAL/CPU/1c_0hp_0hpc_2chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 1 -p 0 -k 4 -d 1 -s results/8_morecpuwork/IDEAL/CPU/1c_0hp_0hpc_4chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 1 -p 0 -k 8 -d 1 -s results/8_morecpuwork/IDEAL/CPU/1c_0hp_0hpc_8chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 1 -p 0 -k 16 -d 1 -s results/8_morecpuwork/IDEAL/CPU/1c_0hp_0hpc_16chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 1 -p 0 -k 32 -d 1 -s results/8_morecpuwork/IDEAL/CPU/1c_0hp_0hpc_32chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 1 -p 0 -k 64 -d 1 -s results/8_morecpuwork/IDEAL/CPU/1c_0hp_0hpc_64chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 1 -p 0 -k 128 -d 1 -s results/8_morecpuwork/IDEAL/CPU/1c_0hp_0hpc_128chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 1 -p 0 -k 256 -d 1 -s results/8_morecpuwork/IDEAL/CPU/1c_0hp_0hpc_256chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 1 -p 0 -k 512 -d 1 -s results/8_morecpuwork/IDEAL/CPU/1c_0hp_0hpc_512chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 1 -p 0 -k 1024 -d 1 -s results/8_morecpuwork/IDEAL/CPU/1c_0hp_0hpc_1024chunk_noioctl.data

# #HPC
# ./gemm.sh -n 7 -f 1024 -c 0 -l 1 -k 2 -d 1 -s results/8_morecpuwork/IDEAL/HPC/0c_0hp_1hpc_2chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 0 -l 1 -k 4 -d 1 -s results/8_morecpuwork/IDEAL/HPC/0c_0hp_1hpc_4chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 0 -l 1 -k 8 -d 1 -s results/8_morecpuwork/IDEAL/HPC/0c_0hp_1hpc_8chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 0 -l 1 -k 16 -d 1 -s results/8_morecpuwork/IDEAL/HPC/0c_0hp_1hpc_16chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 0 -l 1 -k 32 -d 1 -s results/8_morecpuwork/IDEAL/HPC/0c_0hp_1hpc_32chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 0 -l 1 -k 64 -d 1 -s results/8_morecpuwork/IDEAL/HPC/0c_0hp_1hpc_64chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 0 -l 1 -k 128 -d 1 -s results/8_morecpuwork/IDEAL/HPC/0c_0hp_1hpc_128chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 0 -l 1 -k 256 -d 1 -s results/8_morecpuwork/IDEAL/HPC/0c_0hp_1hpc_256chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 0 -l 1 -k 512 -d 1 -s results/8_morecpuwork/IDEAL/HPC/0c_0hp_1hpc_512chunk_noioctl.data
# ./gemm.sh -n 7 -f 1024 -c 0 -l 1 -k 1024 -d 1 -s results/8_morecpuwork/IDEAL/HPC/0c_0hp_1hpc_1024chunk_noioctl.data


#######
#IOCTL#
#######

#CPUHP
./gemm.sh -n 7 -f 1024 -c 4 -p 4 -k 2 -d 1 -i -s results/8_morecpuwork/CPUHP/IOCTL/4c_4hp_0hpc_2chunk_ioctl.data
./gemm.sh -n 7 -f 1024 -c 4 -p 4 -k 4 -d 1 -i -s results/8_morecpuwork/CPUHP/IOCTL/4c_4hp_0hpc_4chunk_ioctl.data
./gemm.sh -n 7 -f 1024 -c 4 -p 4 -k 8 -d 1 -i -s results/8_morecpuwork/CPUHP/IOCTL/4c_4hp_0hpc_8chunk_ioctl.data
./gemm.sh -n 7 -f 1024 -c 4 -p 4 -k 16 -d 1 -i -s results/8_morecpuwork/CPUHP/IOCTL/4c_4hp_0hpc_16chunk_ioctl.data
./gemm.sh -n 7 -f 1024 -c 4 -p 4 -k 32 -d 1 -i -s results/8_morecpuwork/CPUHP/IOCTL/4c_4hp_0hpc_32chunk_ioctl.data
./gemm.sh -n 7 -f 1024 -c 4 -p 4 -k 64 -d 1 -i -s results/8_morecpuwork/CPUHP/IOCTL/4c_4hp_0hpc_64chunk_ioctl.data
./gemm.sh -n 7 -f 1024 -c 4 -p 4 -k 128 -d 1 -i -s results/8_morecpuwork/CPUHP/IOCTL/4c_4hp_0hpc_128chunk_ioctl.data
./gemm.sh -n 7 -f 1024 -c 4 -p 4 -k 256 -d 1 -i -s results/8_morecpuwork/CPUHP/IOCTL/4c_4hp_0hpc_256chunk_ioctl.data
./gemm.sh -n 7 -f 1024 -c 4 -p 4 -k 512 -d 1 -i -s results/8_morecpuwork/CPUHP/IOCTL/4c_4hp_0hpc_512chunk_ioctl.data
./gemm.sh -n 7 -f 1024 -c 4 -p 4 -k 1024 -d 1 -i -s results/8_morecpuwork/CPUHP/IOCTL/4c_4hp_0hpc_1024chunk_ioctl.data

#########
#NOIOCTL#
#########

#CPUHP
./gemm.sh -n 7 -f 1024 -c 4 -p 4 -k 2 -d 1 -s results/8_morecpuwork/CPUHP/NOIOCTL/4c_4hp_0hpc_2chunk_noioctl.data
./gemm.sh -n 7 -f 1024 -c 4 -p 4 -k 4 -d 1 -s results/8_morecpuwork/CPUHP/NOIOCTL/4c_4hp_0hpc_4chunk_noioctl.data
./gemm.sh -n 7 -f 1024 -c 4 -p 4 -k 8 -d 1 -s results/8_morecpuwork/CPUHP/NOIOCTL/4c_4hp_0hpc_8chunk_noioctl.data
./gemm.sh -n 7 -f 1024 -c 4 -p 4 -k 16 -d 1 -s results/8_morecpuwork/CPUHP/NOIOCTL/4c_4hp_0hpc_16chunk_noioctl.data
./gemm.sh -n 7 -f 1024 -c 4 -p 4 -k 32 -d 1 -s results/8_morecpuwork/CPUHP/NOIOCTL/4c_4hp_0hpc_32chunk_noioctl.data
./gemm.sh -n 7 -f 1024 -c 4 -p 4 -k 64 -d 1 -s results/8_morecpuwork/CPUHP/NOIOCTL/4c_4hp_0hpc_64chunk_noioctl.data
./gemm.sh -n 7 -f 1024 -c 4 -p 4 -k 128 -d 1 -s results/8_morecpuwork/CPUHP/NOIOCTL/4c_4hp_0hpc_128chunk_noioctl.data
./gemm.sh -n 7 -f 1024 -c 4 -p 4 -k 256 -d 1 -s results/8_morecpuwork/CPUHP/NOIOCTL/4c_4hp_0hpc_256chunk_noioctl.data
./gemm.sh -n 7 -f 1024 -c 4 -p 4 -k 512 -d 1 -s results/8_morecpuwork/CPUHP/NOIOCTL/4c_4hp_0hpc_512chunk_noioctl.data
./gemm.sh -n 7 -f 1024 -c 4 -p 4 -k 1024 -d 1 -s results/8_morecpuwork/CPUHP/NOIOCTL/4c_4hp_0hpc_1024chunk_noioctl.data

#HP
./gemm.sh -n 7 -f 1024 -c 0 -p 4 -k 2 -d 1 -s results/8_morecpuwork/HP/0c_4hp_0hpc_2chunk_noioctl.data
./gemm.sh -n 7 -f 1024 -c 0 -p 4 -k 4 -d 1 -s results/8_morecpuwork/HP/0c_4hp_0hpc_4chunk_noioctl.data
./gemm.sh -n 7 -f 1024 -c 0 -p 4 -k 8 -d 1 -s results/8_morecpuwork/HP/0c_4hp_0hpc_8chunk_noioctl.data
./gemm.sh -n 7 -f 1024 -c 0 -p 4 -k 16 -d 1 -s results/8_morecpuwork/HP/0c_4hp_0hpc_16chunk_noioctl.data
./gemm.sh -n 7 -f 1024 -c 0 -p 4 -k 32 -d 1 -s results/8_morecpuwork/HP/0c_4hp_0hpc_32chunk_noioctl.data
./gemm.sh -n 7 -f 1024 -c 0 -p 4 -k 64 -d 1 -s results/8_morecpuwork/HP/0c_4hp_0hpc_64chunk_noioctl.data
./gemm.sh -n 7 -f 1024 -c 0 -p 4 -k 128 -d 1 -s results/8_morecpuwork/HP/0c_4hp_0hpc_128chunk_noioctl.data
./gemm.sh -n 7 -f 1024 -c 0 -p 4 -k 256 -d 1 -s results/8_morecpuwork/HP/0c_4hp_0hpc_256chunk_noioctl.data
./gemm.sh -n 7 -f 1024 -c 0 -p 4 -k 512 -d 1 -s results/8_morecpuwork/HP/0c_4hp_0hpc_512chunk_noioctl.data
./gemm.sh -n 7 -f 1024 -c 0 -p 4 -k 1024 -d 1 -s results/8_morecpuwork/HP/0c_4hp_0hpc_1024chunk_noioctl.data

