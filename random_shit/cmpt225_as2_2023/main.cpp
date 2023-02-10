#include "QueueT.h"
#include "QFunctions.h"
#include <iostream>
#include <string>

using namespace std;

void simpleTest()
{
    cout << "SIMPLE TEST" << endl;
    QueueT<int> q1;

    q1.enqueue(2);
    q1.enqueue(4);
    q1.enqueue(6);

    cout << "empty: 0 = " << q1.empty() << endl;
    cout << "print: {2,4,6} = ";
    cout << q1 << endl;
    cout << "size: 3 = " << q1.size() << endl;

    QueueT<int> q2(q1);
    QueueT<int> q3;
    q3.enqueue(42);
    q3 = q1;

    q1.dequeue();

    cout << "q1 post copy: {4,6} = " << q1 << endl;
    cout << "q2 (copy): {2,4,6} = " << q2 << endl;
    cout << "q3 (op=): {2,4,6} = " << q3 << endl;
}

void qconcatenateTest()
{
    // test qconcatenate
    cout << "QCONCATENATE TEST" << endl;
    QueueT<int> q1;
    QueueT<int> q2;
    QueueT<int> q3;

    q1.enqueue(2);
    q1.enqueue(4);
    q1.enqueue(6);

    q2.enqueue(8);
    q2.enqueue(10);
    q2.enqueue(12);

    q3 = qconcatenate(q1, q2);

    cout << "q1: " << q1 << endl;
    cout << "q2: " << q2 << endl;
    cout << "q3: " << q3 << endl;
}

void qconcatenateTest2()
{
    // test qconcatenate
    cout << "QCONCATENATE TEST2" << endl;
    QueueT<int> q1;
    QueueT<int> q2;
    QueueT<int> q3;

    q1.enqueue(1);
    q1.enqueue(4);
    q1.enqueue(1);

    q2.enqueue(7);
    q2.enqueue(6);
    q2.enqueue(8);

    q3 = qconcatenate(q1, q2);

    cout << "q1: " << q1 << endl;
    cout << "q2: " << q2 << endl;
    cout << "q3: " << q3 << endl;
}

void qmergeTest()
{
    // test qmerge
    cout << "QMERGE TEST" << endl;
    QueueT<int> q1;
    QueueT<int> q2;
    QueueT<int> q3;

    q1.enqueue(2);
    q1.enqueue(4);
    q1.enqueue(6);

    q2.enqueue(8);
    q2.enqueue(10);
    q2.enqueue(12);

    q3 = qmerge(q1, q2);

    cout << "q1: " << q1 << endl;
    cout << "q2: " << q2 << endl;
    cout << "q3: " << q3 << endl;
}

void qmergeTest2()
{
    // test qmerge
    cout << "QMERGE TEST2" << endl;
    QueueT<char> q1;
    QueueT<char> q2;
    QueueT<char> q3;

    q1.enqueue('a');
    q1.enqueue('b');
    q1.enqueue('c');
    q1.enqueue('d');
    q1.enqueue('e');

    q2.enqueue('r');
    q2.enqueue('s');
    q2.enqueue('t');

    q3 = qmerge(q1, q2);

    cout << "q1: " << q1 << endl;
    cout << "q2:  " << q2 << endl;
    cout << "q3: " << q3 << endl;
}

void qreverseTest()
{
    // test qreverse
    cout << "QREVERSE TEST" << endl;
    QueueT<int> q1;
    QueueT<int> q2;

    q1.enqueue(2);
    q1.enqueue(4);
    q1.enqueue(6);

    q2 = qreverse(q1);

    cout << "q1: " << q1 << endl;
    cout << "q2: " << q2 << endl;
}

void qreverseTest2()
{
    // test qreverse
    cout << "QREVERSE TEST2" << endl;
    QueueT<string> q1;
    QueueT<string> q2;

    q1.enqueue("egg");
    q1.enqueue("dog");
    q1.enqueue("duck");
    q1.enqueue("cat");

    q2 = qreverse(q1);

    cout << "q1: " << q1 << endl;
    cout << "q2: " << q2 << endl;
}

int main()
{
    simpleTest();
    cout << "----------------------------------------" << endl;
    qconcatenateTest();
    cout << "----------------------------------------" << endl;
    qconcatenateTest2();
    cout << "----------------------------------------" << endl;
    qmergeTest();
    cout << "----------------------------------------" << endl;
    qmergeTest2();
    cout << "----------------------------------------" << endl;
    qreverseTest();
    cout << "----------------------------------------" << endl;
    qreverseTest2();
    cout << "----------------------------------------" << endl;
    return 0;
}