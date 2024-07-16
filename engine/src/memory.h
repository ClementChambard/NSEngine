#ifndef MEMORY_HEADER_INCLUDED
#define MEMORY_HEADER_INCLUDED

#include "./defines.h"

#include <new>

namespace ns {

enum class MemTag {
  UNKNOWN,
  LINEAR_ALLOCATOR,
  ARRAY,
  VECTOR,
  DICT,
  RING_QUEUE,
  BST,
  STRING,
  APPLICATION,
  JOB,
  TEXTURE,
  MATERIAL_INSTANCE,
  RENDERER,
  GAME,
  TRANSFORM,
  ENTITY,
  ENTITY_NODE,
  SCENE,

  MAX_TAGS,
  UNTRACKED
};

struct MemoryConfiguration {
  u64 total_alloc_size;
};

NS_API bool memory_system_initialize(MemoryConfiguration config);

NS_API void memory_system_shutdown();

/**
 * Allocate a block of memory
 * @param size the size of the block to allocate
 * @param tag the tag of the block
 */
NS_API ptr alloc_raw(usize size, MemTag tag = MemTag::UNKNOWN);

/**
 * Free a block of memory
 * @param block the block to free
 * @param size the size of the block to free
 * @param tag the tag of the block
 */
NS_API void free_raw(ptr block, usize size, MemTag tag = MemTag::UNKNOWN);

/**
 * Allocate an object in memory
 * @param tag the tag of the block
 */
template <typename T>
NS_API T *alloc(MemTag tag = MemTag::UNKNOWN) {
  return reinterpret_cast<T*>(alloc_raw(sizeof(T), tag));
}

/**
 * Allocate an array of a type
 * @param count the count of elements in the block
 * @param tag the tag of the block
 */
template <typename T>
NS_API T *alloc_n(usize count, MemTag tag = MemTag::UNKNOWN) {
  return reinterpret_cast<T*>(alloc_raw(sizeof(T) * count, tag));
}

/**
 * Free an allocation of a type
 * @param block the block to free
 * @param tag the tag of the block
 */
template <typename T>
NS_API void free(T* block, MemTag tag = MemTag::UNKNOWN) {
  free_raw(block, sizeof(T), tag);
}

/**
 * Free an array of a type
 * @param block the block to free
 * @param count the count of elements in the block
 * @param tag the tag of the block
 */
template <typename T>
NS_API void free_n(T *block, usize count, MemTag tag = MemTag::UNKNOWN) {
  free_raw(block, count * sizeof(T), tag);
}

/**
 * Alloc and construct an object
 * @param tag the tag of the memory block allocated
 * @param args... the arguments of the constructor
 */
template <typename T, typename... Args>
NS_API T *construct(MemTag tag, Args&&... args) {
  ptr mem = alloc_raw(sizeof(T), tag);
  return ::new(mem) T(static_cast<Args&&>((Args&)args)...);
}

/**
 * Alloc and construct an array of objects
 * @param count the number of elements in the array
 * @param tag the tag of the memory block allocated
 * @param args... the arguments of the constructor
 */
template <typename T>
NS_API T *construct_n(usize count, MemTag tag) {
  T *mem = alloc_n<T>(count, tag);
  for (; count; --count) ::new(&mem[count-1]) T();
  return mem;
}

/**
 * Call destructor and free a typed allocation
 * @param obj the object to destroy
 * @param tag the tag of the block
 */
template <typename T>
NS_API void destroy(T *obj, MemTag tag = MemTag::UNKNOWN) {
  obj->~T();
  free(obj, tag);
}

/**
 * Call destructor and free an array of a type
 * @param arr the array to destroy
 * @param count the count of elements in the array
 * @param tag the tag of the block
 */
template <typename T>
NS_API void destroy_n(T *arr, usize count, MemTag tag = MemTag::UNKNOWN) {
  for (; count; --count) arr[count-1].~T();
  free_n(arr, count, tag);
}

/**
 * Zero a block of memory
 * @param block the block to zero
 * @param size the size of the block
 */
NS_API ptr mem_zero(ptr block, usize size);

/**
 * Copy a block of memory
 * @param dest the destination block
 * @param source the source block
 * @param size the size of the block
 */
NS_API ptr mem_copy(ptr dest, roptr source, usize size);

/**
 * Set a block of memory
 * @param dest the destination block
 * @param value the value to set
 * @param size the size of the block
 */
NS_API ptr mem_set(ptr dest, i32 value, usize size);

/**
 * Get the memory usage string
 * @returns the memory usage string
 */
NS_API pstr get_memory_usage_str();

/**
 * Get the number of allocations
 * @returns the number of allocations
 */
NS_API u64 get_memory_alloc_count();


template <MemTag tag = MemTag::UNTRACKED>
struct WithOpNewDel {
  void *operator new(unsigned long sz) {
    return alloc_raw(sz, tag);
  }

  void operator delete(void* d, unsigned long sz) {
    free_raw(d, sz, tag);
  }
};

} // namespace ns

#endif // MEMORY_HEADER_INCLUDED
