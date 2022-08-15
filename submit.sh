#!/bin/bash
#
#SBATCH --cpus-per-task=8
#SBATCH --time=02:00
#SBATCH --mem=5G
#SBATCH --partition=fast

#one lock qeue correctness
#./one_lock_queue_correctness --n_producers 2 --n_consumers 2 --input_file /scratch/assignment5/inputs/rand_10M

#one lock qeue throughput
#./one_lock_queue_throughput --n_producers 4 --n_consumers 4


#two lock qeue correctness
#./two_lock_queue_correctness --n_producers 4 --n_consumers 4 --input_file /scratch/assignment5/inputs/rand_10M

#two lock qeuea throughput
#./two_lock_queue_throughput --n_producers 4 --n_consumers 4


#non-blocking qeue correctness
#./non_blocking_queue_correctness --n_producers 2 --n_consumers 2 --input_file /scratch/assignment5/inputs/rand_10M

#non-blocking queue throughput
./non_blocking_queue_throughput --n_producers 4 --n_consumers 4 --seconds 5 --init_allocator 100000000






