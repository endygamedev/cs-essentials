/**
 * @header  GARBAGE COLLECTOR
 * @brief   This file contains all the necessary functions and data structures to implement the garbage collector.
 */



#ifndef _STDLIB_H
#include <stdlib.h>
#endif



#define STACK_MAX 256           //< Maximum stack size
#define INIT_OBJ_NUM_MAX 4      //< Initinal number of collected objects



/**
 * @fn      void assert(int condition, const char* message)
 * @brief   If a condition is not met, a message is displayed
 *          on the screen and the program aborts with exit code 1.
 *
 * @param   condition   The result of logical expression.
 * @param   message     The message that will be printed.
 * @return  None
 */
void assert(int condition, const char* message) {
    if(!condition) {
        printf("%s\n", message);
        exit(1);
    }
}



/* @brief   We got two types of objects: ints and pairs */
typedef enum {
    OBJ_INT,
    OBJ_PAIR
} ObjectType;



/* @brief   The data sctucture of the object that we will use in the future. */
typedef struct sObject {
    ObjectType type;                    //< object type: int or pairs

    union {
        //! OBJ_INT
        int value;
        
        //! OBJ_PAIL
        struct {
            struct sObject* head;       //< first element in the pair
            struct sObject* tail;       //< second element in the pair
        };
    };

    struct sObject* next;               //< the next object in the linked list of heap allocated objects

    unsigned char marked;               //< is the object in use or can it be discarded from the memory area
} Object;



/* @brief   Virtual Machine data structure. */
typedef struct {
    int numObjects;                     //< the total number of currently allocated objects
    int maxObjects;                     //< the number of objects required to trigger a GC
    Object* firstObject;                //< the first object in the linked list of all objects on the heap
    Object* stack[STACK_MAX];           //< all allocated objects
    int stackSize;                      //< the virtual machine stack size
} VM;



/**
 * @fn      VM* newVM(void)
 * @brief   Initialize new virtual machine.
 *
 * @return  New virtual machine structure.
 */
VM* newVM(void) {
    VM* vm = malloc(sizeof(VM));        //< allocate memory for VM
    vm->stackSize = 0;                  //< for a start stack size is equal zero
    vm->firstObject = NULL;             //< the first object in the linked list is `NULL`, because we have not objects yet
    vm->numObjects = 0;                 //< the number of objects is zero
    vm->maxObjects = INIT_OBJ_NUM_MAX;  //< and maximum size of objetcs is `INIT_OBJ_NUM_MAX` value
    return vm;
}



/**
 * @fn      void push(VM* vm, Objects* value)
 * @brief   Append element into the stack in virtual machine.
 *
 * @param   vm      Current virtual machine which keeps objects.
 * @param   value   The value that will be added to the stack.
 * @return  None
 */
void push(VM* vm, Object* value) {
    assert(vm->stackSize < STACK_MAX, "Stack is overflow!");
    vm->stack[vm->stackSize++] = value;
}



/**
 * @fn      Object* pop(VM* vm)
 * @brief   Returns the last object added to the stack.
 *
 * @param   vm  Current virtual machine which keeps objects.
 * @return      The last object added to the stack.
 */
Object* pop(VM* vm) {
    assert(vm->stackSize > 0, "Stack underflow!");
    return vm->stack[--vm->stackSize];
} 



/**
 * @fn      Object* newObject(VM* vm, ObjectType type)
 * @brief   Creates a new object.
 *
 * @param   vm      Current virtual machine which keeps objects.
 * @param   type    The type of object (int or pair).
 * @return          New object.
 */
Object* newObject(VM* vm, ObjectType type) {
    //! If we reach the maximum available number of objects in VM, start garbage collector.
    if (vm->numObjects == vm->maxObjects) gc(vm);

    Object* object = malloc(sizeof(Object));        //< allocate memory for new object
    object->type = type;                            //< obeject type
    object->marked = 0;                             //< by default new object is unreachable

    //! Prepend new object in linked list.
    object->next = vm->firstObject;
    vm->firstObject = object;
    vm->numObjects++;

    return object;
}



/**
 * @fn      void pushInt(VM* vm, int intValue)
 * @brief   Append a new object with `OBJ_INT` type into the stack.
 *
 * @param   vm          Current virtual machine which keeps objects.
 * @param   intValue    The int value of a new object.
 * @return  None
 */
void pushInt(VM* vm, int intValue) {
    Object* object = newObject(vm, OBJ_INT);
    object->value = intValue;
    objectPrint(object);
    push(vm, object);
    printf("\nCollected %d objects.\n", vm->numObjects);
}



/**
 * @fn      void pushPair(VM* vm)
 * @brief   Append a new object with `OBJ_PAIR` type into the stack.
 *
 * @param   vm      Current virtual machine which keeps objects.
 * @return  None
 */
void pushPair(VM* vm) {
    Object* object = newObject(vm, OBJ_PAIR);       //< create a new object
    object->tail = pop(vm);                         //< get the last element from stack and set the first element in pair
    object->head = pop(vm);                         //< get residual last element from stack and set the second element in pair
    objectPrint(object);
    push(vm, object);                               //< push new object into the stack
    printf("\nCollected %d objects.\n", vm->numObjects);
}



/**
 * @fn      void markAll(VM* vm)
 * @brief   Mark all reachable object as 1.
 *
 * @param   vm      Current virtual machine which keeps objects.
 * @return  None
 */
void markAll(VM* vm) {
    for (int i = 0; i < vm->stackSize; i++) {
        mark(vm->stack[i]);
    }
}



/**
 * @fn      void mark(Object* object)
 * @brief   Mark object as reachable.
 *
 * @param   object  Current object.
 * @return  None
 */
void mark(Object* object) {
    //! In order to avoid infinite cycle and runtime error,
    //! it is necessary to avoid already maked objects.
    if (object->marked) return;

    object->marked = 1;

    if (object->type == OBJ_PAIR) {
        mark(object->head);
        mark(object->tail);
    }
}



/**
 * @fn      void sweep(VM* vm)
 * @brief   Delete all unreachable (unused) objects. 
 *
 * @param   vm      Current virtual machine which keeps objects.
 * @return  None
 */
void sweep(VM* vm) {
    Object** object = &vm->firstObject;     //< get the first (last) element in linked list
    while (*object) {                       //< while we have objects do ...
        if (!(*object)->marked) {           //< if the object is unmarked do ...
            Object* unreached = *object;    //< set this object is unreachable
            *object = unreached->next;      //< go to the next object from linked list
            free(unreached);                //< free all memory used by this object
        } else {
            (*object)->marked = 0;
            object = &(*object)->next;      //< to to the next object from linked list
        }
    }
}



/**
 * @fn      void gc(VM* vm)
 * @brief   Run mark-and-sweep garbage collector. 
 *
 * @param   vm      Current virtual machine which keeps objects.
 * @return  None
 */
void gc(VM* vm) {
    int numObjects = vm->numObjects;

    //! Mark all objects.
    markAll(vm);

    //! Sweep unused objects.
    sweep(vm);

    //! If we have reached the maximum of available object then double this space.
    vm->maxObjects = vm->numObjects == 0 ? INIT_OBJ_NUM_MAX : vm->numObjects * 2;

    printf("Collected %d objects, %d remaning.\n", numObjects - vm->numObjects, vm->numObjects);
}



/**
 * @fn      void freeVM(VM* vm)
 * @brief   Close virtual machine and free all memory.
 * 
 * @param   vm      Current virtual machine which keeps objects.
 * @return  None
 */
void freeVM(VM* vm) {
    vm->stackSize = 0;
    gc(vm);
    free(vm);
}



/**
 * @fn      void objectPrint(Object* object)
 * @brief   Print the object's value.
 *
 * @param   object  Current object.
 * @return  None
 */
void objectPrint(Object* object) {
    switch(object->type) {
        case OBJ_INT:
            printf("%d", object->value);
            break;

        case OBJ_PAIR:
            printf("(");
            objectPrint(object->head);
            printf(",");
            objectPrint(object->tail);
            printf(")");
            break;
    }
}
