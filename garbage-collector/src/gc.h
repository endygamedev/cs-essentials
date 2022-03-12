#define STACK_MAX 256
#define INIT_OBJ_NUM_MAX 4



typedef enum {
    OBJ_INT,
    OBJ_PAIR
} ObjectType;



typedef struct sObject {
    struct sObject* next;
    unsigned char marked;
    ObjectType type;

    union {
        /* OBJ_INT */
        int value;
        
        /* OBJ_TAIL */
        struct {
            struct sObject* head;
            struct sObject* tail;
        };
    };
} Object;



typedef struct {
    int numObjects;
    int maxObjects;
    Object* firstObject;
    Object* stack[STACK_MAX];
    int stackSize;
} VM;



void assert(int condition, const char* message) {
    if(!condition) {
        printf("%s\n", message);
        exit(1);
    }
}



VM* newVM() {
    VM* vm = malloc(sizeof(VM));
    vm->stackSize = 0;
    vm->firstObject = NULL;
    vm->numObjects = 0;
    vm->maxObjects = INIT_OBJ_NUM_MAX;
    return vm;
}



void push(VM *vm, Object *value) {
    assert(vm->stackSize < STACK_MAX, "Stack is overflow!");
    vm->stack[vm->stackSize++] = value;
}



Object* pop(VM* vm) {
    assert(vm->stackSize > 0, "Stack underflow!");
    return vm->stack[--vm->stackSize];
} 



Object* newObject(VM* vm, ObjectType type) {

    if (vm->numObjects == vm->maxObjects) gc(vm);

    Object* object = malloc(sizeof(Object));
    object->type = type;
    object->marked = 0;

    object->next = vm->firstObject;
    vm->firstObject = object;
    vm->numObjects++;
    return object;
}



void pushInt(VM* vm, int intValue) {
    Object* object = newObject(vm, OBJ_INT);
    object->value = intValue;
    objectPrint(object);
    push(vm, object);
    printf("\nCollected %d objects.\n", vm->numObjects);
}



Object* pushPair(VM* vm) {
    Object* object = newObject(vm, OBJ_PAIR);
    object->tail = pop(vm);
    object->head = pop(vm);
    objectPrint(object);
    push(vm, object);
    printf("\nCollected %d objects.\n", vm->numObjects);
}



void markAll(VM* vm) {
    for (int i = 0; i < vm->stackSize; i++) {
        mark(vm->stack[i]);
    }
}



void mark(Object* object) {

    if (object->marked) return;

    object->marked = 1;

    if (object->type == OBJ_PAIR) {
        mark(object->head);
        mark(object->tail);
    }
}



void sweep(VM* vm) {
    Object** object = &vm->firstObject;
    while (*object) {
        if (!(*object)->marked) {
            Object* unreached = *object;

            *object = unreached->next;
            free(unreached);
        } else {
            (*object)->marked = 0;
            object = &(*object)->next;
        }
    }
}



void gc(VM* vm) {
    int numObjects = vm->numObjects;

    markAll(vm);
    sweep(vm);

    vm->maxObjects = vm->numObjects == 0 ? INIT_OBJ_NUM_MAX : vm->numObjects*2;

    printf("Collected %d objects, %d remaning.\n", numObjects - vm->numObjects, vm->numObjects);
}



void freeVM(VM* vm) {
    vm->stackSize = 0;
    gc(vm);
    free(vm);
}



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
