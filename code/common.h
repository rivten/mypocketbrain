// NOTE(hugo): This file is _VERY_ largely taken from gb.h
// But there is an issue with using gb.h with raylib :(
// Indeed, gb.h includes windows.h on Win32 platform
// but raylib redefines some symbols that are already presents in this file...
// So I just decided to copy pastes the parts that interests me :)
#include <stdio.h>

#if defined(_WIN64) || defined(__x86_64__) || defined(_M_X64) || defined(__64BIT__) || defined(__powerpc64__) || defined(__ppc64__)
	#ifndef ARCH_64_BIT
	#define ARCH_64_BIT 1
	#endif
#else
	// NOTE(bill): I'm only supporting 32 bit and 64 bit systems
	#ifndef ARCH_32_BIT
	#define ARCH_32_BIT 1
	#endif
#endif

#if defined(_WIN32) || defined(_WIN64)
	#ifndef SYSTEM_WINDOWS
	#define SYSTEM_WINDOWS 1
	#endif
#elif defined(__APPLE__) && defined(__MACH__)
	#ifndef SYSTEM_OSX
	#define SYSTEM_OSX 1
	#endif
#elif defined(__unix__)
	#ifndef SYSTEM_UNIX
	#define SYSTEM_UNIX 1
	#endif

	#if defined(__linux__)
		#ifndef SYSTEM_LINUX
		#define SYSTEM_LINUX 1
		#endif
	#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
		#ifndef SYSTEM_FREEBSD
		#define SYSTEM_FREEBSD 1
		#endif
	#else
		#error This UNIX operating system is not supported
	#endif
#else
	#error This operating system is not supported
#endif

#if defined(_MSC_VER)
	#define COMPILER_MSVC 1
#elif defined(__GNUC__)
	#define COMPILER_GCC 1
#elif defined(__clang__)
	#define COMPILER_CLANG 1
#else
	#error Unknown compiler
#endif
////////////////////////////////////////////////////////////////
//
// Base Types
//
//

#if defined(COMPILER_MSVC)
	#if _MSC_VER < 1300
	typedef unsigned char     u8;
	typedef   signed char     i8;
	typedef unsigned short   u16;
	typedef   signed short   i16;
	typedef unsigned int     u32;
	typedef   signed int     i32;
	#else
	typedef unsigned __int8   u8;
	typedef   signed __int8   i8;
	typedef unsigned __int16 u16;
	typedef   signed __int16 i16;
	typedef unsigned __int32 u32;
	typedef   signed __int32 i32;
	#endif
	typedef unsigned __int64 u64;
	typedef   signed __int64 i64;
#else
	#include <stdint.h>
	typedef uint8_t   u8;
	typedef  int8_t   i8;
	typedef uint16_t u16;
	typedef  int16_t i16;
	typedef uint32_t u32;
	typedef  int32_t i32;
	typedef uint64_t u64;
	typedef  int64_t i64;
#endif

typedef size_t    usize;
typedef ptrdiff_t isize;

typedef float  f32;
typedef double f64;

typedef i32 Rune; // NOTE(bill): Unicode codepoint
#define RUNE_INVALID cast(Rune)(0xfffd)
#define RUNE_MAX     cast(Rune)(0x0010ffff)
#define RUNE_BOM     cast(Rune)(0xfeff)
#define RUNE_EOF     cast(Rune)(-1)


typedef i8  b8;
typedef i16 b16;
typedef i32 b32; // NOTE(bill): Prefer this!!!

// NOTE(bill): Get true and false
#if !defined(__cplusplus)
	#if (defined(_MSC_VER) && _MSC_VER < 1800) || (!defined(_MSC_VER) && !defined(__STDC_VERSION__))
		#ifndef true
		#define true  (0 == 0)
		#endif
		#ifndef false
		#define false (0 != 0)
		#endif
		typedef b8 bool;
	#else
		#include <stdbool.h>
	#endif
#endif

// NOTE(bill): These do are not prefixed with gb because the types are not.
#ifndef U8_MIN
#define U8_MIN 0u
#define U8_MAX 0xffu
#define I8_MIN (-0x7f - 1)
#define I8_MAX 0x7f

#define U16_MIN 0u
#define U16_MAX 0xffffu
#define I16_MIN (-0x7fff - 1)
#define I16_MAX 0x7fff

#define U32_MIN 0u
#define U32_MAX 0xffffffffu
#define I32_MIN (-0x7fffffff - 1)
#define I32_MAX 0x7fffffff

#define U64_MIN 0ull
#define U64_MAX 0xffffffffffffffffull
#define I64_MIN (-0x7fffffffffffffffll - 1)
#define I64_MAX 0x7fffffffffffffffll

#if defined(ARCH_32_BIT)
	#define USIZE_MIX U32_MIN
	#define USIZE_MAX U32_MAX

	#define ISIZE_MIX S32_MIN
	#define ISIZE_MAX S32_MAX
#elif defined(ARCH_64_BIT)
	#define USIZE_MIX U64_MIN
	#define USIZE_MAX U64_MAX

	#define ISIZE_MIX I64_MIN
	#define ISIZE_MAX I64_MAX
#else
	#error Unknown architecture size. This library only supports 32 bit and 64 bit architectures.
#endif

#define F32_MIN 1.17549435e-38f
#define F32_MAX 3.40282347e+38f

#define F64_MIN 2.2250738585072014e-308
#define F64_MAX 1.7976931348623157e+308

#endif

#ifndef NULL
	#if defined(__cplusplus)
		#if __cplusplus >= 201103L
			#define NULL nullptr
		#else
			#define NULL 0
		#endif
	#else
		#define NULL ((void *)0)
	#endif
#endif

// NOTE(bill): Easy to grep
// NOTE(bill): Not needed in macros
#ifndef cast
#define cast(Type) (Type)
#endif

// NOTE(bill): Because a signed sizeof is more useful
#ifndef size_of
#define size_of(x) (isize)(sizeof(x))
#endif

#ifndef count_of
#define count_of(x) ((size_of(x)/size_of(0[x])) / ((isize)(!(size_of(x) % size_of(0[x])))))
#endif

#ifndef offset_of
#define offset_of(Type, element) ((isize)&(((Type *)0)->element))
#endif

#if defined(__cplusplus)
#ifndef align_of
	#if __cplusplus >= 201103L
		#define align_of(Type) (isize)alignof(Type)
	#else
extern "C++" {
		// NOTE(bill): Fucking Templates!
		template <typename T> struct gbAlignment_Trick { char c; T member; };
		#define align_of(Type) offset_of(gbAlignment_Trick<Type>, member)
}
	#endif
#endif
#else
	#ifndef align_of
	#define align_of(Type) offset_of(struct { char c; Type member; }, member)
	#endif
#endif

////////////////////////////////////////////////////////////////
//
// Macro Fun!
//
//

#ifndef JOIN_MACROS
#define JOIN_MACROS
	#define JOIN2_IND(a, b) a##b

	#define JOIN2(a, b)       JOIN2_IND(a, b)
	#define JOIN3(a, b, c)    JOIN2(JOIN2(a, b), c)
	#define JOIN4(a, b, c, d) JOIN2(JOIN2(JOIN2(a, b), c), d)
#endif


#ifndef BIT
#define BIT(x) (1<<(x))
#endif

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef min3
#define min3(a, b, c) min(min(a, b), c)
#endif

#ifndef max3
#define max3(a, b, c) max(max(a, b), c)
#endif

#ifndef clamp
#define clamp(x, lower, upper) min(max((x), (lower)), (upper))
#endif

#ifndef clamp01
#define clamp01(x) clamp((x), 0, 1)
#endif

#ifndef is_between
#define is_between(x, lower, upper) (((lower) <= (x)) && ((x) <= (upper)))
#endif

#ifndef abs
#define abs(x) ((x) < 0 ? -(x) : (x))
#endif

/* NOTE(bill): Very useful bit setting */
#ifndef MASK_SET
#define MASK_SET(var, set, mask) do { \
	if (set) (var) |=  (mask); \
	else     (var) &= ~(mask); \
} while (0)
#endif


// NOTE(bill): Because static means 3/4 different things in C/C++. Great design (!)
#ifndef global
#define global        static // Global variables
#define internal      static // Internal linkage
#define local_persist static // Local Persisting variables
#endif

// NOTE(bill): Some compilers support applying printf-style warnings to user functions.
#if defined(__clang__) || defined(__GNUC__)
#define PRINTF_ARGS(FMT) __attribute__((format(printf, FMT, (FMT+1))))
#else
#define PRINTF_ARGS(FMT)
#endif

////////////////////////////////////////////////////////////////
//
// Debug
//
//


#ifndef DEBUG_TRAP
	#if defined(_MSC_VER)
	 	#if _MSC_VER < 1300
		#define DEBUG_TRAP() __asm int 3 /* Trap to debugger! */
		#else
		#define DEBUG_TRAP() __debugbreak()
		#endif
	#else
		#define DEBUG_TRAP() __builtin_trap()
	#endif
#endif

#ifndef ASSERT_MSG
#define ASSERT_MSG(cond, msg, ...) do { \
	if (!(cond)) { \
		assert_handler("Assertion Failure", #cond, __FILE__, cast(i64)__LINE__, msg, ##__VA_ARGS__); \
		DEBUG_TRAP(); \
	} \
} while (0)
#endif

#ifndef ASSERT
#define ASSERT(cond) ASSERT_MSG(cond, NULL)
#endif

#ifndef ASSERT_NOT_NULL
#define ASSERT_NOT_NULL(ptr) ASSERT_MSG((ptr) != NULL, #ptr " must not be NULL")
#endif

// NOTE(bill): Things that shouldn't happen with a message!
#ifndef PANIC
#define PANIC(msg, ...) do { \
	assert_handler("Panic", NULL, __FILE__, cast(i64)__LINE__, msg, ##__VA_ARGS__); \
	DEBUG_TRAP(); \
} while (0)
#endif

void assert_handler(char const *prefix, char const *condition, char const *file, i32 line, char const *msg, ...);


#ifdef COMMON_IMPLEMENTATION

void assert_handler(char const *prefix, char const *condition, char const *file, i32 line, char const *msg, ...) {
	printf("%s(%d): %s: ", file, line, prefix);
	if (condition)
		printf( "`%s` ", condition);
	if (msg) {
		va_list va;
		va_start(va, msg);
		printf(msg, va);
		va_end(va);
	}
	printf("\n");
}

#endif
