/**
 * @name    Garbage Collector implementation
 * @author  Egor Bronnikov
 * @edited  13-03-2022
 */



#include <stdio.h>
#include <stdlib.h>

#include "gc.h"             //< Garbage Collector and Virtual Machine header



/* @brief   Objects on stack are preserved */
void test1(void) {
  printf("Test 1: Objects on stack are preserved.\n");
  VM* vm = newVM();
  pushInt(vm, 1);
  pushInt(vm, 2);

  gc(vm);
  assert(vm->numObjects == 2, "Should have preserved objects.");
  freeVM(vm);
}



/* @brief   Unreached objects are collected */
void test2(void) {
  printf("Test 2: Unreached objects are collected.\n");
  VM* vm = newVM();
  pushInt(vm, 1);
  pushInt(vm, 2);
  pop(vm);

  gc(vm);
  assert(vm->numObjects == 0, "Should have collected objects.");
  freeVM(vm);
}



/* @brief   Reach nested objects */
void test3(void) {
  printf("Test 3: Reach nested objects.\n");
  VM* vm = newVM();
  pushInt(vm, 1);
  pushInt(vm, 2);
  pushPair(vm);
  pushInt(vm, 3);
  pushInt(vm, 4);
  pushPair(vm);
  pushPair(vm);

  gc(vm);
  assert(vm->numObjects == 7, "Should have reached objects.");
  freeVM(vm);
}



/* @brief   Performance test */
void perfTest(void) {
    printf("Performance Test.\n");
    VM* vm = newVM();

    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < 20; j++) {
            pushInt(vm, i);
        }

        for (int k = 0; k < 20; k++) {
            pop(vm);
        }
    }
    freeVM(vm);
}



int main(void) {
    /* perfTest(); */
    test3();
    return 0;
}
