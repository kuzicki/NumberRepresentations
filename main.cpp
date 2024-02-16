#include "BinaryNumbers.h"
#include <iostream>

int main() {
    FloatingPoint fb1(10.1f);
    FloatingPoint fb2(21.3f);

    std::cout << fb1.ToString() << std::endl << fb2.ToString() << std::endl;  

    std::cout << (fb1 + fb2).Base10() << std::endl;


    FloatingPoint fp(-10.f);
    std::cout << fp.ToString() << ": " << fp.Base10() << std::endl;

    FloatingPoint fp1(0.90f);
    std::cout << fp1.ToString() << ": " << fp1.Base10() << std::endl;
    FloatingPoint fres = fp + fp1;
    std::cout << fres.ToString() << ": " << fres.Base10() << std::endl;

    return 0;
}
