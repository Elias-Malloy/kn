#pragma once

#include <stdint.h>

#ifdef __BMI__
#include <immintrin.h>
#endif

typedef struct bit_array {
	uint64_t length;
	uint64_t bits[1];
} bit_array;

static inline void bit_array_copy(bit_array *dst, bit_array *src) {
    uint64_t i, len;
	len = (dst->length >> 6) + 1;
	for (i = 0; i < len; i++) {
		dst->bits[i] = src->bits[i];
	}
}

static inline void bit_array_zero(bit_array *a) {
    uint64_t i, len;
	len = (a->length >> 6) + 1;
	for (i = 0; i < len; i++) {
		a->bits[i] = 0;
	}
}
static inline unsigned char bit_array_get(bit_array *a, uint64_t i) {
    return a->bits[i>>6] >> (i&63) & 1;
}
static inline void bit_array_set(bit_array *a, uint64_t i) {
    a->bits[i>>6] |= 1ull<<(i&63);
}
static inline void bit_array_unset(bit_array *a, uint64_t i) {
    a->bits[i>>6] &= ~(1ull<<(i&63));
}
static inline void bit_array_flip(bit_array *a, uint64_t i) {
    a->bits[i>>6] ^= 1ull<<(i&63);
}
static inline void bit_array_not(bit_array *a) {
    uint64_t i, len;
	len = (a->length >> 6) + 1;
	for (i = 0; i < len; i++) {
		a->bits[i] = ~a->bits[i];
	}
}
static inline void bit_array_and(bit_array *a, bit_array *b) {
    uint64_t i, len;
	len = (a->length >> 6) + 1;
	for (i = 0; i < len; i++) {
		a->bits[i] &= b->bits[i];
	}
}
static inline void bit_array_or(bit_array *a, bit_array *b) {
    uint64_t i, len;
	len = (a->length >> 6) + 1;
	for (i = 0; i < len; i++) {
		a->bits[i] |= b->bits[i];
	}
}
static void bit_array_xor(bit_array *a, bit_array *b) {
    uint64_t i, len;
	len = (a->length >> 6) + 1;
	for (i = 0; i < len; i++) {
		a->bits[i] ^= b->bits[i];
	}
}
static unsigned long bit_array_tzcnt(bit_array *a) {
    uint64_t i, len, trailing_zeros;
	len = a->length;
#ifdef __BMI__
	i = 0;
	do {
		trailing_zeros = _tzcnt_u64(a->bits[i >> 6]);
	} while (trailing_zeros != 64 && (i += trailing_zeros) < len);
#else
    for (i = 0; i < len; i++) {
        if (bit_array256_get(a, i)) break; 
    } 
#endif
    return i;
}

typedef unsigned long long *bit_array256;

static inline void bit_array256_copy(bit_array256 dst, bit_array256 src) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
}

static inline void bit_array256_zero(bit_array256 a) {
    a[0] = a[1] = a[2] = a[3] = 0;
}

static inline unsigned char bit_array256_get(bit_array256 a, unsigned char b) {
    return a[b>>6] >> (b&63) & 1;
}
static inline void bit_array256_set(bit_array256 a, unsigned char b) {
    a[b >> 6] |= 1ull << (b & 63);
}
static inline void bit_array256_unset(bit_array256 a, unsigned char b) {
    a[b>>6] &= ~(1ull<<(b&63));
}
static inline void bit_array256_flip(bit_array256 a, unsigned char b) {
    a[b>>6] ^= 1ull<<(b&63);
}
static inline void bit_array256_not(bit_array256 a) {
    a[0] = ~a[0];
    a[1] = ~a[1];
    a[2] = ~a[2];
    a[3] = ~a[3];
}
static inline void bit_array256_and(bit_array256 a, bit_array256 b) {
    a[0] &= b[0];
    a[1] &= b[1];
    a[2] &= b[2];
    a[3] &= b[3];
}
static inline void bit_array256_or(bit_array256 a, bit_array256 b) {
    a[0] |= b[0];
    a[1] |= b[1];
    a[2] |= b[2];
    a[3] |= b[3];
}
static void bit_array256_xor(bit_array256 a, bit_array256 b) {
    a[0] ^= b[0];
    a[1] ^= b[1];
    a[2] ^= b[2];
    a[3] ^= b[3];
}
static unsigned long bit_array256_tzcnt(bit_array256 a) {
    unsigned long i;
#ifdef __BMI__
    i = _tzcnt_u64(a[0]);
    if (i == 64) {
        i += _tzcnt_u64(a[1]);
        if (i == 128) {
            i += _tzcnt_u64(a[2]);
            if (i == 192) {
                i += _tzcnt_u64(a[3]);
            }
        }
    }
#else
    for (i = 0; i < 256; i++) {
        if (bit_array256_get(a, i)) break; 
    } 
#endif
    return i;
}