
#include <array>
#include <iostream>
#include <vector>
#include "mb_data_handlers.hpp"
#include "mb_function.hpp"
#include "mb_hello.hpp"
#include "mb_register.hpp"

int main() {
  std::cout << "Hello, world!" << std::endl;

  hello();

  MBRegister<double> regf{};
  MBRegister regi{true};

  regf.print();
  regf.print_type();
  regi.print();
  regi.print_type();

  std::vector<int> v_i{};
  v_i.emplace_back(1);
  v_i.emplace_back(22);
  std::array<float, 10> a_f{};
  a_f[5] = 1337.0F;

  handle_write(v_i.cbegin(), v_i.cend());
  handle_write(a_f.cbegin(), a_f.cend() - 1);

  std::array<uint8_t, 5> test_bytes{3, 0x1B, 0x59, 0x00, 0x10};
  auto test_req{supermodbus::parse_req_from_bytes(test_bytes.cbegin(),
                                                  test_bytes.cend())};

  if (test_req) {
    test_req->print();
  }

  return 0;
}
