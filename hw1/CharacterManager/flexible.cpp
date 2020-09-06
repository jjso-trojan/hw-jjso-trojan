#include "flexible.h"
#include <ostream>

using namespace std;


// The constructor creates a unique_ptr to a dynamically allocated array of two
// CharacterBlocks using the initialization list syntax. No need to change this
// unless you add fields to the FlexibleCharacterManager.
FlexibleCharacterManager::FlexibleCharacterManager() : blocks(new CharacterBlock[2] {}) {}

ostream& operator<<(ostream& os, const FlexibleCharacterManager& mgr) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (mgr.buffer[i] == '\0') {
            os << char(248);  // prints 0 for null character
        } else {
            os << mgr.buffer[i];
        }
    }
    os << endl;
    os << "blocks in use = " << mgr.blocks_in_use << endl;
    return os;
}

char* FlexibleCharacterManager::alloc_chars(size_t n) {
	// step #1. first look for a big enough free slot
	size_t emptySlotNum = 0;   // index of the free block that is bigger or equal 'n'
	char* addr = buffer;       // start address of the free block
	size_t newEmptyBlockSize = 0;  // left-over space from the free block
	while (emptySlotNum < blocks_in_use) {
        if (blocks[emptySlotNum].address[0] == '\0' && n<=blocks[emptySlotNum].size) {
            addr = blocks[emptySlotNum].address;
            // if the current empty slot is bigger than n,
            // create another block for the rest of empty space
            if (n < blocks[emptySlotNum].size) {
                newEmptyBlockSize = blocks[emptySlotNum].size - n;
            }
            blocks[emptySlotNum].size = n;
            break;
        }
        emptySlotNum++;
	}

	// step #2. return nullptr early if there is no big enough free slot
	if (emptySlotNum == blocks_in_use) {
	    if (blocks_in_use > 0)
            addr = blocks[blocks_in_use-1].address + blocks[blocks_in_use-1].size;
        // If the size of the last free slot in use is not big enough,
        // there is no free space left in buffer.
        if (blocks_in_use > 0) {
            CharacterBlock lastBlock = blocks[blocks_in_use-1];
            if ((lastBlock.address+lastBlock.size+n) > (buffer+BUFFER_SIZE))
                return nullptr;
        } else if (n > BUFFER_SIZE) {
            return nullptr;
        }
	}

	// step #3. if there no block to use, create a bigger blocks array
	// and copy old blocks to new blocks.
	if (blocks_in_use ==  blocks_size) {
        CharacterBlock* oldBlocks = blocks.release();
        blocks_size *= 2;
        blocks = make_unique<CharacterBlock[]>(blocks_size);
        CharacterBlock* newBlocks = blocks.get();
        for (size_t i=0; i<blocks_in_use; i++) {
            newBlocks[i] = oldBlocks[i];
        }
	}

	// step #4. Now update the block with appropriate info addr and n
	if (emptySlotNum == blocks_in_use) {
        blocks[emptySlotNum] = {addr, n};
        blocks_in_use++;
	} else if (newEmptyBlockSize > 0) {
        // shift all slots from emptySlotNum+1 till blocks_in_use-1 to the right
        // by one slot to make a room to place the new block in the right order
        // Note that no need to check if there is a block to use at i+1
        // because we already increased the blocks_size at step #3.
        for (size_t i = blocks_in_use-1; i>emptySlotNum; i--) {
            blocks[i+1] = blocks[i];
        }
        blocks[emptySlotNum+1] =
            {blocks[emptySlotNum].address+blocks[emptySlotNum].size, newEmptyBlockSize};
        blocks_in_use++;
	}
    return addr;
}

void FlexibleCharacterManager::free_chars(char* p) {
    // step #0. Do nothing if p is not a valid address in range
    if (p == nullptr || p<buffer || p>=buffer+BUFFER_SIZE)
        return;

    // step #1. look for the block whose start address is 'p'
    // and fill its content with '\0'
    size_t i=0;
	while (i<blocks_in_use) {
        if (blocks[i].address == p) {
            // clear the content of block with '\0'
	        fill(p, p+blocks[i].size, '\0');
            break;
        }
        i++;
	}

	// step #2. If neighbor blocks are empty, merge them with the current block.
	if (i < blocks_in_use) {
        if (i > 1 && blocks[i-1].address[0] == '\0') {
            // if the i+1 block is empty, merge it with the current block
            if ((i+1) < blocks_in_use && blocks[i+1].address[0] == '\0') {
                blocks[i-1].size += blocks[i].size + blocks[i+1].size;
                // shift all blocks from i+2 to blocks_in_use-1 to the left
                // by two blocks and reduce blocks_in_use by 2.
                while ((i+2) < blocks_in_use) {
                    blocks[i] = blocks[i+2];
                    i++;
                }
                // note that blocks_in_use is decremented at the end of
                // this code segment. so decrease once only here.
                blocks_in_use -= 2;
            } else {
                blocks[i-1].size += blocks[i].size;
                // shift all blocks from i+1 to blocks_in_use-1 to the left
                // by one block and reduce blocks_in_use by 1.
                while ((i+1) < blocks_in_use) {
                    blocks[i] = blocks[i+1];
                    i++;
                }
                blocks_in_use--;
            }
        } else if ((i+1) < blocks_in_use && blocks[i+1].address[0] == '\0') {
            blocks[i].size += blocks[i+1].size;
            // shift all blocks from i+2 to blocks_in_use-1 to the left
            // by one block and reduce blocks_in_use by 1.
            i++;
            while ((i+1) < blocks_in_use) {
                blocks[i] = blocks[i+1];
                i++;
            }
            blocks_in_use--;
         }
    }

    // step #3. reduce 'blocks_size' half if blocks_in_use is less than half of it.
    if (blocks_in_use > 2 && blocks_in_use < (blocks_size / 2)) {
        blocks_size /= 2;
    }
}
