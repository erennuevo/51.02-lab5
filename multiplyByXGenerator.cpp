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
   cout << "\tmovq\t8(%rdi), %rdx" << endl; // store first argument at address rdi + 8 into general register
   // compute address of elements[i]
   cout << "\tleaq\t(%rdx,%rax,4), %rdx" << endl; // (rdx = rdx + rax * 4) rax = return value

   /* ORIGINAL CODE WITH IMULL */
   // multiply element by constant x (3-operand imull with immediate)
   // cout << "\timull\t$" << x << ", (%rdx), %ecx" << endl;
   // store result back
   // cout << "\tmovl\t%ecx, (%rdx)" << endl;

    /* ===================== PART C: NO MULTIPLY INSTRUCTION =====================

    Yes, there exists a general algorithm to replace multiplication by a constant
    without using any hardware multiply instructions (such as imul, mul, or fmul).
    This algorithm is based on binary decomposition of the constant and is commonly
    referred to as shift-and-add multiplication or strength reduction.

    Any positive integer constant can be represented as a sum of powers of two.
    Since multiplication by a power of two can be implemented using a left shift,
    multiplication by an arbitrary constant can be performed using only shift and
    addition instructions.

    Example (multiplyBy61):
        61 = 32 + 16 + 8 + 4 + 1
        x * 61 = (x << 5) + (x << 4) + (x << 3) + (x << 2) + x

    In this generator program, the user-provided constant is decomposed into its
    binary representation. The code iterates through each bit of the constant,
    accumulates shifted values of the original element when a bit is set, and
    shifts the working value for the next bit position.

    As a result, the generated assembly code uses only: shl (shift left), add, and mov
    and does not generate any multiply instructions.

    This technique simulates how older processors without hardware multiplication
    support performed arithmetic and is still used by modern compilers as an
    optimization strategy.

    Sources:
    Stack Overflow. (2010, May 5). How can I multiply and divide using only bit shifting and adding? 
    Retrieved February 24, 2026, from 
    https://stackoverflow.com/questions/2776211/how-can-i-multiply-and-divide-using-only-bit-shifting-and-adding

    Spiral Project. (n.d.). Multiplierless constant multiplication. 
    Retrieved February 24, 2026, from http://www.spiral.net/hardware/multless.html
     ============================================================================ */

   // load element into %r8d (original value)
    cout << "\tmovl\t(%rdx), %r8d" << endl;

    // accumulator = 0  (result)
    cout << "\txorl\t%r9d, %r9d" << endl; // xor with itself = 0

    // r10d = shifted working value (start = original element)
    cout << "\tmovl\t%r8d, %r10d" << endl;

    int temp = x;
    while (temp > 0) {

        if (temp & 1) { // checks if the current bit is = 1
            // add current shifted value
            cout << "\taddl\t%r10d, %r9d" << endl;
        }

        temp >>= 1; // divide temp by 2 to go to the next bit

        if (temp > 0) {
            // shift once for next bit
            cout << "\tshll\t$1, %r10d" << endl;
        }
    }

    // store result back
    cout << "\tmovl\t%r9d, (%rdx)" << endl;

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
