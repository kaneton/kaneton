/*
 * This file is part of DSX, development environment for static
 * SoC applications.
 * 
 * This file is distributed under the terms of the GNU General Public
 * License.
 * 
 * Copyright (c) 2005, Nicolas Pouillon, <nipo@ssji.net>
 *     Laboratoire d'informatique de Paris 6 / ASIM, France
 * 
 *  $Id: dsx_endianness.h 152 2006-02-15 13:28:29Z nipo $
 */
#ifndef DSX_ENDIANNESS_H_
#define DSX_ENDIANNESS_H_

#define DSX_ENDIANNESS_INLINED
#define DSX_ENDIANNESS_STR "little-endian"
#define dsx_short_from_be(x) (((x)>>8)|(((x)&0xff)<<8))
#define dsx_short_to_be(x) (((x)>>8)|(((x)&0xff)<<8))
#define dsx_long_from_be(x) (((x)>>24)|(((x)&0xff0000)>>8)|(((x)&0xff00)<<8)|(((x)&0xff)<<24))
#define dsx_long_to_be(x) (((x)>>24)|(((x)&0xff0000)>>8)|(((x)&0xff00)<<8)|(((x)&0xff)<<24))
#define dsx_short_from_le(x) (x)
#define dsx_short_to_le(x) (x)
#define dsx_long_from_le(x) (x)
#define dsx_long_to_le(x) (x)

#endif /* DSX_ENDIANNESS_H_ */
