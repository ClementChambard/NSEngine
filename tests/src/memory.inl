#include "memory.h"
#include "testing/Testing.hpp"

constexpr ns::MemTag MEMTAG = ns::MemTag::UNTRACKED;

TEST_BATCH_BEGIN(run_memory_tests, "memory")

TEST("alloc-dealloc raw") {
  int *pointer = (int *)ns::alloc_raw(sizeof(int), MEMTAG);
  TEST_NE(pointer, nullptr);
  *pointer = 12345;
  ns::free_raw(pointer, sizeof(int), MEMTAG);
  TEST_OK;
}

TEST("alloc-dealloc one") {
  int *pointer = ns::alloc<int>(MEMTAG);
  TEST_NE(pointer, nullptr);
  *pointer = 12345;
  ns::free(pointer, MEMTAG);
  TEST_OK;
}

TEST("alloc-dealloc n") {
  int *pointer = ns::alloc_n<int>(3, MEMTAG);
  TEST_NE(pointer, nullptr);
  pointer[0] = 123;
  pointer[1] = 456;
  pointer[2] = 789;
  ns::free_n(pointer, 3, MEMTAG);
  TEST_OK;
}

TEST("construct-destroy") {
  int *pointer = ns::construct<int>(MEMTAG, 12345);
  TEST_NE(pointer, nullptr);
  TEST_EQ(*pointer, 12345);
  ns::destroy(pointer, MEMTAG);
  TEST_OK;
}

TEST("construct-destroy n") {
  int *pointer = ns::construct_n<int>(3, MEMTAG);
  TEST_NE(pointer, nullptr);
  pointer[0] = 123;
  pointer[1] = 456;
  pointer[2] = 789;
  ns::destroy_n(pointer, 3, MEMTAG);
  TEST_OK;
}

TEST("mem_zero") {
  int data[] = {1, 2, 3};
  ns::mem_zero(data, sizeof(data));
  TEST_EQ(data[0], 0);
  TEST_EQ(data[1], 0);
  TEST_EQ(data[2], 0);
  TEST_OK;
}

TEST("mem_set") {
  char data[] = {1, 2, 3};
  ns::mem_set(data, 4, sizeof(data));
  TEST_EQ(data[0], 4);
  TEST_EQ(data[1], 4);
  TEST_EQ(data[2], 4);
  TEST_OK;
}

TEST("mem_copy") {
  int data[] = {1, 2, 3};
  int data2[] = {4, 5, 6};
  ns::mem_copy(data, data2, sizeof(data));
  TEST_EQ(data[0], data2[0]);
  TEST_EQ(data[1], data2[1]);
  TEST_EQ(data[2], data2[2]);
  TEST_OK;
}

TEST_BATCH_END
