#include "BinaryOperation.h"
#include <__config>
#include <cmath>
#include <stdexcept>

BinaryOperation::BinaryOperation(Expression *left, char op, Expression *right) {
  left_ = left;
  right_ = right;
  
  switch (op) {
  case '+':
    operation_ = OperationType::Add;
    break;
  case '-':
    operation_ = OperationType::Sub;
    break;
  case '*':
    operation_ = OperationType::Mult;
    break;
  case '/':
    operation_ = OperationType::Div;
    break;
  default:
    throw std::invalid_argument("Unknown operation type for BinaryOperation");
  }
}

double BinaryOperation::evaluate() {
  if (!left_ || !right_) {
    throw std::runtime_error("Subexpressions are nullptr");
  }

  double res = 0;

  switch (operation_) {
  case OperationType::Add:
    res = left_->evaluate() + right_->evaluate();
    break;
  case OperationType::Sub:
    res = left_->evaluate() - right_->evaluate();
    break;
  case OperationType::Mult:
    res = left_->evaluate() * right_->evaluate();
    break;
  case OperationType::Div:
    res = left_->evaluate() / right_->evaluate();
    break;
  }

  return res;
}

BinaryOperation::~BinaryOperation() {
  if (left_) {
    delete left_;
    left_ = nullptr;
  }
  if (right_) {
    delete right_;
    right_ = nullptr;
  }
}