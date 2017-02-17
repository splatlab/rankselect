/* -*- Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*- */
#ifndef _FASTRANK_POPCOUNT_H_
#define _FASTRANK_POPCOUNT_H_

#include <sys/types.h>
#include <stdio.h>

#define L8 0x0101010101010101ULL // Every lowest 8th bit set: 00000001...
#define G2 0xAAAAAAAAAAAAAAAAULL // Every highest 2nd bit: 101010...
#define G4 0x3333333333333333ULL // 00110011 ... used to group the sum of 4 bits.
#define G8 0x0F0F0F0F0F0F0F0FULL
#define H8 0x8080808080808080ULL 
#define L9 0x0040201008040201ULL
#define H9 (L9 << 8)
#define L16 0x0001000100010001ULL
#define H16 0x8000800080008000ULL

#define ONES_STEP_4 ( 0x1111111111111111ULL )
#define ONES_STEP_8 ( 0x0101010101010101ULL )
#define ONES_STEP_9 ( 1ULL << 0 | 1ULL << 9 | 1ULL << 18 | 1ULL << 27 | 1ULL << 36 | 1ULL << 45 | 1ULL << 54 )
#define ONES_STEP_16 ( 1ULL << 0 | 1ULL << 16 | 1ULL << 32 | 1ULL << 48 )
#define MSBS_STEP_4 ( 0x8ULL * ONES_STEP_4 )
#define MSBS_STEP_8 ( 0x80ULL * ONES_STEP_8 )
#define MSBS_STEP_9 ( 0x100ULL * ONES_STEP_9 )
#define MSBS_STEP_16 ( 0x8000ULL * ONES_STEP_16 )
#define INCR_STEP_8 ( 0x80ULL << 56 | 0x40ULL << 48 | 0x20ULL << 40 | 0x10ULL << 32 | 0x8ULL << 24 | 0x4ULL << 16 | 0x2ULL << 8 | 0x1 )

#define ONES_STEP_32 ( 0x0000000100000001ULL )
#define MSBS_STEP_32 ( 0x8000000080000000ULL )

#define COMPARE_STEP_8(x,y) ( ( ( ( ( (x) | MSBS_STEP_8 ) - ( (y) & ~MSBS_STEP_8 ) ) ^ (x) ^ ~(y) ) & MSBS_STEP_8 ) >> 7 )
#define LEQ_STEP_8(x,y) ( ( ( ( ( (y) | MSBS_STEP_8 ) - ( (x) & ~MSBS_STEP_8 ) ) ^ (x) ^ (y) ) & MSBS_STEP_8 ) >> 7 )

#define UCOMPARE_STEP_9(x,y) ( ( ( ( ( ( (x) | MSBS_STEP_9 ) - ( (y) & ~MSBS_STEP_9 ) ) | ( x ^ y ) ) ^ ( x | ~y ) ) & MSBS_STEP_9 ) >> 8 )
#define UCOMPARE_STEP_16(x,y) ( ( ( ( ( ( (x) | MSBS_STEP_16 ) - ( (y) & ~MSBS_STEP_16 ) ) | ( x ^ y ) ) ^ ( x | ~y ) ) & MSBS_STEP_16 ) >> 15 )
#define ULEQ_STEP_9(x,y) ( ( ( ( ( ( (y) | MSBS_STEP_9 ) - ( (x) & ~MSBS_STEP_9 ) ) | ( x ^ y ) ) ^ ( x & ~y ) ) & MSBS_STEP_9 ) >> 8 )
#define ULEQ_STEP_16(x,y) ( ( ( ( ( ( (y) | MSBS_STEP_16 ) - ( (x) & ~MSBS_STEP_16 ) ) | ( x ^ y ) ) ^ ( x & ~y ) ) & MSBS_STEP_16 ) >> 15 )
#define ZCOMPARE_STEP_8(x) ( ( ( x | ( ( x | MSBS_STEP_8 ) - ONES_STEP_8 ) ) & MSBS_STEP_8 ) >> 7 )

    //! Lookup table for select on bytes.
    /*! Entry at idx = 256*j + i equals the position of the
        (j+1)-th set bit in byte i. Positions lie in the range \f$[0..7]\f$.
     */

const uint8_t lt_sel[] = {
    0,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    7,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,

    0,0,0,1,0,2,2,1,0,3,3,1,3,2,2,1,
    0,4,4,1,4,2,2,1,4,3,3,1,3,2,2,1,
    0,5,5,1,5,2,2,1,5,3,3,1,3,2,2,1,
    5,4,4,1,4,2,2,1,4,3,3,1,3,2,2,1,
    0,6,6,1,6,2,2,1,6,3,3,1,3,2,2,1,
    6,4,4,1,4,2,2,1,4,3,3,1,3,2,2,1,
    6,5,5,1,5,2,2,1,5,3,3,1,3,2,2,1,
    5,4,4,1,4,2,2,1,4,3,3,1,3,2,2,1,
    0,7,7,1,7,2,2,1,7,3,3,1,3,2,2,1,
    7,4,4,1,4,2,2,1,4,3,3,1,3,2,2,1,
    7,5,5,1,5,2,2,1,5,3,3,1,3,2,2,1,
    5,4,4,1,4,2,2,1,4,3,3,1,3,2,2,1,
    7,6,6,1,6,2,2,1,6,3,3,1,3,2,2,1,
    6,4,4,1,4,2,2,1,4,3,3,1,3,2,2,1,
    6,5,5,1,5,2,2,1,5,3,3,1,3,2,2,1,
    5,4,4,1,4,2,2,1,4,3,3,1,3,2,2,1,

    0,0,0,0,0,0,0,2,0,0,0,3,0,3,3,2,
    0,0,0,4,0,4,4,2,0,4,4,3,4,3,3,2,
    0,0,0,5,0,5,5,2,0,5,5,3,5,3,3,2,
    0,5,5,4,5,4,4,2,5,4,4,3,4,3,3,2,
    0,0,0,6,0,6,6,2,0,6,6,3,6,3,3,2,
    0,6,6,4,6,4,4,2,6,4,4,3,4,3,3,2,
    0,6,6,5,6,5,5,2,6,5,5,3,5,3,3,2,
    6,5,5,4,5,4,4,2,5,4,4,3,4,3,3,2,
    0,0,0,7,0,7,7,2,0,7,7,3,7,3,3,2,
    0,7,7,4,7,4,4,2,7,4,4,3,4,3,3,2,
    0,7,7,5,7,5,5,2,7,5,5,3,5,3,3,2,
    7,5,5,4,5,4,4,2,5,4,4,3,4,3,3,2,
    0,7,7,6,7,6,6,2,7,6,6,3,6,3,3,2,
    7,6,6,4,6,4,4,2,6,4,4,3,4,3,3,2,
    7,6,6,5,6,5,5,2,6,5,5,3,5,3,3,2,
    6,5,5,4,5,4,4,2,5,4,4,3,4,3,3,2,

    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,
    0,0,0,0,0,0,0,4,0,0,0,4,0,4,4,3,
    0,0,0,0,0,0,0,5,0,0,0,5,0,5,5,3,
    0,0,0,5,0,5,5,4,0,5,5,4,5,4,4,3,
    0,0,0,0,0,0,0,6,0,0,0,6,0,6,6,3,
    0,0,0,6,0,6,6,4,0,6,6,4,6,4,4,3,
    0,0,0,6,0,6,6,5,0,6,6,5,6,5,5,3,
    0,6,6,5,6,5,5,4,6,5,5,4,5,4,4,3,
    0,0,0,0,0,0,0,7,0,0,0,7,0,7,7,3,
    0,0,0,7,0,7,7,4,0,7,7,4,7,4,4,3,
    0,0,0,7,0,7,7,5,0,7,7,5,7,5,5,3,
    0,7,7,5,7,5,5,4,7,5,5,4,5,4,4,3,
    0,0,0,7,0,7,7,6,0,7,7,6,7,6,6,3,
    0,7,7,6,7,6,6,4,7,6,6,4,6,4,4,3,
    0,7,7,6,7,6,6,5,7,6,6,5,6,5,5,3,
    7,6,6,5,6,5,5,4,6,5,5,4,5,4,4,3,

    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,
    0,0,0,0,0,0,0,5,0,0,0,5,0,5,5,4,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,
    0,0,0,0,0,0,0,6,0,0,0,6,0,6,6,4,
    0,0,0,0,0,0,0,6,0,0,0,6,0,6,6,5,
    0,0,0,6,0,6,6,5,0,6,6,5,6,5,5,4,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,
    0,0,0,0,0,0,0,7,0,0,0,7,0,7,7,4,
    0,0,0,0,0,0,0,7,0,0,0,7,0,7,7,5,
    0,0,0,7,0,7,7,5,0,7,7,5,7,5,5,4,
    0,0,0,0,0,0,0,7,0,0,0,7,0,7,7,6,
    0,0,0,7,0,7,7,6,0,7,7,6,7,6,6,4,
    0,0,0,7,0,7,7,6,0,7,7,6,7,6,6,5,
    0,7,7,6,7,6,6,5,7,6,6,5,6,5,5,4,

    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,
    0,0,0,0,0,0,0,6,0,0,0,6,0,6,6,5,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,
    0,0,0,0,0,0,0,7,0,0,0,7,0,7,7,5,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,
    0,0,0,0,0,0,0,7,0,0,0,7,0,7,7,6,
    0,0,0,0,0,0,0,7,0,0,0,7,0,7,7,6,
    0,0,0,7,0,7,7,6,0,7,7,6,7,6,6,5,

    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,
    0,0,0,0,0,0,0,7,0,0,0,7,0,7,7,6,

    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7
};

    //! Use to help to decide if a prefix sum stored in a byte overflows.

const uint64_t ps_overflow[] = {
    0x8080808080808080ULL,
    0x7f7f7f7f7f7f7f7fULL,
    0x7e7e7e7e7e7e7e7eULL,
    0x7d7d7d7d7d7d7d7dULL,
    0x7c7c7c7c7c7c7c7cULL,
    0x7b7b7b7b7b7b7b7bULL,
    0x7a7a7a7a7a7a7a7aULL,
    0x7979797979797979ULL,
    0x7878787878787878ULL,
    0x7777777777777777ULL,
    0x7676767676767676ULL,
    0x7575757575757575ULL,
    0x7474747474747474ULL,
    0x7373737373737373ULL,
    0x7272727272727272ULL,
    0x7171717171717171ULL,
    0x7070707070707070ULL,
    0x6f6f6f6f6f6f6f6fULL,
    0x6e6e6e6e6e6e6e6eULL,
    0x6d6d6d6d6d6d6d6dULL,
    0x6c6c6c6c6c6c6c6cULL,
    0x6b6b6b6b6b6b6b6bULL,
    0x6a6a6a6a6a6a6a6aULL,
    0x6969696969696969ULL,
    0x6868686868686868ULL,
    0x6767676767676767ULL,
    0x6666666666666666ULL,
    0x6565656565656565ULL,
    0x6464646464646464ULL,
    0x6363636363636363ULL,
    0x6262626262626262ULL,
    0x6161616161616161ULL,
    0x6060606060606060ULL,
    0x5f5f5f5f5f5f5f5fULL,
    0x5e5e5e5e5e5e5e5eULL,
    0x5d5d5d5d5d5d5d5dULL,
    0x5c5c5c5c5c5c5c5cULL,
    0x5b5b5b5b5b5b5b5bULL,
    0x5a5a5a5a5a5a5a5aULL,
    0x5959595959595959ULL,
    0x5858585858585858ULL,
    0x5757575757575757ULL,
    0x5656565656565656ULL,
    0x5555555555555555ULL,
    0x5454545454545454ULL,
    0x5353535353535353ULL,
    0x5252525252525252ULL,
    0x5151515151515151ULL,
    0x5050505050505050ULL,
    0x4f4f4f4f4f4f4f4fULL,
    0x4e4e4e4e4e4e4e4eULL,
    0x4d4d4d4d4d4d4d4dULL,
    0x4c4c4c4c4c4c4c4cULL,
    0x4b4b4b4b4b4b4b4bULL,
    0x4a4a4a4a4a4a4a4aULL,
    0x4949494949494949ULL,
    0x4848484848484848ULL,
    0x4747474747474747ULL,
    0x4646464646464646ULL,
    0x4545454545454545ULL,
    0x4444444444444444ULL,
    0x4343434343434343ULL,
    0x4242424242424242ULL,
    0x4141414141414141ULL,
    0x4040404040404040ULL
};


// Population count of a 64 bit integer in SWAR (SIMD within a register) style
// From Sebastiano Vigna, "Broadword Implementation of Rank/Select Queries"
// http://sux.dsi.unimi.it/paper.pdf p4
// This variant uses multiplication for the last summation instead of
// continuing the shift/mask/addition chain.
inline int suxpopcount(uint64 x) {
	// Step 1:  00 - 00 = 0;  01 - 00 = 01; 10 - 01 = 01; 11 - 01 = 10;
	x = x - ((x & G2) >> 1);
	// step 2:  add 2 groups of 2.
	x = (x & G4) + ((x >> 2) & G4);
	// 2 groups of 4.
	x = (x + (x >> 4)) & G8;
	// Using a multiply to collect the 8 groups of 8 together.
	x = x * L8 >> 56;
	return x;
}

// Default to using the GCC builtin popcount.  On architectures
// with -march popcnt, this compiles to a single popcnt instruction.
#ifndef popcount
#define popcount __builtin_popcountll
//#define popcount suxpopcount
#endif

#define popcountsize 64ULL
#define popcountmask (popcountsize - 1)

inline uint64 popcountLinear(uint64 *bits, uint64 x, uint64 nbits) {
	if (nbits == 0) { return 0; }
	uint64 lastword = (nbits - 1) / popcountsize;
	uint64 p = 0;

	for (int i = 0; i < lastword; i++) { /* tested;  manually unrolling doesn't help, at least in C */
		p += popcount(bits[x+i]); // note that use binds us to 64 bit popcount impls
	}

	// 'nbits' may or may not fall on a multiple of 64 boundary,
	// so we may need to zero out the right side of the last word
	// (accomplished by shifting it right, since we're just popcounting)
	uint64 lastshifted = bits[x+lastword] >> (63 - ((nbits - 1) & popcountmask));
	p += popcount(lastshifted);
	return p;
}

// Return the index of the kth bit set in x 
inline int select64_naive(uint64 x, int k) {
	int count = -1;
	for (int i = 63; i >= 0; i--) {
		count++;
		if (x & (1ULL << i)) {
			k--;
			if (k == 0) {
				return count;
			}
		}
	}
	return -1;
}

// Returns the position of the rank'th 1.  (rank = 0 returns the 1st 1)
// Returns 64 if there are fewer than rank+1 1s.
inline uint64_t select64_pdep_tzcnt(uint64_t val, int rank) {
	//struct timeval tv_start, tv_end;
	//gettimeofday(&tv_start, NULL);
	uint64_t i = 1ULL << rank;
	asm("pdep %[val], %[mask], %[val]"
			: [val] "+r" (val)
			: [mask] "r" (i));
	asm("tzcnt %[bit], %[index]"
			: [index] "=r" (i)
			: [bit] "g" (val)
			: "cc");
	//gettimeofday(&tv_end, NULL);
	//double elapsed_seconds = timeval_diff(&tv_start, &tv_end);
	//printf("Time to perform select 64: %.2f seconds\n", elapsed_seconds);
	return i;
}

inline uint32 _sel(uint64 x, uint32 i)
{
	uint64 s = x, b;  // s = sum
	s = s-((s>>1) & 0x5555555555555555ULL);
	s = (s & 0x3333333333333333ULL) + ((s >> 2) & 0x3333333333333333ULL);
	s = (s + (s >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
	s = 0x0101010101010101ULL*s;
	b = (s+ps_overflow[i]);//&0x8080808080808080ULL;// add something to the partial sums to cause overflow
	i = (i-1)<<8;
	if (b&0x0000000080000000ULL) // byte <=3
		if (b&0x0000000000008000ULL) //byte <= 1
			if (b&0x0000000000000080ULL)
				return    lt_sel[(x&0xFFULL) + i];
			else
				return 8 +lt_sel[(((x>>8)&0xFFULL)  + i - ((s&0xFFULL)<<8))&0x7FFULL];//byte 1;
		else//byte >1
			if (b&0x0000000000800000ULL) //byte <=2
				return 16+lt_sel[(((x>>16)&0xFFULL) + i - (s&0xFF00ULL))&0x7FFULL];//byte 2;
			else
				return 24+lt_sel[(((x>>24)&0xFFULL) + i - ((s>>8)&0xFF00ULL))&0x7FFULL];//byte 3;
	else//  byte > 3
		if (b&0x0000800000000000ULL) // byte <=5
			if (b&0x0000008000000000ULL) //byte <=4
				return 32+lt_sel[(((x>>32)&0xFFULL) + i - ((s>>16)&0xFF00ULL))&0x7FFULL];//byte 4;
			else
				return 40+lt_sel[(((x>>40)&0xFFULL) + i - ((s>>24)&0xFF00ULL))&0x7FFULL];//byte 5;
		else// byte >5
			if (b&0x0080000000000000ULL) //byte<=6
				return 48+lt_sel[(((x>>48)&0xFFULL) + i - ((s>>32)&0xFF00ULL))&0x7FFULL];//byte 6;
			else
				return 56+lt_sel[(((x>>56)&0xFFULL) + i - ((s>>40)&0xFF00ULL))&0x7FFULL];//byte 7;
	return 0;
}

inline uint32 select64_sdsl(uint64 x, uint32 i) {
#ifdef __SSE4_2__
	uint64_t s = x, b;
	s = s-((s>>1) & 0x5555555555555555ULL);
	s = (s & 0x3333333333333333ULL) + ((s >> 2) & 0x3333333333333333ULL);
	s = (s + (s >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
	s = 0x0101010101010101ULL*s;
	// now s contains 8 bytes s[7],...,s[0]; s[j] contains the cumulative sum
	// of (j+1)*8 least significant bits of s
	b = (s+ps_overflow[i]) & 0x8080808080808080ULL;
	// ps_overflow contains a bit mask x consisting of 8 bytes
	// x[7],...,x[0] and x[j] is set to 128-j
	// => a byte b[j] in b is >= 128 if cum sum >= j

	// __builtin_ctzll returns the number of trailing zeros, if b!=0
	int  byte_nr = __builtin_ctzll(b) >> 3;   // byte nr in [0..7]
	s <<= 8;
	i -= (s >> (byte_nr<<3)) & 0xFFULL;
	return (byte_nr << 3) + lt_sel[((i-1) << 8) + ((x>>(byte_nr<<3))&0xFFULL) ];
#endif
	return _sel(x, i);
}

inline int select64_popcount_search(uint64 x, int k) {
	//struct timeval tv_start, tv_end;
	//gettimeofday(&tv_start, NULL);
	int loc = -1;
	// if (k > popcount(x)) { return -1; }

	for (int testbits = 32; testbits > 0; testbits >>= 1) {
		int lcount = popcount(x >> testbits);
		if (k > lcount) {
			x &= ((1ULL << testbits)-1);
			loc += testbits;
			k -= lcount;
		} else {
			x >>= testbits;
		}
	}
	//gettimeofday(&tv_end, NULL);
	//double elapsed_seconds = timeval_diff(&tv_start, &tv_end);
	//printf("Time to perform select 64: %.2f seconds\n", elapsed_seconds);
	return loc+k;
}

inline int select64_broadword(uint64 x, int k) {
	uint64 word = x;
	int residual = k;
	register uint64 byte_sums;

	byte_sums = word - ( ( word & 0xa * ONES_STEP_4 ) >> 1 );
	byte_sums = ( byte_sums & 3 * ONES_STEP_4 ) + ( ( byte_sums >> 2 ) & 3 * ONES_STEP_4 );
	byte_sums = ( byte_sums + ( byte_sums >> 4 ) ) & 0x0f * ONES_STEP_8;
	byte_sums *= ONES_STEP_8;

	// Phase 2: compare each byte sum with the residual
	const uint64 residual_step_8 = residual * ONES_STEP_8;
	const int place = ( LEQ_STEP_8( byte_sums, residual_step_8 ) * ONES_STEP_8 >> 53 ) & ~0x7;

	// Phase 3: Locate the relevant byte and make 8 copies with incremental masks
	const int byte_rank = residual - ( ( ( byte_sums << 8 ) >> place ) & 0xFF );

	const uint64 spread_bits = ( word >> place & 0xFF ) * ONES_STEP_8 & INCR_STEP_8;
	const uint64 bit_sums = ZCOMPARE_STEP_8( spread_bits ) * ONES_STEP_8;

	// Compute the inside-byte location and return the sum
	const uint64 byte_rank_step_8 = byte_rank * ONES_STEP_8;

	return place + ( LEQ_STEP_8( bit_sums, byte_rank_step_8 ) * ONES_STEP_8 >> 56 );   
}

inline int select64(uint64 x, int k) {
	return select64_sdsl(x, k);
	//return select64_pdep_tzcnt(x, k);
	//return select64_popcount_search(x, k);
	//return select64_broadword(x, k);
	//return 0;
}

// x is the starting offset of the 512 bits;
// k is the thing we're selecting for.
inline int select512(uint64 *bits, int x, int k) {
	//struct timeval tv_start, tv_end;
	//gettimeofday(&tv_start, NULL);
	
	__asm__ __volatile__ (
									"prefetchnta (%0)\n"
									: : "r" (&bits[x]) );
	int i = 0;
	int pop = popcount(bits[x+i]);
	while (k > pop && i < 7) {
		k -= pop;
		i++;
		pop = popcount(bits[x+i]);
	}
	if (i == 7 && popcount(bits[x+i]) < k) {
		return -1;
	}
	//gettimeofday(&tv_end, NULL);
	//double elapsed_seconds = timeval_diff(&tv_start, &tv_end);
	//printf("Time to perform aux 512: %.2f seconds\n", elapsed_seconds);
	// We're now certain that the bit we want is stored in bv[x+i]
	return i*64 + select64(bits[x+i], k);
}

// brute-force linear select
// x is the starting offset of the bits in bv;
// k is the thing we're selecting for (starting from bv[x]).
// bvlen is the total length of bv
inline uint64 selectLinear(uint64* bits, uint64 length, uint64 x, uint64 k) {
	if (k > (length - x) * 64)
		return -1;
	uint64 i = 0;
	uint64 pop = popcount(bits[x+i]);
	while (k > pop && i < (length - 1)) {
		k -= pop;
		i++;
		pop = popcount(bits[x+i]);
	}
	if ((i == length - 1) && (pop < k)) {
		return -1;
	}
	// We're now certain that the bit we want is stored in bits[x+i]
	return i*64 + select64(bits[x+i], k);
}

#endif /* _FASTRANK_POPCOUNT_H_ */
