#pragma once
#include "Binary.h"

class ComplementBinary : public Binary {
public:
    ComplementBinary(int number);
    ComplementBinary();
    ComplementBinary(const Binary& other);
    ComplementBinary(ComplementBinary& other);
    ComplementBinary operator+(const ComplementBinary& other) const;
    ComplementBinary operator-(ComplementBinary other) const;
    int Base10();
    void shiftLeft(int offset);
private:
    void addOneBit();
};