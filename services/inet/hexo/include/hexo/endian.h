#ifndef HEXO_ENDIAN_H
#define HEXO_ENDIAN_H

#include <hexo/types.h>
#include <endian.h>

static inline uint16_t __endian_swap16(uint16_t x)
{
  return (x >> 8) | (x << 8);
}

static inline uint32_t __endian_swap32(uint32_t x)
{
  return (((x >> 24) & 0x000000ff) |
	  ((x >> 8 ) & 0x0000ff00) |
	  ((x << 8 ) & 0x00ff0000) |
	  ((x << 24) & 0xff000000));
}

#define __ENDIAN_REVERSE_ARGS(b01, b02, b03, b04, b05, b06, b07, b08, b09, b10, b11, b12, b13, b14, b15, b16, ...)    \
			       b16; b15; b14; b13; b12; b11; b10; b09; b08; b07; b06; b05; b04; b03; b02; b01;

#define __ENDIAN_ARGS(b01, b02, b03, b04, b05, b06, b07, b08, b09, b10, b11, b12, b13, b14, b15, b16, ...)    \
			       b01; b02; b03; b04; b05; b06; b07; b08; b09; b10; b11; b12; b13; b14; b15; b16;

#if __BYTE_ORDER == __LITTLE_ENDIAN
#  define endian_le16(x)	(x)
#  define endian_le32(x)	(x)
#  define endian_le64(x)	(x)
#  define endian_be16(x)	__endian_swap16(x)
#  define endian_be32(x)	__endian_swap32(x)

#  define ENDIAN_BITFIELD(...)	__ENDIAN_REVERSE_ARGS(__VA_ARGS__,,,,,,,,,,,,,,,)
#else

#  define endian_be16(x)	(x)
#  define endian_be32(x)	(x)
#  define endian_be64(x)	(x)
#  define endian_le16(x)	__endian_swap16(x)
#  define endian_le32(x)	__endian_swap32(x)

#  define ENDIAN_BITFIELD(...)	__ENDIAN_ARGS(__VA_ARGS__,,,,,,,,,,,,,,,)

#endif

# define endian_16_na_load(a)		(*((uint16_t*)a))
# define endian_32_na_load(a)		(*((uint32_t*)a))
# define endian_16_na_store(a, x)	(*((uint16_t*)a) = (x))
# define endian_32_na_store(a, x)	(*((uint32_t*)a) = (x))

# define __endian_16_na_load(a)		endian_16_na_load(a)
# define __endian_32_na_load(a)		endian_32_na_load(a)
# define __endian_16_na_store(a, x)	endian_16_na_store(a, x)
# define __endian_32_na_store(a, x)	endian_32_na_store(a, x)

/* direct non aligned word width memory access with endian perm */
# define endian_le16_na_load(a)		endian_le16(endian_16_na_load(a))
# define endian_le32_na_load(a)		endian_le32(endian_32_na_load(a))
# define endian_le16_na_store(a, x)	endian_16_na_store(a, endian_le16(x))
# define endian_le32_na_store(a, x)	endian_32_na_store(a, endian_le32(x))

# define endian_be16_na_load(a)		endian_be16(endian_16_na_load(a))
# define endian_be32_na_load(a)		endian_be32(endian_32_na_load(a))
# define endian_be16_na_store(a, x)	endian_16_na_store(a, endian_be16(x))
# define endian_be32_na_store(a, x)	endian_32_na_store(a, endian_be32(x))

#endif
