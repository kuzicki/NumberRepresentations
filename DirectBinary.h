#pragma once
#include "Binary.h"

class DirectBinary : public Binary {
public:
    DirectBinary(int number);
    DirectBinary();
    DirectBinary(const Binary& other);
    DirectBinary operator*(DirectBinary second);
};