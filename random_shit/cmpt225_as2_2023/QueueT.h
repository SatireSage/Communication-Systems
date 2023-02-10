#pragma once
#include <iostream>
#include <stdexcept>
using std::cout;
using std::endl;
using std::ostream;
using std::runtime_error;

template <class T>
class NodeT
{
public:
    T data;
    NodeT *next;
    NodeT() : data(0), next(nullptr){};
    NodeT(T val) : data(val), next(nullptr){};
    NodeT(T val, NodeT *nxt) : data(val), next(nxt){};
    ~NodeT(){};
};

template <class T>
class QueueT
{
private:
    NodeT<T> *front;                                            // pointer to front
    NodeT<T> *back;                                             // pointer to back
    int current_size;                                           // current size of queue
    void copy_queue(const QueueT &q);                           // helper function for copy constructor
    void delete_queue();                                        // helper function for deleting queue
    friend ostream &operator<<(ostream &os, const QueueT<T> &q) // friend function for << operator
    {
        NodeT<T> *temp = q.front; // prints our queue
        while (temp != nullptr)
        {
            os << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
        return os;
    }

public:
    QueueT();                              // default constructor
    QueueT(const QueueT &q);               // copy constructor
    ~QueueT();                             // destructor
    QueueT<T> &operator=(const QueueT &q); // overloaded assignment operator
    void enqueue(T val);                   // enqueue
    T dequeue();                           // dequeue
    bool empty() const;                    // empty
    int size() const;                      // size
};

// default constructor !!
template <class T>
QueueT<T>::QueueT()
{
    front = nullptr; // sets the front and  back to null, and current size to 0
    back = nullptr;
    current_size = 0;
}

// helper function for deep copying queue !!
template <class T>
void QueueT<T>::copy_queue(const QueueT &q)
{
    front = nullptr;
    back = nullptr;
    current_size = q.current_size;
    NodeT<T> *original = q.front;
    if (original != nullptr)
    {
        front = new NodeT<T>(original->data);
        original = original->next;
        NodeT<T> *copy = front;
        while (original != nullptr)
        {
            NodeT<T> *newNode = new NodeT<T>(original->data);
            copy->next = newNode;
            copy = copy->next;
            original = original->next;
        }
    }
}

// copy constructor !!
template <class T>
QueueT<T>::QueueT(const QueueT &q)
{
    copy_queue(q); // deep copy constructor from helper funciton
}

// helper function for deleting queue
template <class T>
void QueueT<T>::delete_queue()
{
    NodeT<T> *temp = front;
    if (front != nullptr)
    {
        front = front->next;
        delete temp;
        temp = front;
    }
    current_size = 0;
}

// destructor !!
template <class T>
QueueT<T>::~QueueT()
{
    delete_queue(); // deleting queue from helper function
}

// overloaded assignment operator
template <class T>
QueueT<T> &QueueT<T>::operator=(const QueueT &q)
{
    if (this != &q)
    {
        delete_queue(); // deleting queue from helper function
        copy_queue(q);  // deep copy constructor from helper funciton
    }
    return *this;
}

// enqueue !!
template <class T>
void QueueT<T>::enqueue(T val)
{
    NodeT<T> *temp = new NodeT<T>(val);
    if (front == nullptr) // if the front is null, then the back and the node pointer is also null
    {
        front = back = temp;
    }
    else // if not, then it points to the next
    {
        back->next = temp;
        back = temp;
    }
    current_size++; // increases current size
}

// dequeue !!
template <class T>
T QueueT<T>::dequeue()
{
    if (empty()) // if the queue is empty, then throws runtime error
    {
        throw runtime_error("the queue is empty");
    }
    NodeT<T> *temp = front; // temp pointer to front of queue, then points to next data
    T result = front->data;
    front = front->next;
    delete temp;
    return result; // returns front of queue
}

// empty !!
template <class T>
bool QueueT<T>::empty() const
{
    if (front == nullptr) // if the front points to null, then return true, else return false
    {
        return true;
    }
    return false;
}

// size !!
template <class T>
int QueueT<T>::size() const
{
    return current_size; // returns current size of queue
}