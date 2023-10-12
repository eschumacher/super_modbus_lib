
#include <iostream>
#include <typeinfo>

template <typename Datatype>
class MBRegister {
public:

    explicit MBRegister(Datatype data)
        :data_{data} {}

    MBRegister() = default;

    void print() const { std::cout << "Data: " << data_ << std::endl; }
    void print_type() const { std::cout << "Type: " << typeid(data_).name() << std::endl; }

private:
    Datatype data_{};
};