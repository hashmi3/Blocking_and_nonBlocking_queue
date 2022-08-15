#include "../common/allocator.h"

template <class T>
class Node
{
public:
    T value;
    Node<T> * next;

};

std::mutex mtx_e;
std::mutex mtx_d;


template <class T>
class TwoLockQueue
{
    CustomAllocator my_allocator_;
    Node<T> * q_head;
    Node<T> * q_tail;

public:
    TwoLockQueue() : my_allocator_()
    {
        std::cout << "Using TwoLockQueue\n";
    }

    void initQueue(long t_my_allocator_size){
        std::cout << "Using Allocator\n";
        my_allocator_.initialize(t_my_allocator_size, sizeof(Node<T>));
        // Initialize the queue head or tail here
        Node<T>* newNode = (Node<T>*)my_allocator_.newNode();
        newNode->next = NULL;
        q_head = newNode; 
        q_tail = newNode;
        
        //my_allocator_.freeNode(newNode);
    }

    void enqueue(T value)
    {
        Node<T> * node = (Node<T> *) my_allocator_.newNode();
        node-> value = value;
        node-> next = NULL;
        mtx_e.lock();
        q_tail->next = node;
        q_tail = node;
        mtx_e.unlock();

    }

    bool dequeue(T *value)
    {
        bool ret_value = false;
        mtx_d.lock();
        Node<T> * node = q_head;
        Node<T> * new_head = q_head-> next;            
        if(new_head == NULL){
            mtx_d.unlock();
            return ret_value;
        }
        *value = new_head->value;
        q_head = new_head;
        my_allocator_.freeNode(node);
        mtx_d.unlock();

        return !ret_value ;
    }

    void cleanup()
    {
        my_allocator_.cleanup();
    }
};