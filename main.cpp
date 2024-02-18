#include "BinaryNumbers.h"
#include <iostream>

int main() {
    ComplementBinary ct1(10);
    ComplementBinary ct2(-13);
    ComplementBinary res1 = ct1 + ct2;
    std::cout << "10 + (-13)" << std::endl;
    std::cout << res1.Base10() << ": " << res1.ToString() << std::endl;

    DirectBinary db1(10);
    DirectBinary db2(13);
    DirectBinary res2 = db1 * db2;
    std::cout << "10 * 13" << std::endl;
    std::cout << res2.Base10() << ": " << res2.ToString() << std::endl;

    FixedDirectBinary fx1(13);
    FixedDirectBinary fx2(8);
    FixedDirectBinary res3 = fx1 / fx2;
    std::cout << "13 / 8" << std::endl;
    std::cout << res3.Base10() << ": " << res3.ToString() << std::endl;

    return 0;
}