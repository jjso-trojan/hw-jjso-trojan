#include "simple.h"
#include <memory>

using namespace std;

SimpleCharacterManager::SimpleCharacterManager() {
  fill(buffer, buffer + BUFFER_SIZE, '\0');
}

char *SimpleCharacterManager::alloc_chars(size_t size) {
  // TODO: your implementation here
  if (size + first_available_address > buffer + BUFFER_SIZE) {
    return nullptr;
  } else {
    char *temp = first_available_address;
    first_available_address += size;
    return temp;
  }
}

void SimpleCharacterManager::free_chars(char *address) {
  // TODO: your implementation here
  if (address != nullptr &&
      (address >= buffer && address < buffer + BUFFER_SIZE)) {
    fill(address, first_available_address, '\0');
  }
}
