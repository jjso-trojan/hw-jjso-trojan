#include "token.h"
#include "token_stack.h"
#include <cctype>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

// THROW ALL EXCEPTIONS EXACTLY AS GIVEN IN THE COMMENTS
// You need only uncomment them to throw the exceptions
// under the correct conditons
// You can check the conditions using an if statement or
// switch statement
// For any problems with the expression format as well as attempting
// division by zero, throw the exception invalid_argument("Malformed");

TokenStack tokenize(std::string expression) {
  TokenStack stack;
  stringstream stream(expression);

  // Loop while there are still characters in the stream.
  while (stream.rdbuf()->in_avail()) {
    stream >> ws;
    // TODO: push your tokens to the stack.
    int buf = stream.peek();
    if (isdigit(buf)) {
      stream >> buf;
      stack.push(Token(TokenKind::NUMBER, buf));
    } else {
      char c;
      stream >> c;
      if (c == '(') {
        stack.push(Token(TokenKind::LP));
      } else if (c == ')') {
        stack.push(Token(TokenKind::RP));
      } else if (c == '+') {
        stack.push(Token(TokenKind::PLUS));
      } else if (c == '-') {
        stack.push(Token(TokenKind::MINUS));
      } else if (c == '*') {
        stack.push(Token(TokenKind::TIMES));
      } else if (c == '/') {
        stack.push(Token(TokenKind::DIVIDE));
      } else {
        throw invalid_argument("Malformed");
      }
    }

    // If we find an invalid token, we can simply:
    // throw invalid_argument("Malformed");
  }

  // We pushed the tokens into the stack from left to right, which means
  // we'll pop them back out backwards. For convenience, we'll return the
  // reverse of our current stack so that we can pop the tokens out from
  // left to right, as you'd read the expression.
  stack.reverse();
  return stack;
}

// Reduce is called when we encounter closing parentheses. If the expression
// is validly formed, we can expect there to be either a binary operation on
// the work stack. This method should pop the tokens in that operation,
// compute the result, and push it back onto the work stack as a new token.
void reduce(TokenStack &work) {
  TokenStack immediate;

  // TODO: pop tokens off of work into immediate until we hit TokenKind::LP.
  TokenKind temp = TokenKind::INVALID;
  int firstNum;
  int secNum;
  while (!work.empty() && work.top().kind != TokenKind::LP) {
    immediate.push(work.top());
    work.pop();
  }
  if (!work.empty() && work.top().kind == TokenKind::LP) {
    work.pop();
  } else {
    throw invalid_argument("Malformed");
  }
  if (immediate.empty()) {
    throw invalid_argument("Malformed");
  }

  // TODO: check whether we have "-p" or "p @ q @ ..." and handle accordingly
  if (immediate.top().kind == TokenKind::MINUS) {
    immediate.pop();
    if (immediate.top().kind == TokenKind::NUMBER) {
      firstNum = immediate.top().value * (-1);
      immediate.pop();
      work.push(Token(TokenKind::NUMBER, firstNum));
    } else {
      throw invalid_argument("Malformed");
    }
    if (!immediate.empty()) {
      throw invalid_argument("Malformed");
    }
    return;
  }
  // TODO: if you are expecting a token but immediate is empty, throw:
  // throw invalid_argument("Malformed");

  while (!immediate.empty()) {
    if (immediate.top().kind == TokenKind::NUMBER) {
      firstNum = immediate.top().value;
      immediate.pop();
      if (immediate.empty()) {
        work.push(Token(TokenKind::NUMBER, firstNum));
        return;
      }
      if (immediate.top().kind == TokenKind::NUMBER) {
        throw invalid_argument("Malformed");
      }
      if (temp == TokenKind::INVALID) {
        temp = immediate.top().kind;
      } else if (temp != immediate.top().kind) {
        throw invalid_argument("Malformed");
      }
      temp = immediate.top().kind;
      immediate.pop();
      if (immediate.empty() || immediate.top().kind != TokenKind::NUMBER) {
        throw invalid_argument("Malformed");
      }
      secNum = immediate.top().value;
      immediate.pop();
      if (temp == TokenKind::PLUS) {
        firstNum += secNum;
      } else if (temp == TokenKind::DIVIDE) {
        if (secNum == 0) {
          throw invalid_argument("Malformed");
        }
        firstNum /= secNum;
      } else if (temp == TokenKind::MINUS) {
        firstNum -= secNum;
      } else {
        firstNum *= secNum;
      }
      Token nums = Token(TokenKind::NUMBER, firstNum);
      immediate.push(nums);
    } else {
      throw invalid_argument("Malformed");
    }
  }
  // TODO: if you are expecting a token of a different kind, throw:
  // throw invalid_argument("Malformed");
  // TODO: if you encounter a differing chained binary operation, throw:
  // throw invalid_argument("Malformed");

  // TODO: use integer math to do your computations.
  // TODO: if the operator is not of kind +, -, *, or /, throw:
  // throw invalid_argument("Malformed");
  // TODO: if dividing by zero, throw:
  // throw invalid_argument("Malformed");

  // TODO: push the result of either branch back onto work
}

int evaluate(std::string expression) {
  TokenStack tokens = tokenize(expression);
  TokenStack work;

  // TODO: while the stack is not empty, pop the top token.
  while (!tokens.empty()) {
    Token head = tokens.top();
    tokens.pop();
    // TODO: if the top token is of TokenKind::RP, reduce(work).
    if (head.kind == TokenKind::RP) {
      reduce(work);
      cout << work << endl;
    }
    // TODO: otherwise, push the top token onto work.
    else {
      work.push(head);
    }
  }
  // TODO: once done, check there is exactly one TokenKind::NUMBER token.
  if (!work.empty() && work.top().kind == TokenKind::NUMBER &&
      work.size() == 1) {
    return work.top().value;
  } else {
    throw invalid_argument("Malformed");
  }
  // TODO: if so, return its value, otherwise throw:
  // throw invalid_argument("Malformed");
}

// Please DO NOT CHANGE MAIN
int main(int argc, char **argv) {
  if (argc <= 1) {
    cerr << "Expect an expression..." << endl;
    return -1;
  }

  try {
    cout << evaluate(argv[1]) << endl;
  } catch (invalid_argument &e) {
    cerr << "error: " << e.what() << endl;
  }
  return 0;
}

// If you're interested in extending your parser to support things like order
// of operations, unary operators, and single values, feel free to ping me
// on Discord (noahbkim) once you've finished the assignment as is. This will
// not give you any extra credit or improve your score in any way; it's just
// something fun and cool.
