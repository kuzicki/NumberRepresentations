#include "DirectBinary.h"

DirectBinary::DirectBinary(int number) : Binary(number) {}

DirectBinary::DirectBinary() : Binary(0) {}

DirectBinary::DirectBinary(const Binary& other) {
    for (int i = 0; i < 32; i++) {
        bits[i] = other.bits[i];
    }
}

DirectBinary DirectBinary::operator*(DirectBinary second) {
    DirectBinary result(0);
    DirectBinary multiplyingStage(0);
    DirectBinary first(*this);

    bool isNegativeResult = false;
    if (first.isNegative() || second.isNegative())
        isNegativeResult = true;

    first.toPositiveSign();
    second.toPositiveSign();

    for (int otherIndex = 0; otherIndex < 31; otherIndex++) {
        if (second.bits[otherIndex] == 0)
            continue;

        for (int i = 0; i + otherIndex < 31; i++) {
            multiplyingStage[i + otherIndex] =
                first.bits[i] * second.bits[otherIndex];
        }
        result.addBits(multiplyingStage);
        multiplyingStage.empty();
    }
    if (isNegativeResult)
        result.changeSign();

    return result;
}