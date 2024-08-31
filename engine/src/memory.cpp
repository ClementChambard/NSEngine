#include "./memory.h"

// #include "../memory/dynamic_allocator.h"
#include "./platform/platform.h"
#include "./logger.h"
#include <cstdio>
#include <cstring>

namespace ns {

struct MemoryStats {
  u64 total_allocated;
  u64 tagged_allocations[static_cast<usize>(MemTag::MAX_TAGS)];
};

NS_STATIC_ASSERT(static_cast<usize>(MemTag::MAX_TAGS) == 18,
                 "update memory tag strings");
static cstr memory_tag_strings[static_cast<usize>(MemTag::MAX_TAGS)] = {
    "UNKNOWN    ", "LINEAR_ALLC", "ARRAY      ", "VECTOR     ", "DICT       ",
    "RING_QUEUE ", "BST        ", "STRING     ", "APPLICATION", "JOB        ",
    "TEXTURE    ", "MAT_INST   ", "RENDERER   ", "GAME       ", "TRANSFORM  ",
    "ENTITY     ", "ENTITY_NODE", "SCENE      ",
};
// TODO: Make it so that the user can add custom memory tags to avoid putting all in GAME

struct memory_system_state {
  MemoryConfiguration config;
  MemoryStats stats;
  u64 alloc_count;
  usize allocator_memory_requirement;
  // dynamic_allocator allocator;
  ptr allocator_block;
};

static memory_system_state *g_state;

bool memory_system_initialize(MemoryConfiguration config) {
  usize state_memory_requirement = sizeof(memory_system_state);
  usize alloc_requirement = 0;
  // dynamic_allocator_create(config.total_alloc_size, &alloc_requirement, 0, 0);
  ptr block = platform::allocate_memory(
      state_memory_requirement + alloc_requirement, false);
  if (!block) {
    NS_FATAL("Memory system allocation failed and the system cannot continue.");
    return false;
  }

  g_state = reinterpret_cast<memory_system_state *>(block);
  g_state->config = config;
  g_state->stats = {};
  g_state->alloc_count = 0;
  g_state->allocator_memory_requirement = alloc_requirement;
  platform::zero_memory(&g_state->stats, sizeof(g_state->stats));
  g_state->allocator_block =
      reinterpret_cast<u8 *>(block) + state_memory_requirement;
  // if (!dynamic_allocator_create(
  //         config.total_alloc_size, &state_ptr->allocator_memory_requirement,
  //         state_ptr->allocator_block, &state_ptr->allocator)) {
  //   NS_FATAL("Memory system is unable to setup internal allocator. Application "
  //            "cannot continue.");
  //   return false;
  // }

  NS_DEBUG("Memory system successfully allocated %llu bytes.",
           config.total_alloc_size);
  return true;
}

void memory_system_shutdown() {
  if (g_state) {
    // dynamic_allocator_destroy(&state_ptr->allocator);
    platform::free_memory(g_state, g_state->allocator_memory_requirement +
                                         sizeof(memory_system_state));
    g_state = nullptr;
  }
}

ptr alloc_raw(usize size, MemTag tag) {
  // if (tag == MemTag::UNKNOWN) {
  //   NS_WARN("ns::alloc called using mem_tag::UNKNOWN. Re-class this "
  //           "allocation.");
  // }

  if (tag == MemTag::UNTRACKED) {
    return platform::allocate_memory(size, false);
  }

  ptr block = nullptr;
  if (g_state) {
    g_state->stats.total_allocated += size;
    g_state->stats.tagged_allocations[static_cast<usize>(tag)] += size;
    g_state->alloc_count++;
    // block = dynamic_allocator_allocate(&state_ptr->allocator, size);
    block = platform::allocate_memory(size, false);
  } else {
    NS_WARN("ns::alloc called before the memory system is initialized.");
    block = platform::allocate_memory(size, false);
  }

  if (block) {
    platform::zero_memory(block, size);
    return block;
  }

  NS_FATAL("ns::alloc failed to allocate successfully.");
  return 0;
}

void free_raw(ptr block, usize size, MemTag tag) {
  // if (tag == MemTag::UNKNOWN) {
  //   NS_WARN("ns::free called using mem_tag::UNKNOWN. Re-class this "
  //           "free.");
  // }

  if (tag == MemTag::UNTRACKED) {
    platform::free_memory(block, false);
    return;
  }

  if (g_state) {
    g_state->stats.total_allocated -= size;
    g_state->stats.tagged_allocations[static_cast<usize>(tag)] -= size;
    // bool result = dynamic_allocator_free(&state_ptr->allocator, block, size);
    // if (!result) { // block was not created with the dynamic_allocator
    platform::free_memory(block, false);
    // }
  } else {
    // TODO: memory alignment
    platform::free_memory(block, false);
  }
}

ptr mem_zero(ptr block, usize size) {
  return platform::zero_memory(block, size);
}

ptr mem_copy(ptr dest, roptr source, usize size) {
  return platform::copy_memory(dest, source, size);
}

ptr mem_set(ptr dest, i32 value, usize size) {
  return platform::set_memory(dest, value, size);
}

pstr get_memory_usage_str() {
  if (!g_state)
    return nullptr;
  const usize gib = 1024 * 1024 * 1024;
  const usize mib = 1024 * 1024;
  const usize kib = 1024;

  char buffer[8000] = "System memory use (tagged):\n";
  usize offset = strlen(buffer);
  for (u32 i = 0; i < static_cast<u32>(MemTag::MAX_TAGS); ++i) {
    char unit[4] = "XiB";
    f32 amount = 1.0f;
    if (g_state->stats.tagged_allocations[i] >= gib) {
      unit[0] = 'G';
      amount = g_state->stats.tagged_allocations[i] / static_cast<f32>(gib);
    } else if (g_state->stats.tagged_allocations[i] >= mib) {
      unit[0] = 'M';
      amount = g_state->stats.tagged_allocations[i] / static_cast<f32>(mib);
    } else if (g_state->stats.tagged_allocations[i] >= kib) {
      unit[0] = 'K';
      amount = g_state->stats.tagged_allocations[i] / static_cast<f32>(kib);
    } else {
      unit[0] = 'B';
      unit[1] = '\0';
      amount = static_cast<f32>(g_state->stats.tagged_allocations[i]);
    }

    offset += snprintf(buffer + offset, sizeof(buffer) - offset,
                       "  %s: %.2f%s\n", memory_tag_strings[i], amount, unit);
  }

  pstr out_string = strdup(buffer);
  return out_string;
}

u64 get_memory_alloc_count() {
  if (g_state)
    return g_state->alloc_count;
  return 0;
}

} // namespace ns
