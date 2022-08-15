#!/bin/bash

rm slur*
########################### For one_lock_queue_correctness
#make one_lock_queue_correctness
#make one_lock_queue_throughput


########################### For two_lock_queue_correctness
#make two_lock_queue_correctness
#make two_lock_queue_throughput

########################### For non_blocking_queue_throughput
#make non_blocking_queue_correctness
make non_blocking_queue_throughput


sbatch submit.sh
squeue


########################### For Triangle count
#make triangle_counting
#make triangle_counting_parallel

############################ For Page Rank
#make USE_INT=1 page_rank_parallel
#make page_rank_parallel

'''
for ((i=0;i<10;i++))
do
    sbatch submit.sh
    sleep 10


    echo "################################"
    squeue
done



for i in $(ls | grep slurm)
do
    cat $i >> Out.out
    echo "---------------------------------------------" >> Out.out
done

echo "################################"


'''
