
#include <iostream>
#include <mb_function/mb_function_code.hpp>
#include "mb_function/mb_request.hpp"

int main() {
  std::cout << "Example Modbus RTU Master" << std::endl;

  supermodbus::MBRequest request(
      1, supermodbus::MBFunctionCode(supermodbus::MBFunctionCode::kReadHR),
      {3001, 5});

  request.print();

  return 0;
}
