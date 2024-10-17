#ifndef NUMBER_H
#define NUMBER_H

#include "Expression.h"

class Number : public Expression {
private:
  double val_;

public:
  Number(double val);
  double evaluate() override;
};

#endif