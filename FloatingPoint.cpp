#include "FloatingPoint.h"
#include <iostream>

void FloatingPoint::getIntegerBits(int integerNumber, std::vector<int> &numberBinary) {
    while (integerNumber != 0) {
        if (integerNumber % 2 == 0)
            numberBinary.insert(numberBinary.begin(), 0);
        else
            numberBinary.insert(numberBinary.begin(), 1);
        integerNumber /= 2;
    }
}

void FloatingPoint::getFloatBits(float number, int& size, std::vector<int> &numberBinary) {
    float floatNumber = getFractionalPart(number);

    while (size < 128 && getFractionalPart(floatNumber) != 0) {
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
    result.bits[22] = 0;
    int binaryCounter = 1;
    for (int i = 22; i >= 0 && binaryCounter < numberBinary.size(); i--)
    {
        result.bits[i] = numberBinary[binaryCounter++];
    }
    for (int i = 23; i < 31; i++)
    {
        result.bits[i] = exponentPart.bits[i - 23];
    }
    if (isNegativeNumber) {
        result.bits[31] = 1;
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
    bool isNegative = bits[31] == 1 ? true : false;
    int exponent = getExponentNumber() - 127;
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
    for (int i = 0; i < 23; i++) {
        mantissaPart.bits[i] = bits[i];
    }
    mantissaPart.bits[23] = 1;
    ComplementBinary result(mantissaPart);
    return result;
}

int FloatingPoint::getExponentNumber() {
    Binary exponentPart;
    for (int i = 23; i < 31; i++) {
        exponentPart.bits[i - 23] = bits[i];
    }
    return exponentPart.Base10();
}

ComplementBinary FloatingPoint::getExponentPart() {
    ComplementBinary exponentPart;
    for (int i = 0; i < 8; i++) {
        exponentPart.bits[i] = bits[23 + i];
    }
    return exponentPart;
}

void FloatingPoint::copyExponent(const Binary& floatExponent) {
    for (int i = 23; i < 31; i++) {
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

    for (int i = 0; i < 32; i++) {
        int bit = first.bits[i] - second.bits[i] - borrow;
        borrow = bit < 0 ? 1 : 0;
        result.bits[i] = std::abs(bit) % 2;
    }
    return result;
}

FloatingPoint FloatingPoint::getFloatingPointResult(FloatingPoint oldNumber, ComplementBinary newMantissa, int mantissaShift, bool isNegativeResult) {
    ComplementBinary exponentPart = oldNumber.getExponentPart();
    for (int i = 31; i >= 0; i--) {
        if (newMantissa.bits[i] == 1) {
            newMantissa.bits[i--] = 0;

            int shiftBinary = i - 22;

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
    for (int i = 0; i < 23; i++) {
        oldNumber.bits[i] = newMantissa.bits[i];
    }
    oldNumber.bits[31] = newMantissa.bits[31];
    for (int i = 0; i < 8; i++) {
        oldNumber.bits[23 + i] = exponentPart.bits[i];
    }
    if (isNegativeResult)
        oldNumber.bits[31] = 1;

    return oldNumber;
}

float FloatingPoint::getFloatingPart(const ComplementBinary& mantissa) {
    float result = 0;
    for (int i = 22; i >= 0; i--) {
        if (mantissa.bits[i] == 1)
            result += std::pow(2, i - 23);
    }
    return result;
}

int FloatingPoint::getIntegerPart(const ComplementBinary& mantissa) {
    int result = 0;
    for (int i = 23; i < 32; i++) {
        if (mantissa.bits[i] == 1)
            result += std::pow(2, i - 23);
    }
    return result;
}