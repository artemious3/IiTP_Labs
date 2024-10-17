#ifndef BINARY_OPERATION_H
#define BINARY_OPERATION_H

#include "Expression.h"
#include <cstdlib>

enum class OperationType { Add, Sub, Mult, Div };

class BinaryOperation : public Expression {
  Expression *left_ = nullptr;
  Expression *right_ = nullptr;
  OperationType operation_;

public:
  BinaryOperation(Expression *left, char op, Expression *right);

  double evaluate() override;
  
  ~BinaryOperation() override;
};

#endif