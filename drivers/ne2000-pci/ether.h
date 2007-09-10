/* Copyright (C) 1997, 1999, 2001 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

/* Based on the FreeBSD version of this file. Curiously, that file
   lacks a copyright in the header. */

#ifndef NETINET_ETHER_H_
#define NETINET_ETHER_H_

#include <stdint.h>

/*
 *	IEEE 802.3 Ethernet magic constants.  The frame sizes omit the preamble
 *	and FCS/CRC (frame check sequence).
 */

#define ETH_ALEN	6		/* Octets in one ethernet addr	 */
#define ETH_HLEN	14		/* Total octets in header.	 */
#define ETH_ZLEN	60		/* Min. octets in frame sans FCS */
#define ETH_DATA_LEN	1500		/* Max. octets in payload	 */
#define ETH_FRAME_LEN	1514		/* Max. octets in frame sans FCS */

/* This is a name for the 48 bit ethernet address available on many
   systems.  */
struct		ether_addr
{
  uint8_t	ether_addr_octet[ETH_ALEN];
} __attribute__ ((packed));

/* 10Mb/s ethernet header */
struct		ether_header
{
  uint8_t	ether_dhost[ETH_ALEN];	/* destination eth addr	*/
  uint8_t	ether_shost[ETH_ALEN];	/* source ether addr	*/
  uint16_t	ether_type;	        /* packet type ID field	*/
} __attribute__ ((packed));

/* Ethernet protocol ID's */
#define	ETHERTYPE_PUP		0x0200          /* Xerox PUP */
#define	ETHERTYPE_IP		0x0800		/* IP */
#define	ETHERTYPE_ARP		0x0806		/* Address resolution */
#define	ETHERTYPE_REVARP	0x8035		/* Reverse ARP */

/* For PF_PACKET */
#define ETH_P_ALL		0x0003		/* Every packet (be careful!!!) */
#define ETH_P_IP		ETHERTYPE_IP
#define ETH_P_ARP		ETHERTYPE_ARP
#define ETH_P_RARP		ETHERTYPE_REVARP

/* Some size constants */
#define	ETHER_ADDR_LEN	ETH_ALEN                 /* size of ethernet addr */
#define	ETHER_TYPE_LEN	2                        /* bytes in type field */
#define	ETHER_CRC_LEN	4                        /* bytes in CRC field */
#define	ETHER_HDR_LEN	ETH_HLEN                 /* total octets in header */
#define	ETHER_MIN_LEN	(ETH_ZLEN + ETHER_CRC_LEN) /* min packet length */
#define	ETHER_MAX_LEN	(ETH_FRAME_LEN + ETHER_CRC_LEN) /* max packet length */

#define	ETHERMTU	ETH_DATA_LEN
#define	ETHERMIN	(ETHER_MIN_LEN - ETHER_HDR_LEN - ETHER_CRC_LEN)

/* make sure ethenet length is valid */
#define	ETHER_IS_VALID_LEN(foo)	\
	((foo) >= ETHER_MIN_LEN && (foo) <= ETHER_MAX_LEN)

#endif

