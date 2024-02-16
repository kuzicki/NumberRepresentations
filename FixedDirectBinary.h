#pragma once
#include "DirectBinary.h"
#include "ComplementBinary.h"

class FixedDirectBinary : public DirectBinary {
public:
    FixedDirectBinary();
    FixedDirectBinary(const Binary& other);
    FixedDirectBinary(int number);
    FixedDirectBinary& operator=(const FixedDirectBinary& other);
    FixedDirectBinary operator/(const FixedDirectBinary& other);
    FixedDirectBinary operator-() const;
    std::string Base10();
private:
    int getMostSignificantNumber() const;
    FixedDirectBinary longDivision(FixedDirectBinary& quotient, const FixedDirectBinary& divisor,
        int significantDiff, bool isNegativeResult, bool isResultScaled);

    const int _scalingFactor = 100000;
    const int _decimalPoints = 5;
    bool isScaled;
};