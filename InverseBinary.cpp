#include "InverseBinary.h"

InverseBinary::InverseBinary(int number) : Binary(number) {
    if (isNegative()) {
        inverseBits();
    }
}

int InverseBinary::Base10() {
    if (isNegative()) {
        inverseBits();
        int result = Binary::Base10();
        inverseBits();
        return result;
    }
    return Binary::Base10();
}