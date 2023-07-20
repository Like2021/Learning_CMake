#include <cmath>
#include <iostream>
#include <string>

#include "TutorialConfig.h"
#include "MathFunctions.h"

int main(int argc, char* argv[])
{
  std::cout << argc << std::endl;

  // report version
  std::cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
            << Tutorial_VERSION_MINOR << std::endl;
  std::cout << "Usage: " << argv[0] << " number" << std::endl;
  std::cout << "Compile time: " << COMPILE_TIME << std::endl;

  // convert input to double
  const double inputValue = std::stod(argv[1]);

  // calculate square root
  const double outputValue = sqrt(inputValue);
  std::cout << "The square root of " << inputValue
            << " is " << outputValue
            << std::endl;
  
  // myfunc result
  const double outputMy = myFunc(inputValue);
  std::cout << "my output: " << outputMy << std::endl;
  return 0;
  }