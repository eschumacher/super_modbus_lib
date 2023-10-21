
#pragma once

#include <iostream>

template <typename Iter>
void handle_write(Iter iter, Iter end) {
  for (; iter != end; ++iter) {
    std::cout << *iter << std::endl;
  }
}
