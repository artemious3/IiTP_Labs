#include "../t3/Expression.h"
#include "../t3/BinaryOperation.h"
#include "../t3/Number.h"

#include <cstdint>
#include <iomanip>
#include <iostream>

bool check_equals(Expression const* ex1, Expression const* ex2) {
  // access the first [size_of_pointer] bytes of class
  // this is where pointer to vtable stored in most of compilers
  return *(uintptr_t*)(ex1) ==
         *(uintptr_t*)(ex2);
}

void test_check_equals() {
  Expression *num1 = new Number(3.14);
  Expression *num2 = new Number(2.71);

  Expression *bin1 = new BinaryOperation(new Number(1e9), '/', new Number(1e8));
  Expression *bin2 = new BinaryOperation(bin1, '+', num2);

  std::cout << "_________ TEST: Number & Number _________\n";
  std::cout << "RESULT:    " << std::boolalpha << check_equals(num1, num1) << '\n';
  std::cout << "EXPECTED : true\n";

  std::cout << "_________ TEST: BinaryOperation & BinaryOperation _________\n";
  std::cout << "RESULT:    " << std::boolalpha << check_equals(bin1, bin2) << '\n';
  std::cout << "EXPECTED : true\n";

  std::cout << "_________ TEST: Number & BinaryOperation _________\n";
  std::cout << "RESULT:    " << std::boolalpha << check_equals(num1, bin1) << '\n';
  std::cout << "EXPECTED : false\n";

  delete num1; num1 = nullptr;
  delete bin2; bin2 = nullptr;//bin2 is responsible for bin1 & num2

}


int main() {
  test_check_equals();
  return 0;
}
