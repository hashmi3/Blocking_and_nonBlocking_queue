#include "../common/allocator.h"

#define LFENCE asm volatile("lfence" : : : "memory")
#define SFENCE asm volatile("sfence" : : : "memory")

template <class P>
struct pointer_t{
    P* ptr;
    
    P* address(){
        uint64_t msk = 0x0000FFFFFFFFFFFF;
        return (P* )( msk & (uint64_t)ptr);
    }


    uint count(){
        uint64_t msk = 0xFFFF000000000000;
        msk = msk &  (uint64_t)ptr;
        return msk >> 48;
    }
    
    void count_inc(){
        uint64_t temp = count();
        uint64_t msk = 0x0000FFFFFFFFFFFF;
 
        temp++;
        temp <<= 48;
        ptr = (P*)( (uint64_t)ptr & msk );
        ptr = (P*)( (uint64_t)ptr | temp );
 
        return;
    }


    bool operator == (const pointer_t &lhs) const
    {
        return lhs.ptr == ptr;
    }

};

template <class T>
class Node
{
public:
    T value;
    pointer_t<Node <T>> next;
};

template <class T>
class NonBlockingQueue
{
    CustomAllocator my_allocator_;
    pointer_t<Node <T>> q_head;
    pointer_t<Node <T>> q_tail;
public:
    
    NonBlockingQueue() : my_allocator_()
    {
        std::cout << "Using NonBlockingQueue\n";        
    }

    void initQueue(long t_my_allocator_size){
        std::cout << "Using Allocator\n";
        my_allocator_.initialize(t_my_allocator_size, sizeof(Node<T>));
        // Initialize the queue head or tail here
        Node<T>* newNode = (Node<T>*)my_allocator_.newNode();
        newNode-> next.ptr = NULL;
        q_head.ptr = newNode;
        q_tail.ptr = newNode;
        
        
        //my_allocator_.freeNode(newNode);
    }

    void enqueue(T value)
    {
        // Use LFENCE and SFENCE as mentioned in pseudocode
        Node<T>* newNode = (Node<T>*)my_allocator_.newNode();
        newNode-> value = value;
        newNode-> next.ptr = NULL;
        pointer_t<Node<T>> tail;
        pointer_t<Node<T>> temp;
        pointer_t<Node<T>> next; 
        SFENCE;
        //return;
        while(1){
            tail = q_tail;
            LFENCE;
            next = tail.address()->next;
            LFENCE;
            if(tail == q_tail){
                if(next.address() == NULL){
                    temp.ptr = newNode;
                    if( CAS( &tail.address()->next, next, temp ) ){ 
                        next.count_inc();
                        break;} 
                }else{
                    temp.ptr = next.address();
                    if(CAS(&q_tail, tail, temp ) ){  // TOPU
                    
                    tail.count_inc();
                    }
                }


            }
        }
        SFENCE;
        temp.ptr = newNode;
        if( CAS(&q_tail, tail, temp ) ){

            tail.count_inc();
        }

    }

    bool dequeue(T *value)
    {
        // Use LFENCE and SFENCE as mentioned in pseudocode
        bool ret_value = false;
        pointer_t<Node<T>> tail;
        pointer_t<Node<T>> head;
        pointer_t<Node<T>> next; 
        pointer_t<Node<T>> temp; 
        while(1){
            head = q_head;
            LFENCE;
            tail = q_tail;
            LFENCE;
            next = head.address() -> next;
            LFENCE;
            if( head == q_head){
                if(head.address() == tail.address() ){
                    if(next.address() == NULL)
                        return false;
                    temp.ptr = next.address();
                    if (CAS(&q_tail, tail, temp ) ){
                        tail.count_inc(); 
                    }
                } else {
                    *value = next.address()->value;
                    temp.ptr = next.address();
                    if(CAS(&q_head, head, temp  ) ){
                        head.count_inc();
                        break;}
                }
            }
        }
        my_allocator_.freeNode(head.address());   
        return true;
        
        return ret_value;
    }

    void cleanup()
    {
        my_allocator_.cleanup();
    }

};

