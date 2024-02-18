#include "Binary.h"

Binary::Binary() { bits.fill(0); }

Binary::Binary(const Binary& other) {
    for (int i = 0; i < constBinary._size; i++) {
        bits[i] = other.bits[i];
    }
}

Binary::Binary(int number) {
    if (!number) {
        bits.fill(0);
        return;
    }
    int signlessNumber;
    signlessNumber = number < 0 ? -number : number;
    int i = 0;

    while (signlessNumber > 0) {
        bits[i++] = signlessNumber & 1;
        signlessNumber >>= 1;
    }

    while (i < constBinary._signBit) {
        bits[i++] = 0;
    }

    bits[constBinary._signBit] = number < 0 ? 1 : 0;
}

std::string Binary::ToString() const {
    std::string str(constBinary._size, '\0');
    for (int i = 0; i < constBinary._size; i++) {
        str[constBinary._signBit - i] = (char)48 + bits[i];
    }
    return str;
}

int Binary::Base10() {
    int number = 0;
    for (int i = 30; i >= 0; i--) {
        number <<= 1;
        if (bits[i] == 1) {
            number |= 1;
        }
    }
    return bits[constBinary._signBit] == 1 ? -number : number;
}

void Binary::shiftLeft(int offset) {
    for (int i = constBinary._signBit; i >= offset; i--) {
        bits[i] = bits[i - offset];
    }
    for (int i = 0; i < offset; i++) {
        bits[i] = 0;
    }
}

void Binary::shiftRight(int offset) {
    for (int i = 0; i < constBinary._size - offset; i++) {
        bits[i] = bits[i + offset];
    }
    for (int i = constBinary._signBit; i > constBinary._size - offset; i--) {
        bits[i] = 0;
    }
}

void Binary::Abs() { bits[constBinary._signBit] = 0; }

void Binary::empty() { bits.fill(0); }

void Binary::addBits(const Binary& other) {
    int carryNumber = 0;
    for (int i = 0; i < constBinary._size; i++) {
        int sum = bits[i] + other.bits[i] + carryNumber;
        carryNumber = sum / 2;
        bits[i] = sum % 2;
    }
}

void Binary::inverseBits() {
    for (int i = 0; i < bits.size() - 1; i++) {
        bits[i] = bits[i] == 1 ? 0 : 1;
    }
}

void Binary::changeSign() { bits[constBinary._signBit] = !bits[constBinary._signBit]; }

void Binary::toPositiveSign() { bits[constBinary._signBit] = 0; }

bool Binary::isNegative() const { 
    return bits[constBinary._signBit] == 1; 
}
