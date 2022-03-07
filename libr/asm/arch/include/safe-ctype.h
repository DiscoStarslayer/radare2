/* <ctype.h> replacement macros.

   Copyright (C) 2000, 2001 Free Software Foundation, Inc.
   Contributed by Zack Weinberg <zackw@stanford.edu>.

This file is part of the libiberty library.
Libiberty is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

Libiberty is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with libiberty; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
Boston, MA 02110-1301, USA.  */

/* This is a compatible replacement of the standard C library's <ctype.h>
   with the following properties:

   - Implements all isxxx() macros required by C99.
   - Also implements some character classes useful when
     parsing C-like languages.
   - Does not change behavior depending on the current locale.
   - Behaves properly for all values in the range of a signed or
     unsigned char.

   To avoid conflicts, this header defines the isxxx functions in upper
   case, e.g. ISALPHA not isalpha.  */

#ifndef SAFE_CTYPE_H
#define SAFE_CTYPE_H

/* Determine host character set.  */
#define HOST_CHARSET_UNKNOWN 0
#define HOST_CHARSET_ASCII   1
#define HOST_CHARSET_EBCDIC  2

#if  '\n' == 0x0A && ' ' == 0x20 && '0' == 0x30 \
   && 'A' == 0x41 && 'a' == 0x61 && '!' == 0x21
#  define HOST_CHARSET HOST_CHARSET_ASCII
#else
# if '\n' == 0x15 && ' ' == 0x40 && '0' == 0xF0 \
   && 'A' == 0xC1 && 'a' == 0x81 && '!' == 0x5A
#  define HOST_CHARSET HOST_CHARSET_EBCDIC
# else
#  define HOST_CHARSET HOST_CHARSET_UNKNOWN
# endif
#endif

/* Categories.  */

enum {
  /* In C99 */
  _sch_isblank  = 0x0001,	/* space \t */
  _sch_iscntrl  = 0x0002,	/* nonprinting characters */
  _sch_isdigit  = 0x0004,	/* 0-9 */
  _sch_islower  = 0x0008,	/* a-z */
  _sch_isprint  = 0x0010,	/* any printing character including ' ' */
  _sch_ispunct  = 0x0020,	/* all punctuation */
  _sch_isspace  = 0x0040,	/* space \t \n \r \f \v */
  _sch_isupper  = 0x0080,	/* A-Z */
  _sch_isxdigit = 0x0100,	/* 0-9A-Fa-f */

  /* Extra categories useful to cpplib.  */
  _sch_isidst	= 0x0200,	/* A-Za-z_ */
  _sch_isvsp    = 0x0400,	/* \n \r */
  _sch_isnvsp   = 0x0800,	/* space \t \f \v \0 */

  /* Combinations of the above.  */
  _sch_isalpha  = _sch_isupper|_sch_islower,	/* A-Za-z */
  _sch_isalnum  = _sch_isalpha|_sch_isdigit,	/* A-Za-z0-9 */
  _sch_isidnum  = _sch_isidst|_sch_isdigit,	/* A-Za-z0-9_ */
  _sch_isgraph  = _sch_isalnum|_sch_ispunct,	/* isprint and not space */
  _sch_iscppsp  = _sch_isvsp|_sch_isnvsp,	/* isspace + \0 */
  _sch_isbasic  = _sch_isprint|_sch_iscppsp     /* basic charset of ISO C
						   (plus ` and @)  */
};

/* Shorthand */
#define bl _sch_isblank
#define cn _sch_iscntrl
#define di _sch_isdigit
#define is _sch_isidst
#define lo _sch_islower
#define nv _sch_isnvsp
#define pn _sch_ispunct
#define pr _sch_isprint
#define sp _sch_isspace
#define up _sch_isupper
#define vs _sch_isvsp
#define xd _sch_isxdigit

/* Masks.  */
#define L  (const unsigned short) (lo|is   |pr)	/* lower case letter */
#define XL (const unsigned short) (lo|is|xd|pr)	/* lowercase hex digit */
#define U  (const unsigned short) (up|is   |pr)	/* upper case letter */
#define XU (const unsigned short) (up|is|xd|pr)	/* uppercase hex digit */
#define D  (const unsigned short) (di   |xd|pr)	/* decimal digit */
#define P  (const unsigned short) (pn      |pr)	/* punctuation */

#define C  (const unsigned short) (         cn)	/* control character */
#define Z  (const unsigned short) (nv      |cn)	/* NUL */
#define M  (const unsigned short) (nv|sp   |cn)	/* cursor movement: \f \v */
#define V  (const unsigned short) (vs|sp   |cn)	/* vertical space: \r \n */
#define T  (const unsigned short) (nv|sp|bl|cn)	/* tab */
#define S  (const unsigned short) (nv|sp|bl|pr)	/* space */

/* Character classification.  */
static const unsigned short _sch_istable[256] =
{
	Z,  C,  C,  C,   C,  C,  C,  C,   /* NUL SOH STX ETX  EOT ENQ ACK BEL */
	C,  T,  V,  M,   M,  V,  C,  C,   /* BS  HT  LF  VT   FF  CR  SO  SI  */
	C,  C,  C,  C,   C,  C,  C,  C,   /* DLE DC1 DC2 DC3  DC4 NAK SYN ETB */
	C,  C,  C,  C,   C,  C,  C,  C,   /* CAN EM  SUB ESC  FS  GS  RS  US  */
	S,  P,  P,  P,   P,  P,  P,  P,   /* SP  !   "   #    $   %   &   '   */
	P,  P,  P,  P,   P,  P,  P,  P,   /* (   )   *   +    ,   -   .   /   */
	D,  D,  D,  D,   D,  D,  D,  D,   /* 0   1   2   3    4   5   6   7   */
	D,  D,  P,  P,   P,  P,  P,  P,   /* 8   9   :   ;    <   =   >   ?   */
	P, XU, XU, XU,  XU, XU, XU,  U,   /* @   A   B   C    D   E   F   G   */
	U,  U,  U,  U,   U,  U,  U,  U,   /* H   I   J   K    L   M   N   O   */
	U,  U,  U,  U,   U,  U,  U,  U,   /* P   Q   R   S    T   U   V   W   */
	U,  U,  U,  P,   P,  P,  P,  (const unsigned short) (pn|is   |pr),   /* X   Y   Z   [    \   ]   ^   _   */
	P, XL, XL, XL,  XL, XL, XL,  L,   /* `   a   b   c    d   e   f   g   */
	L,  L,  L,  L,   L,  L,  L,  L,   /* h   i   j   k    l   m   n   o   */
	L,  L,  L,  L,   L,  L,  L,  L,   /* p   q   r   s    t   u   v   w   */
	L,  L,  L,  P,   P,  P,  P,  C,   /* x   y   z   {    |   }   ~   DEL */

	/* high half of unsigned char is locale-specific, so all tests are
	   false in "C" locale */
	0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
	0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
	0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
	0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,

	0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
	0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
	0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
	0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
};

#define _sch_test(c, bit) (_sch_istable[(c) & 0xff] & (unsigned short)(bit))

#define ISALPHA(c)  _sch_test(c, _sch_isalpha)
#define ISALNUM(c)  _sch_test(c, _sch_isalnum)
#define ISBLANK(c)  _sch_test(c, _sch_isblank)
#define ISCNTRL(c)  _sch_test(c, _sch_iscntrl)
#define ISDIGIT(c)  _sch_test(c, _sch_isdigit)
#define ISGRAPH(c)  _sch_test(c, _sch_isgraph)
#define ISLOWER(c)  _sch_test(c, _sch_islower)
#define ISPRINT(c)  _sch_test(c, _sch_isprint)
#define ISPUNCT(c)  _sch_test(c, _sch_ispunct)
#define ISSPACE(c)  _sch_test(c, _sch_isspace)
#define ISUPPER(c)  _sch_test(c, _sch_isupper)
#define ISXDIGIT(c) _sch_test(c, _sch_isxdigit)

#define ISIDNUM(c)	_sch_test(c, _sch_isidnum)
#define ISIDST(c)	_sch_test(c, _sch_isidst)
#define IS_ISOBASIC(c)	_sch_test(c, _sch_isbasic)
#define IS_VSPACE(c)	_sch_test(c, _sch_isvsp)
#define IS_NVSPACE(c)	_sch_test(c, _sch_isnvsp)
#define IS_SPACE_OR_NUL(c)	_sch_test(c, _sch_iscppsp)

/* Character transformation.  */
extern const unsigned char  _sch_toupper[256];
static const unsigned char _sch_tolower[256] =
{
	0,  1,  2,  3,   4,  5,  6,  7,   8,  9, 10, 11,  12, 13, 14, 15,
	16, 17, 18, 19,  20, 21, 22, 23,  24, 25, 26, 27,  28, 29, 30, 31,
	32, 33, 34, 35,  36, 37, 38, 39,  40, 41, 42, 43,  44, 45, 46, 47,
	48, 49, 50, 51,  52, 53, 54, 55,  56, 57, 58, 59,  60, 61, 62, 63,
	64,

	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',

	91, 92, 93, 94, 95, 96,

	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',

	123,124,125,126,127,

	128,129,130,131, 132,133,134,135, 136,137,138,139, 140,141,142,143,
	144,145,146,147, 148,149,150,151, 152,153,154,155, 156,157,158,159,
	160,161,162,163, 164,165,166,167, 168,169,170,171, 172,173,174,175,
	176,177,178,179, 180,181,182,183, 184,185,186,187, 188,189,190,191,

	192,193,194,195, 196,197,198,199, 200,201,202,203, 204,205,206,207,
	208,209,210,211, 212,213,214,215, 216,217,218,219, 220,221,222,223,
	224,225,226,227, 228,229,230,231, 232,233,234,235, 236,237,238,239,
	240,241,242,243, 244,245,246,247, 248,249,250,251, 252,253,254,255,
};
#define TOUPPER(c) _sch_toupper[(c) & 0xff]
#define TOLOWER(c) _sch_tolower[(c) & 0xff]

/* Prevent the users of safe-ctype.h from accidently using the routines
   from ctype.h.  Initially, the approach was to produce an error when
   detecting that ctype.h has been included.  But this was causing
   trouble as ctype.h might get indirectly included as a result of
   including another system header (for instance gnulib's stdint.h).
   So we include ctype.h here and then immediately redefine its macros.  */

#include <ctype.h>
#undef isalpha
#define isalpha(c) do_not_use_isalpha_with_safe_ctype
#undef isalnum
#define isalnum(c) do_not_use_isalnum_with_safe_ctype
#undef iscntrl
#define iscntrl(c) do_not_use_iscntrl_with_safe_ctype
#undef isdigit
#define isdigit(c) do_not_use_isdigit_with_safe_ctype
#undef isgraph
#define isgraph(c) do_not_use_isgraph_with_safe_ctype
#undef islower
#define islower(c) do_not_use_islower_with_safe_ctype
#undef isprint
#define isprint(c) do_not_use_isprint_with_safe_ctype
#undef ispunct
#define ispunct(c) do_not_use_ispunct_with_safe_ctype
#undef isspace
#define isspace(c) do_not_use_isspace_with_safe_ctype
#undef isupper
#define isupper(c) do_not_use_isupper_with_safe_ctype
#undef isxdigit
#define isxdigit(c) do_not_use_isxdigit_with_safe_ctype
#undef toupper
#define toupper(c) do_not_use_toupper_with_safe_ctype
#undef tolower
#define tolower(c) do_not_use_tolower_with_safe_ctype

#endif /* SAFE_CTYPE_H */
