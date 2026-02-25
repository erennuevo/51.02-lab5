#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

#include "IntArray.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <multiplier>" << endl;
        return 1;
    }

    int x = atoi(argv[1]);
    if (x <= 0) {
        cerr << "Error: multiplier must be a positive integer." << endl;
        return 1;
    }

    // build the function name, e.g. "multiplyByN" where N is an int
    string funcName = "multiplyBy" + to_string(x);
    // c++ name mangling: _Z<len><name>P8IntArray
    // (no trailing 'i' because x is hardcoded, not a parameter)
    string mangledName = "_Z" + to_string(funcName.size()) + funcName + "P8IntArray";

    cout << "\t.file\t\"" << funcName << ".cpp\"" << endl;
    cout << "\t.text" << endl;
    cout << "\t.globl\t" << mangledName << endl;
    cout << "\t.type\t" << mangledName << ", @function" << endl;
    cout << mangledName << ":" << endl;
    cout << ".LFB0:" << endl;
    cout << "\t.cfi_startproc" << endl;
    cout << "\tendbr64" << endl;

    // if (p->size <= 0) return
    cout << "\tcmpl\t$0, (%rdi)" << endl;
    cout << "\tjle\t.L1" << endl;

    // i = 0 in %eax
    cout << "\tmovl\t$0, %eax" << endl;

    cout << ".L3:" << endl;
    // load p->elements into %rdx
    cout << "\tmovq\t8(%rdi), %rdx" << endl;
    // compute address of elements[i]
    cout << "\tleaq\t(%rdx,%rax,4), %rdx" << endl;
    // multiply element by constant x (3-operand imull with immediate)
    cout << "\timull\t$" << x << ", (%rdx), %ecx" << endl;
    // store result back
    cout << "\tmovl\t%ecx, (%rdx)" << endl;
    // i++
    cout << "\taddq\t$1, %rax" << endl;
    // loop condition: if (i < p->size) goto .L3
    cout << "\tcmpl\t%eax, (%rdi)" << endl;
    cout << "\tjg\t.L3" << endl;

    cout << ".L1:" << endl;
    cout << "\tret" << endl;
    cout << "\t.cfi_endproc" << endl;
    cout << ".LFE0:" << endl;
    cout << "\t.size\t" << mangledName << ", .-" << mangledName << endl;
    cout << "\t.ident\t\"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0\"" << endl;
    cout << "\t.section\t.note.GNU-stack,\"\",@progbits" << endl;

    return 0;
}