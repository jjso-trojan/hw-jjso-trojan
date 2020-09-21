#include "token_list.h"
#include <stdexcept>

size_t TokenList::size() const { return this->_size; }

TokenList::TokenList(TokenList const &other) {

  for (size_t i = 0; i < other.size(); i++) {
    push_back(other.at(i));
  }
  // Done
}

TokenList &TokenList::operator=(TokenList const &other) {

  for (size_t i = 0; i < other.size(); i++) {
    push_back(other.at(i));
  }
  return *this;
  // Done
}

TokenList::~TokenList() {
  for (size_t i = 0; i < this->size(); i++) {
    remove(i);
  }
  // Done
}

// TODO: implement the TokenList methods.
void TokenList::push_back(Token value) {
  std::shared_ptr<Item> newI = std::make_shared<Item>(value);
  if (this->size() == 0) {
    head = newI;
  } else if (this->size() == 1) {
    head->next = newI;
    newI->previous = head;
    newI->next = head;
    head->previous = newI;
  } else {
    std::shared_ptr<Item> tail = head->previous;
    newI->previous = tail;
    tail->next = newI;
    newI->next = head;
    head->previous = newI;
  }
  _size++;
  // Done
}
void TokenList::insert(size_t index, Token value) {
  std::shared_ptr<Item> newI = std::make_shared<Item>(value);
  size_t n = _size++;
  if (index % n == 0) {
    head->previous = newI;
    newI->next = head;
    head = newI;
  } else if (index % n == (_size - 1)) {
    newI->next = head;
    newI->previous = head->previous;
    head->previous->next = newI;
    head->previous = newI;
  } else {
    std::shared_ptr<Item> temp = head;
    for (size_t i = 0; i < (index % n); i++) {
      temp = temp->next;
    }
    newI->next = temp->next;
    temp->next->previous = newI;
    temp->next = newI;
    newI->previous = temp;
  }
  // Done
}

void TokenList::remove(size_t index) {
  if (this->size() == 0) {
    return;
  }

  std::shared_ptr<Item> temp = head;
  size_t n = this->size();
  for (size_t i = 0; i < index % n; i++) {
    temp = temp->next;
  }

  if (index % n == 0) {
    if (temp->next) {
      temp->next->previous = nullptr;
    }
    head = temp->next;
  } else if (index % n == (_size - 1)) {
    temp->previous->next = head;
    head->previous = temp->previous;
  } else {
    temp->next->previous = temp->previous;
    temp->previous->next = temp->next;
  }
  _size--;
  temp.reset();
  // Done
}

void TokenList::reverse() {
  if (_size == 0 || _size == 1) {
    return;
  }

  std::shared_ptr<Item> cursor = head;
  for (size_t i = 0; i < (this->size()); i++) {
    std::shared_ptr<Item> temp = cursor->next;
    cursor->next = cursor->previous;
    cursor->previous = temp;
    cursor = cursor->next;
  }
  head = head->next;
}

Token &TokenList::at(size_t index) {
  if (this->size() == 0 || this->size() < index) {
    throw std::out_of_range("no items");
  }

  std::shared_ptr<Item> temp = head;

  size_t i = 0;
  while (i < index) {
    temp = temp->next;
    i++;
  }

  return temp->value;
}

const Token &TokenList::at(size_t index) const {
  if (this->size() == 0 || this->size() < index) {
    throw std::out_of_range("no items");
  }

  std::shared_ptr<Item> temp = head;

  size_t i = 0;
  while (i < index) {
    temp = temp->next;
    i++;
  }

  return temp->value;
}

std::ostream &operator<<(std::ostream &out, const TokenList &list) {
  out << "{";
  std::shared_ptr<TokenList::Item> cursor = list.head;
  if (cursor != nullptr) {
    for (size_t i = 0; i < list.size() - 1; i++) {
      out << cursor->value << ", ";
      cursor = cursor->next;
    }
    out << cursor->value;
  }
  return out << "}";
}
std::ostream &operator<<(std::ostream &out, TokenList &List) {
  out << "{";
  std::shared_ptr<TokenList::Item> cursor = List.head;
  if (cursor != nullptr) {
    for (size_t i = 0; i < List.size() - 1; i++) {
      out << cursor->value << ", ";
      cursor = cursor->next;
    }
    out << cursor->value;
  }
  return out << "}";
}
