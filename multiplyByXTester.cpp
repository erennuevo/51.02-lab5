#include <iostream>
using namespace std;
#include "IntArray.h"

extern void multiplyByX(IntArray *p, int x);

int main(void)
{
    IntArray array;
    array.size = 5;
    array.elements = new int[5]{1, 2, 3, 4, 5};

    cout << "Original array: " << endl;
    for (int i = 0; i < array.size; i++) {
        cout << array.elements[i] << " ";
    }

    multiplyByX(&array, 3);

    cout << endl << "Array after multiplying by 3: " << endl;
    for (int i = 0; i < array.size; i++) {
        cout << array.elements[i] << " ";
    }

    delete[] array.elements;
    return 0;
}
