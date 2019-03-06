#pragma once

#ifndef __ALIGNEDALLOCATOR_H__
#define __ALIGNEDALLOCATOR_H__

#include <memory>

template <size_t AlignSize>
class AlignedAllocator {
public:
	static void* operator new(size_t size) {
		return _aligned_malloc(size, AlignSize);
	}

	static void operator delete(void* memory) {
		_aligned_free(memory);
	}
};

#endif