# Blocking and Non-Blocking queue
This is an implementation of the paper [Simple, Fast, and Practical Non-Blocking and Blocking Concurrent Queue Algorithms](https://www.cs.rochester.edu/u/scott/papers/1996_PODC_queues.pdf).

### Overview of Repository

*   implemented three different types of queues
    *   `queues/one_lock_queue.h` (standard mutex based)
    *   `queues/two_lock_queue.h` (seperate locks for enqueuing and dequeuing mentioned in paper Figure 2)
    *   `queues/non_blocking_queue.h` (non blocking queue mentioned in paper Figure 1)

*   Driver programs to evaluate the correctness and performance of queue implementations.
    *   `driver_correctness.cpp` - In this driver program, producers repeatedly enqueue elements from an input file into the queue and consumers repeatedly dequeue elements from the queue. The driver program then validates whether the enqueued and dequeued elements are correct. It supports the following command-line parameters:
    
        *   `--n_producers`: Number of producer threads that will enqueue to the queue.
        *   `--n_consumers`: Number of consumer threads that will dequeue from the queue.
        *   `--input_file`: Absolute path of the input file that contains the elements to be inserted.
        *   `--init_allocator`: Initial number of elements to be pre-allocated. We will rely on pre-allocation to eliminate the effects of allocation/deallocation on throughput numbers (more on this in next point).
    
    For example, to test correctness of `one_lock_queue` with 2 producers and 2 consumers, run:
    
        $ make one_lock_queue_correctness
        $ ./one_lock_queue_correctness --n_producers 2 --n_consumers 2 --input_file /inputs/rand_10M
        
    
    *   `driver_throughput.cpp` - In this driver program, producers repeatedly enqueue elements in the queue and consumers repeatedly dequeue elements from the queue for a specified period of time. The number of operations performed by the workers is used to measure throughput of the queue. It supports the following command-line parameters:
    
        *   `--seconds`: Number of seconds the producer and consumer should run.
        *   `--n_producers`: Number of producer threads that will enqueue to the queue.
        *   `--n_consumers`: Number of consumer threads that will dequeue from the queue.
        *   `--init_allocator`: Initial number of elements to be pre-allocated. We will rely on pre-allocation to eliminate the effects of allocation/deallocation on throughput numbers (more on this in next point).
    
    For example, to measure the throughput of `non_blocking_queue` with 2 producers and 2 consumers for 5 seconds, run:
    
        $ make non_blocking_queue_throughput
        $ ./non_blocking_queue_throughput --n_producers 2 --n_consumers 2 --seconds 5 --init_allocator 100000000




