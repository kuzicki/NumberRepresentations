#include "FloatingPoint.h"
#include <iostream>

void FloatingPoint::getIntegerBits(int integerNumber, std::vector<int>& numberBinary) {
    while (integerNumber != 0) {
        if (integerNumber % 2 == 0)
            numberBinary.insert(numberBinary.begin(), 0);
        else
            numberBinary.insert(numberBinary.begin(), 1);
        integerNumber /= 2;
    }
}

void FloatingPoint::getFloatBits(float number, int& size, std::vector<int>& numberBinary) {
    float floatNumber = getFractionalPart(number);

    while (size < floating._maxFloatingsize && getFractionalPart(floatNumber) != 0) {
        floatNumber *= 2;
        if (floatNumber >= 1.f) {
            numberBinary.push_back(1);
            floatNumber -= 1.f;
        }
        else
            numberBinary.push_back(0);
        size++;
    }
}

FloatingPoint::FloatingPoint(float number) {
    std::vector<int> numberBinary;
    bool isNegativeNumber = number < 0.f ? true : false;
    if (isNegativeNumber)
        number *= -1;
    int size = 0;

    getFloatBits(number, size, numberBinary);

    getIntegerBits(number, numberBinary);

    constructBinaryRepresentation(numberBinary, size, isNegativeNumber);
}

void FloatingPoint::constructBinaryRepresentation(std::vector<int>& numberBinary, int size, bool isNegativeNumber) {
    int exponent = numberBinary.size() - size - 1;

    DirectBinary exponentPart(exponent + 127);
    FloatingPoint result;
    result.bits[floating._mantissaStart] = 0;
    int binaryCounter = 1;
    for (int i = floating._mantissaStart; i >= 0 && binaryCounter < numberBinary.size(); i--)
    {
        result.bits[i] = numberBinary[binaryCounter++];
    }
    for (int i = floating._mantissaSize; i < floating._size - 1; i++)
    {
        result.bits[i] = exponentPart.bits[i - floating._mantissaSize];
    }
    if (isNegativeNumber) {
        result.bits[floating._signBit] = 1;
    }
    *this = result;
}

FloatingPoint::FloatingPoint() : Binary(0) { }

FloatingPoint FloatingPoint::mantissaAddtion(FloatingPoint& first, FloatingPoint& second,
    ComplementBinary& firstMantissa, ComplementBinary& secondMantissa, int mantissaShift, bool& isNegativeResult, bool isGreater) {
    ComplementBinary mantissaResult;

    if (first.isNegative() == second.isNegative()) {
        isNegativeResult = first.isNegative();
        mantissaResult = firstMantissa + secondMantissa;
    }
    else {
        if (isGreater) {
            isNegativeResult = first.isNegative();
            mantissaResult = subtractMantissa(firstMantissa, secondMantissa);
        }
        else {
            isNegativeResult = second.isNegative();
            mantissaResult = subtractMantissa(secondMantissa, firstMantissa);
        }
    }

    return getFloatingPointResult(first, mantissaResult, mantissaShift, isNegativeResult);
}

FloatingPoint FloatingPoint::operator+(FloatingPoint second) {
    FloatingPoint first(*this);
    bool isFirstGreaterAbs = isGreaterAbs(first, second);
    bool isNegativeResult;

    int firstExponent = first.getExponentNumber();
    int secondExponent = second.getExponentNumber();

    ComplementBinary firstMantissa = first.getMantissaPart();
    ComplementBinary secondMantissa = second.getMantissaPart();

    int mantissaShift = 0;

    if (firstExponent > secondExponent) {
        mantissaShift = firstExponent - secondExponent;
        secondMantissa.shiftRight(mantissaShift);
        second.copyExponent(first);
    }
    else if (secondExponent > firstExponent) {
        mantissaShift = secondExponent - firstExponent;
        firstMantissa.shiftRight(mantissaShift);
        first.copyExponent(second);
    }

    return mantissaAddtion(first, second, firstMantissa, secondMantissa,
        mantissaShift, isNegativeResult, isFirstGreaterAbs);
}

float FloatingPoint::Base10() {
    bool isNegative = bits[floating._signBit] == 1 ? true : false;
    int exponent = getExponentNumber() - floating._exponentBias;
    ComplementBinary mantissaPart = getMantissaPart();
    if (exponent > 0) {
        mantissaPart.shiftLeft(exponent);
    }
    else if (exponent < 0) {
        mantissaPart.shiftRight(std::abs(exponent));
    }
    float result = getIntegerPart(mantissaPart) + getFloatingPart(mantissaPart);
    if (isNegative)
        result *= -1;

    return result;
}

float FloatingPoint::getFractionalPart(float floatNumber) {
    return floatNumber - floorf(floatNumber);
}

ComplementBinary FloatingPoint::getMantissaPart() {
    DirectBinary mantissaPart;
    for (int i = 0; i < floating._mantissaSize; i++) {
        mantissaPart.bits[i] = bits[i];
    }
    mantissaPart.bits[floating._mantissaSize] = 1;
    ComplementBinary result(mantissaPart);
    return result;
}

int FloatingPoint::getExponentNumber() {
    Binary exponentPart;
    for (int i = floating._mantissaSize; i < floating._signBit; i++) {
        exponentPart.bits[i - floating._mantissaSize] = bits[i];
    }
    return exponentPart.Base10();
}

ComplementBinary FloatingPoint::getExponentPart() {
    ComplementBinary exponentPart;
    for (int i = 0; i < floating._exponentSize; i++) {
        exponentPart.bits[i] = bits[floating._mantissaSize + i];
    }
    return exponentPart;
}

void FloatingPoint::copyExponent(const Binary& floatExponent) {
    for (int i = floating._mantissaSize; i < floating._signBit; i++) {
        bits[i] = floatExponent.bits[i];
    }
}

bool FloatingPoint::isGreaterAbs(const DirectBinary& first, const DirectBinary& second) {
    for (int i = 30; i >= 0; i--) {
        if (first.bits[i] == 1 && second.bits[i] == 0) {
            return true;
        }
        if (second.bits[i] == 1 && first.bits[i] == 0) {
            return false;
        }
    }
    return true;
}

ComplementBinary FloatingPoint::subtractMantissa(ComplementBinary& first, ComplementBinary& second) {
    int borrow = 0;
    ComplementBinary result;

    for (int i = 0; i < floating._size; i++) {
        int bit = first.bits[i] - second.bits[i] - borrow;
        borrow = bit < 0 ? 1 : 0;
        result.bits[i] = std::abs(bit) % 2;
    }
    return result;
}

FloatingPoint FloatingPoint::getFloatingPointResult(FloatingPoint oldNumber, ComplementBinary newMantissa, int mantissaShift, bool isNegativeResult) {
    ComplementBinary exponentPart = oldNumber.getExponentPart();
    for (int i = floating._signBit; i >= 0; i--) {
        if (newMantissa.bits[i] == 1) {
            newMantissa.bits[i--] = 0;

            int shiftBinary = i - floating._mantissaStart;

            if (shiftBinary > 0) {
                exponentPart = exponentPart + ComplementBinary(1);
                newMantissa.shiftRight(shiftBinary);
            }
            else if (shiftBinary < 0) {
                exponentPart = exponentPart - ComplementBinary(1);
                newMantissa.shiftLeft(std::abs(shiftBinary));
            }
            break;
        }
    }
    return constructAdditionResult(oldNumber, exponentPart, newMantissa, isNegativeResult);
}

FloatingPoint FloatingPoint::constructAdditionResult(FloatingPoint& oldNumber, ComplementBinary& exponentPart, ComplementBinary& newMantissa, bool isNegativeResult) {
    for (int i = 0; i < floating._mantissaSize; i++) {
        oldNumber.bits[i] = newMantissa.bits[i];
    }
    oldNumber.bits[floating._signBit] = newMantissa.bits[floating._signBit];
    for (int i = 0; i < floating._exponentSize; i++) {
        oldNumber.bits[floating._mantissaSize + i] = exponentPart.bits[i];
    }
    if (isNegativeResult)
        oldNumber.bits[floating._signBit] = 1;

    return oldNumber;
}

float FloatingPoint::getFloatingPart(const ComplementBinary& mantissa) {
    float result = 0;
    for (int i = floating._mantissaSize - 1; i >= 0; i--) {
        if (mantissa.bits[i] == 1)
            result += std::pow(2, i - floating._mantissaSize);
    }
    return result;
}

int FloatingPoint::getIntegerPart(const ComplementBinary& mantissa) {
    int result = 0;
    for (int i = floating._mantissaSize; i < floating._size; i++) {
        if (mantissa.bits[i] == 1)
            result += std::pow(2, i - floating._mantissaSize);
    }
    return result;
}