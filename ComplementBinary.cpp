#include "ComplementBinary.h"

ComplementBinary::ComplementBinary(int number) : Binary(number) {
    if (isNegative()) {
        inverseBits();
        addOneBit();
    }
}

ComplementBinary::ComplementBinary() : Binary(0) { }

ComplementBinary::ComplementBinary(const Binary& other) : Binary(other) {
    if (isNegative()) {
        inverseBits();
        addOneBit();
    }
}

ComplementBinary::ComplementBinary(ComplementBinary& other) {
    for (int i = 0; i < 32; i++) {
        bits[i] = other.bits[i];
    }
}

ComplementBinary ComplementBinary::operator+(const ComplementBinary& other) {
    ComplementBinary result(*this);
    result.addBits(other);
    return result;
}

ComplementBinary ComplementBinary::operator-(ComplementBinary other) {
    ComplementBinary result = *this;
    other.changeSign();
    other.inverseBits();
    other.addOneBit();
    result.addBits(other);
    return result;
}

int ComplementBinary::Base10() {
    if (isNegative()) {
        inverseBits();
        addOneBit();
        int result = Binary::Base10();
        inverseBits();
        addOneBit();
        return result;
    }
    return Binary::Base10();
}

void ComplementBinary::shiftLeft(int offset) {
    if (offset > 0)
        Binary::shiftLeft(offset);
}

void ComplementBinary::addOneBit() {
    int carryNumber = 0;
    int sum = bits[0] + 1 + carryNumber;
    carryNumber = sum / 2;
    bits[0] = sum % 2;
    for (int i = 1; i < 32; i++) {
        if (carryNumber == 0)
            break;

        sum = bits[i] + carryNumber;
        carryNumber = sum / 2;
        bits[i] = sum % 2;
    }
}