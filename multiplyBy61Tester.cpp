#include <iostream>
using namespace std;
#include "IntArray.h"

extern void multiplyBy61(IntArray *p);

int main()
{
    IntArray array;
    array.size = 5;
    array.elements = new int[5]{1, 2, 3, 4, 5};

    cout << "Original array: " << endl;
    for (int i = 0; i < array.size; i++) {
        cout << array.elements[i] << " ";
    }

    multiplyBy61(&array);

    cout << endl << "Array after multiplying by 61: " << endl;
    for (int i = 0; i < array.size; i++) {
        cout << array.elements[i] << " ";
    }
    cout << endl;

    delete[] array.elements;
    return 0;
}