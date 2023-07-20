#include "MathFunctions.h"

double myFunc(double inputValue)
{
    std::cout << "input: " << inputValue++ << std::endl;
    return ++inputValue;
}