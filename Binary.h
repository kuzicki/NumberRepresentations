#pragma once
#include <string>
#include <array>

const struct ConstBinary {
    const int _size = 32;
    const int _signBit = 31;
} constBinary;

class Binary {
public:
    Binary();
    Binary(const Binary& other);
    Binary(int number);
    int Base10();
    std::string ToString() const;
    void shiftLeft(int offset);
    void shiftRight(int offset);
    void Abs();
    std::array<int, 32> bits;
protected:
    
    int operator[](int index) const { return bits[index]; }
    int& operator[](int index) { return bits[index]; }
    void empty();
    void addBits(const Binary& other);
    void inverseBits();
    void changeSign();
    void toPositiveSign();
    bool isNegative() const;
};