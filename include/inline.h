#ifndef __INLINE_H__
#define __INLINE_H__

#if defined(_MSC_VER)

#define INLINE    __forceinline

#else

#define INLINE inline __attribute__((always_inline))

#endif

#endif