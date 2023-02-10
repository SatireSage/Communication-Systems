#pragma once
#include <iostream>
#include "QueueT.h"
#include <stack>

// qconcatenate
template <typename S>
QueueT<S> qconcatenate(QueueT<S> &q1, QueueT<S> &q2)
{
    QueueT<S> temp1(q1); // copy constructor
    QueueT<S> temp2(q2);
    QueueT<S> merged;

    // use dequeue to add to the new queue
    while (!temp1.empty())
        merged.enqueue(temp1.dequeue());
    while (!temp2.empty())
        merged.enqueue(temp2.dequeue());
    return merged;
}

// qmerge
template <typename S>
QueueT<S> qmerge(QueueT<S> &q1, QueueT<S> &q2)
{
    QueueT<S> temp1(q1); // copy constructor
    QueueT<S> temp2(q2);
    QueueT<S> merged;

    // use dequeue to add to the new queue but alternate
    while (!temp1.empty() && !temp2.empty())
    {
        merged.enqueue(temp1.dequeue());
        merged.enqueue(temp2.dequeue());
    }
    while (!temp1.empty())
        merged.enqueue(temp1.dequeue());
    while (!temp2.empty())
        merged.enqueue(temp2.dequeue());
    return merged;
}

// qreverse
template <typename S>
QueueT<S> qreverse(QueueT<S> &q)
{
    QueueT<S> temp1(q);
    QueueT<S> reversed;
    std::stack<S> tempStack;

    while (!temp1.empty())
        tempStack.push(temp1.dequeue());
    while (!tempStack.empty())
    {
        reversed.enqueue(tempStack.top());
        tempStack.pop();
    }
    return reversed;
}
