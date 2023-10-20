#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#include <malloc.h>
#include <stdio.h>
#include <debug.h>
#include <unistd.h>

typedef struct block {
  size_t size;
  struct block *next;
  int free;
  int debug;

} block_t;

block_t *head = NULL;

#define BLOCK_SIZE sizeof(block_t)

void *findFreeSpace(block_t* block, size_t s);


void *findFreeSpace(block_t* block, size_t s) {
  if (block->free && block->size >= s) {
    return block + 1;
  }

  if (block->next == NULL) {
        block_t *newBlockPtr = NULL;

        newBlockPtr = sbrk(0);
        newBlockPtr = sbrk(BLOCK_SIZE + s);

        if (newBlockPtr == (block_t *) -1) {
                debug_printf("Failed to free memory\n");
                return NULL;
        }

        newBlockPtr->size = s;
        newBlockPtr->next = NULL;
        newBlockPtr->free = 0;

        block->next = newBlockPtr;
        return newBlockPtr + 1;
  }

  return findFreeSpace(block->next , s);
}



void *mymalloc(size_t s) {
  block_t *blockPtr = head;
  void *p;

  if (blockPtr == NULL) {
    blockPtr = sbrk(0);
    blockPtr = sbrk(BLOCK_SIZE + s);

	if (blockPtr == (void*) -1){
	  debug_printf("sbrk failed to allocate memory in mymalloc.\n");
	  return p;
	}
    
	blockPtr->next = NULL;
 	blockPtr->size = s;
	blockPtr->free = 0;
	
	head = blockPtr;
	return ++head;
  }

  debug_printf("malloc %zu bytes\n", s);
  return findFreeSpace(head, s);
}

void *mycalloc(size_t nmemb, size_t s) {

  int *p = (void *) mymalloc(nmemb * s); // list length * element data struct size
  int memoryTable = (nmemb * s) / sizeof(int);

  int i;
  for (i = 0; i < memoryTable; ++i){
    p[i] = 0;
  }

  debug_printf("calloc %zu bytes\n", s);
  return (void *) p;
}

void myfree(void *ptr) {
  debug_printf("Freed some memory\n");

  // Replace the free below with your own free implementation
  block_t *blockPtr = (block_t *) ptr - 1;
  blockPtr->free = 1;
}


