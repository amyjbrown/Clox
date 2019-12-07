/* Memory.h
Memory management macros and tools
Contents:
    * GROW_CAPACITY(capacity): 
        Dynamically grow array length, or initalize length as 8 
    
    *  GROW_ARRAY(previous, type, oldCount, count)
        Dynamically copies and reallocates array, evaluating to the 
        new dynamic code values
    
    * reallocate(previous, oldSize, newSize) -> void*
        Reallocates and returns a pointer(?) to the newly reallocate elements 


Memory allocaiton of dynamic array:
old         new         Operation
-----------------------------------------
0           Positive     Allocate new block
Positive    0            Free allocation
0           < Oldsize    Shrink allocation
Positive    > Oldsize    Grow allocation
*/

#ifndef clox_memory_h
#define clox_memory_h

#define ALLOCATE(type, count) \
    (type*) reallocate(NULL, 0, sizeof(type) * (count));

#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(previous, type, oldCount, count) \
    (type*) reallocate(previous, sizeof(type) * (oldCount), \
        sizeof(type) * (count))

#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate(pointer, sizeof(type) * (oldCount), 0)

void* reallocate(void* previous, size_t oldSize, size_t newSize);


#endif