#include "stdafx.h"
#include "engineMemory.h"


void *AlignendMalloc(size_t size, size_t align)
{
	void* p = malloc(size + (align - 1) + sizeof(void*));
	uintptr_t mem = reinterpret_cast<uintptr_t>(p);
	mem += sizeof(void*);
	mem = (mem + align - 1) & ~(align - 1);
	*reinterpret_cast<void**>(mem - sizeof(void*)) = p;
	return reinterpret_cast<void*>(mem);
}

void AlignedFree(void *inMem)
{
	uintptr_t mem = reinterpret_cast<uintptr_t>(inMem);
	void* p = *reinterpret_cast<void**>(mem - sizeof(void*));
	free(p);
}

bool AlignedMalloc_UnitTest()
{
    bool isOk = true;

    struct MallocTest
    {
        size_t size;
        size_t align;
    };
    MallocTest mallocTest[] =
    {
        { 100, 16 },
        { 200, 16 },
        { 256, 16 },
        { 1024, 16 },
        { 10, 256 },
        { 256, 256 },
        { 511, 256 },
        { 512, 256 },
        { 513, 256 },
        { 100, 1 },
    };
    int count = sizeof(mallocTest) / sizeof(mallocTest[0]);

    // allocate everything in the mallocTest table
    void **pointers = new void*[count];
    for (int i = 0; i < count; ++i)
    {
        // allocate the pointer
        pointers[i] = AlignendMalloc(mallocTest[i].size, mallocTest[i].align);
        //make sure it came out aligned
        uintptr_t address = reinterpret_cast<uintptr_t>(pointers[i]);
        isOk &= (address & (mallocTest[i].align - 1)) == 0;
        // fill the memory at that location with the index
        memset(pointers[i], i, mallocTest[i].size);
    }
    // release all the memory
    for (int i = 0; i < count; ++i)
    {
        // first make sure that the memory still contains what you put in there
        for (size_t j = 0; j < mallocTest[i].size; ++j)
        {
            char* pChar = reinterpret_cast<char*>(pointers[i]);
            isOk &= pChar[j] == i;
        }
        // and free the memory
        AlignedFree(pointers[i]);
    }
    delete[] pointers;

    return isOk;
}