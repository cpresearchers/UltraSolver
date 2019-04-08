﻿#pragma once
#include <vector>
//#include <ppltasks.h>
//#include <intrin.h>

#ifdef _MSC_VER
#include <intrin.h>
//#include <taskflow/graph/basic_taskflow.hpp>
inline int BitCount(const int a) {
	return __popcnt(a);
};
inline int BitCount64(const uint64_t a) {
	return __popcnt64(a);
};
inline int CLZ64(const uint64_t a) {
	return __lzcnt64(a);
};
inline int CTZ64(const uint64_t a) {
	unsigned long b;
	const auto c = _BitScanForward64(&b, a);
	return c ? 63 - b : 64;
};
#endif

#ifdef __GNUC__
inline int BitCount(const int a) {
	return __builtin_popcount(a);
};
inline int BitCount64(const uint64_t a) {
	return __builtin_popcountll(a);
};
inline int CLZ64(const uint64_t a) {
	return __lzcnt64(a);
};
inline int CTZ64(const uint64_t a) {
	unsigned long b;
	const auto c = _BitScanForward64(&b, a);
	return c ? 63 - b : 64;
};
#endif

namespace cp {




typedef unsigned long long u64;

namespace Constants {
const int INDEXOVERFLOW = -1;
const int BITSIZE = 64;
const int DIVBIT = 6;
const int MODMASK = 0x3f;
const u64 MASK1[] = { 0x8000000000000000L, 0x4000000000000000L, 0x2000000000000000L, 0x1000000000000000L, 0x0800000000000000L, 0x0400000000000000L, 0x0200000000000000L, 0x0100000000000000L, 0x0080000000000000L, 0x0040000000000000L, 0x0020000000000000L, 0x0010000000000000L, 0x0008000000000000L, 0x0004000000000000L, 0x0002000000000000L, 0x0001000000000000L, 0x0000800000000000L, 0x0000400000000000L, 0x0000200000000000L, 0x0000100000000000L, 0x0000080000000000L, 0x0000040000000000L, 0x0000020000000000L, 0x0000010000000000L, 0x0000008000000000L, 0x0000004000000000L, 0x0000002000000000L, 0x0000001000000000L, 0x0000000800000000L, 0x0000000400000000L, 0x0000000200000000L, 0x0000000100000000L, 0x0000000080000000L, 0x0000000040000000L, 0x0000000020000000L, 0x0000000010000000L, 0x0000000008000000L, 0x0000000004000000L, 0x0000000002000000L, 0x0000000001000000L, 0x0000000000800000L, 0x0000000000400000L, 0x0000000000200000L, 0x0000000000100000L, 0x0000000000080000L, 0x0000000000040000L, 0x0000000000020000L, 0x0000000000010000L, 0x0000000000008000L, 0x0000000000004000L, 0x0000000000002000L, 0x0000000000001000L, 0x0000000000000800L, 0x0000000000000400L, 0x0000000000000200L, 0x0000000000000100L, 0x0000000000000080L, 0x0000000000000040L, 0x0000000000000020L, 0x0000000000000010L, 0x0000000000000008L, 0x0000000000000004L, 0x0000000000000002L, 0x0000000000000001L };
const u64 MASK0[] = { 0x7FFFFFFFFFFFFFFFL, 0xBFFFFFFFFFFFFFFFL, 0xDFFFFFFFFFFFFFFFL, 0xEFFFFFFFFFFFFFFFL, 0xF7FFFFFFFFFFFFFFL, 0xFBFFFFFFFFFFFFFFL, 0xFDFFFFFFFFFFFFFFL, 0xFEFFFFFFFFFFFFFFL, 0xFF7FFFFFFFFFFFFFL, 0xFFBFFFFFFFFFFFFFL, 0xFFDFFFFFFFFFFFFFL, 0xFFEFFFFFFFFFFFFFL, 0xFFF7FFFFFFFFFFFFL, 0xFFFBFFFFFFFFFFFFL, 0xFFFDFFFFFFFFFFFFL, 0xFFFEFFFFFFFFFFFFL, 0xFFFF7FFFFFFFFFFFL, 0xFFFFBFFFFFFFFFFFL, 0xFFFFDFFFFFFFFFFFL, 0xFFFFEFFFFFFFFFFFL, 0xFFFFF7FFFFFFFFFFL, 0xFFFFFBFFFFFFFFFFL, 0xFFFFFDFFFFFFFFFFL, 0xFFFFFEFFFFFFFFFFL, 0xFFFFFF7FFFFFFFFFL, 0xFFFFFFBFFFFFFFFFL, 0xFFFFFFDFFFFFFFFFL, 0xFFFFFFEFFFFFFFFFL, 0xFFFFFFF7FFFFFFFFL, 0xFFFFFFFBFFFFFFFFL, 0xFFFFFFFDFFFFFFFFL, 0xFFFFFFFEFFFFFFFFL, 0xFFFFFFFF7FFFFFFFL, 0xFFFFFFFFBFFFFFFFL, 0xFFFFFFFFDFFFFFFFL, 0xFFFFFFFFEFFFFFFFL, 0xFFFFFFFFF7FFFFFFL, 0xFFFFFFFFFBFFFFFFL, 0xFFFFFFFFFDFFFFFFL, 0xFFFFFFFFFEFFFFFFL, 0xFFFFFFFFFF7FFFFFL, 0xFFFFFFFFFFBFFFFFL, 0xFFFFFFFFFFDFFFFFL, 0xFFFFFFFFFFEFFFFFL, 0xFFFFFFFFFFF7FFFFL, 0xFFFFFFFFFFFBFFFFL, 0xFFFFFFFFFFFDFFFFL, 0xFFFFFFFFFFFEFFFFL, 0xFFFFFFFFFFFF7FFFL, 0xFFFFFFFFFFFFBFFFL, 0xFFFFFFFFFFFFDFFFL, 0xFFFFFFFFFFFFEFFFL, 0xFFFFFFFFFFFFF7FFL, 0xFFFFFFFFFFFFFBFFL, 0xFFFFFFFFFFFFFDFFL, 0xFFFFFFFFFFFFFEFFL, 0xFFFFFFFFFFFFFF7FL, 0xFFFFFFFFFFFFFFBFL, 0xFFFFFFFFFFFFFFDFL, 0xFFFFFFFFFFFFFFEFL, 0xFFFFFFFFFFFFFFF7L, 0xFFFFFFFFFFFFFFFBL, 0xFFFFFFFFFFFFFFFDL, 0xFFFFFFFFFFFFFFFEL };
const int kINTMAX = 0x3f3f3f3f;
const int kUNBINDLEVEL = 0x3f3f3f3f;
const u64 kALLONELONG = 0xFFFFFFFFFFFFFFFFL;

class Index2D {
public:
	int x, y;
};

inline Index2D GetIndex2D(const int index) {
	return Index2D{ index >> Constants::DIVBIT, index & Constants::MODMASK };
}

inline int GetIndex(const Index2D& index2D) {
	return  (index2D.x << Constants::DIVBIT) + index2D.y;
}
}



/* a=target variable, b=bit number to act upon 0-n */
#define BIT_SET(a,b) ((a) |= (1ULL<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1ULL<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1ULL<<(b)))
#define BIT_CHECK(a,b) (!!((a) & (1ULL<<(b))))        // '!!' to make sure this returns 0 or 1

/* x=target variable, y=mask */
#define BITMASK_SET(x,y) ((x) |= (y))
#define BITMASK_CLEAR(x,y) ((x) &= (~(y)))
#define BITMASK_FLIP(x,y) ((x) ^= (y))
#define BITMASK_CHECK_ALL(x,y) (((x) & (y)) == (y))   // warning: evaluates y twice
#define BITMASK_CHECK_ANY(x,y) ((x) & (y))

//inline int BitCount64(u64& i) {
//	return __builtin_popcountll(i);
//}
//
//inline int LeftMostOne64(u64& i) {
//	return CLZ64(i);
//}


inline bool Has(const u64 b, const int a) {
	return !(!(b & Constants::MASK1[a]));
}


inline void GetValues(u64 b, std::vector<int>& values) {
	values.clear();
	for (int i = CLZ64(b), end = CTZ64(b); i <= end; ++i) {
		if (Has(b, i)) {
			values.push_back(i);
		}
	}
}

}


