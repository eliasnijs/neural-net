////////////////////////////////////////////////////////////////////////////////
//// Memory Arena

/* Memory Arena:
 * One big container, allocations always happen at the end.
 * The memory is freed all at once. (memory is persistent until
 * the arena is destroyed) */

/* Example Usage:
 *
 * | Arena arena;
 * | U8 arena_buffer[1024];
 * | arena_init(&arena, arena_buffer, sizeof(arena_buffer));
 * |
 * | U32 *ptr = (U32 *)arena_alloc(&arena, sizeof(U32));
 * | *ptr = 42;
 * |
 * | arena_reset(&arena);
 * |
 * | U32 *ptr2 = (U32 *)arena_alloc(&arena, sizeof(U32));
 * | *ptr2 = 42;
 * |
 * | // ptr == ptr2
 *
 */

struct Arena {
	U8  *buf;          /* ptr to the reserved memory bufer for the arena */
	U32 len;           /* length of the reserved memory */
	U32 offset_curr;   /* current offset into the bufer */
	U32 offset_prev;   /* previous offset into the bufer */
};

/* default memory alignment, = 16, checkout (`Streaming
 * SIMD Extensions` for the why) */
#ifndef MEM_DEFAULT_ALIGNMENT
#define MEM_DEFAULT_ALIGNMENT (2*sizeof(void *))
#endif

////////////////////////////////////////////////////////////////////////////////
//// Helper Functions

/* returns if an address is on a power off 2 */
internal B32
is_pow_of_two(uintptr_t addr) {
	return (addr & (addr - 1)) == 0;
}

/* push a pointer to the next aligned value */
internal uintptr_t
align_forward(uintptr_t p, U64 align) {
	uintptr_t a, m;

	Assert( is_pow_of_two(align) );

	a = (uintptr_t)align;
	/* equivalent to p % a, but faster. (works because 'a' is a power of 2) */
	m = p & (a - 1);
	if (m)
		p += a - m;

	return p;
}

////////////////////////////////////////////////////////////////////////////////
//// Memory Arena Implementation

/* initialise an arena */
internal void
arena_init(Arena *a, void *backbuffer, U32 l) {
	a->buf = (U8 *)backbuffer;
	a->len = l;
	a->offset_curr = 0;
	a->offset_prev = 0;
	print_info("arena initialised w/ size %d", l);
}

/* destroy an arena */
internal inline void
arena_reset(Arena *a) {
	a->offset_curr = 0;
	a->offset_prev = 0;
}

/* allocate memory in a memory arena with alignment specified */
/* IMPORTANT(Elias): memory is not guaranteed to be 0! */
internal void *
arena_alloc_align(Arena *a, U32 len, U32 align) {
	if (a->offset_curr + len > a->len) {
		print_error("error: arena out of memory, allocation failed");
		return 0x0;
	}

	void *ptr = 0x0;
	uintptr_t ptr_curr = (uintptr_t)a->buf + (uintptr_t)a->offset_curr;
	uintptr_t offset = align_forward(ptr_curr, align) - (uintptr_t)a->buf;

	if (offset + len <= a->len)
	{
		ptr = &a->buf[offset];
		a->offset_prev = offset;
		a->offset_curr = offset + len;
	}

	return ptr;
}

/* allocate memory in a memory arena */
/* IMPORTANT(Elias): memory is not guaranteed to be 0! */
internal void *
arena_alloc(Arena *a, U32 len) {
	return arena_alloc_align(a, len, MEM_DEFAULT_ALIGNMENT);
}

////////////////////////////////////////////////////////////////////////////////
//// References

/* (c) This code is based on ideas and code from:
 * 1. Base [4]: Memory Management, Mr. 4th Programming by Allen Webster
 *    https://www.youtube.com/watch?v=L79vSP8yV2g
 * 2. Memory Allocation Strategies by GingerBill,
 *    https://www.gingerbill.org/series/memory-allocation-strategies/
 * 3. Handmade Hero eps034 by Casey Muratori
 *    https://www.youtube.com/watch?v=IJYTwhqfKLg
 * 4. Untangling Lifetimes: The Arena Allocator by Ryan Fleury
 *    https://www.rfleury.com/p/untangling-lifetimes-the-arena-allocator */

