#include "FixedDirectBinary.h"
#include <iostream>

FixedDirectBinary::FixedDirectBinary() : DirectBinary(0) {
    isScaled = false;
}

FixedDirectBinary::FixedDirectBinary(const Binary& other) : DirectBinary(other) {
    isScaled = false;
}

FixedDirectBinary::FixedDirectBinary(int number) : DirectBinary(number) {
    isScaled = false;
}

FixedDirectBinary& FixedDirectBinary::operator=(const FixedDirectBinary& other) {
    if (this == &other)
        return *this;
    isScaled = other.isScaled;
    for (int i = 0; i < 32; i++) {
        bits[i] = other.bits[i];
    }
    return *this;
}

FixedDirectBinary FixedDirectBinary::operator/(const FixedDirectBinary& other) {
    FixedDirectBinary quotient(*this);
    FixedDirectBinary divisor(other);
    bool isResultScaled = false;
    if (quotient.isScaled && other.isScaled) {
        isResultScaled = false;
    }
    if (!quotient.isScaled) {
        quotient = quotient * DirectBinary(_scalingFactor);
        isResultScaled = true;
    }

    bool isNegativeResult = true;
    if (quotient.isNegative() == divisor.isNegative())
        isNegativeResult = false;
    quotient.Abs();
    divisor.Abs();

    int significantDiff = quotient.getMostSignificantNumber() - divisor.getMostSignificantNumber();
    divisor.shiftLeft(significantDiff);
    
    return longDivision(quotient, divisor, significantDiff, 
                        isNegativeResult, isResultScaled);
}

FixedDirectBinary FixedDirectBinary::longDivision(FixedDirectBinary& quotient, const FixedDirectBinary& divisor, 
    int significantDiff, bool isNegativeResult, bool isResultScaled) {

    ComplementBinary minusDivisor(-divisor);
    FixedDirectBinary result;
    for (int i = 0; i <= significantDiff; i++) {
        if (quotient.isNegative()) {
            quotient.addBits(divisor);
        }
        else {
            quotient.addBits(minusDivisor);
        }

        if (quotient.isNegative()) {
            result.bits[significantDiff - i] = 0;
        }
        else {
            result.bits[significantDiff - i] = 1;
        }

        quotient.shiftLeft(1);
    }
    if (isNegativeResult)
        result.changeSign();
    result.isScaled = isResultScaled;
    return result;
}

FixedDirectBinary FixedDirectBinary::operator-() const {
    FixedDirectBinary result(0);
    for (int i = 0; i < 31; i++) {
        result.bits[i] = bits[i];
    }
    result.bits[31] = bits[31] == 0 ? 1 : 0;
    return result;
}

std::string FixedDirectBinary::Base10() {
    int intRepresentation = Binary::Base10();
    std::string result = std::to_string(intRepresentation);
    if (isScaled) {
        bool isNegative = false;
        if (result[0] == '-') {
            isNegative = true;
            result.erase(0, 1);
        }
        int zeroDiff = 6 - result.size();
        for (int i = 0; i < zeroDiff; i++) {
            result.insert(0, "0");
        }

        result.insert(result.size() - _decimalPoints, ".");
        if (isNegative) {
            result.insert(0, "-");
        }

    }
    return result;
}

int FixedDirectBinary::getMostSignificantNumber() const {
    int significantNumber = 0;
    for (int i = 30; i >= 0; i--) {
        if (bits[i] == 1) {
            significantNumber = i;
            break;
        }
    }

    return significantNumber;
}