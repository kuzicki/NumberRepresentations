#pragma once
#include "Binary.h"
#include "DirectBinary.h"
#include "ComplementBinary.h"
#include <vector>

const struct Constants {
    const int _size = 32;
    const int _mantissaSize = 23;
    const int _exponentSize = 8;
    const int _maxFloatingsize = 128;
    const int _signBit = 31;
    const int _exponentBias = 127;
    const int _mantissaStart = 22;
} floating;

class FloatingPoint : public Binary {
public:
    FloatingPoint(float number);
    FloatingPoint();
    FloatingPoint(const FloatingPoint& other) : Binary(other) { }
    FloatingPoint operator+(FloatingPoint second);
    float Base10();
private:
    int getIntegerPart(const ComplementBinary& mantissa);
    float getFloatingPart(const ComplementBinary& mantissa);
    FloatingPoint getFloatingPointResult(FloatingPoint oldNumber, ComplementBinary newMantissa, int mantissaShift, bool isNegativeResult);
    ComplementBinary subtractMantissa(ComplementBinary& first, ComplementBinary& second);
    static bool isGreaterAbs(const DirectBinary& first, const DirectBinary& second);
    void copyExponent(const Binary& floatExponent);
    ComplementBinary getExponentPart();
    int getExponentNumber();
    ComplementBinary getMantissaPart();
    float getFractionalPart(float floatNumber);
    void getIntegerBits(int integerNumber, std::vector<int>& numberBinary);
    void constructBinaryRepresentation(std::vector<int>& numberBinary, int size, bool isNegativeNumber);
    void getFloatBits(float number, int& size, std::vector<int>& numberBinary);
    FloatingPoint mantissaAddtion(FloatingPoint& first, FloatingPoint& second,
        ComplementBinary& firstMantissa, ComplementBinary& secondMantissa, int mantissaShift, bool& isNegativeResult, bool isGreater);
    FloatingPoint constructAdditionResult(FloatingPoint& oldNumber, ComplementBinary& exponentPart, ComplementBinary& newMantissa, bool isNegativeResult);
};