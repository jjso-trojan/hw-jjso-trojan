#include "token_stack.h"
#include <stdexcept>

// TODO: return the top value on the stack.
const Token &TokenStack::top() const {
  if (this->empty()) {
    throw std::out_of_range("empty stack");
  }

  return list.at(list.size() - 1);
  // TODO: complete function here
}

// TODO: implement the TokenStack methods.
size_t TokenStack::size() const { return list.size(); }

// TODO: check whether the stack is empty
bool TokenStack::empty() const { return (list.size() == 0); }

// TODO: push a new token on top of the stack.
void TokenStack::push(const Token &t) { list.push_back(t); }

// TODO: remove the top element on the stack.
// If the stack is empty, this function should do nothing
void TokenStack::pop() { list.remove(list.size() - 1); }

// TODO: reverse the contents of the stack.
void TokenStack::reverse() { list.reverse(); }

std::ostream &operator<<(std::ostream &out, const TokenStack &stack) {
  out << stack.list;
  return out;
}

std::ostream &operator<<(std::ostream &out, TokenStack &stack) {
  out << stack.list;
  return out;
}