/*
 * Copyright (c) 2024 The cutils authors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "aes.h"

#include <string.h>

#define BYTE_0(x) ((uint8_t) ((x)         & 0xff))
#define BYTE_1(x) ((uint8_t) (((x) >>  8) & 0xff))
#define BYTE_2(x) ((uint8_t) (((x) >> 16) & 0xff))
#define BYTE_3(x) ((uint8_t) (((x) >> 24) & 0xff))
#define BYTE_4(x) ((uint8_t) (((x) >> 32) & 0xff))
#define BYTE_5(x) ((uint8_t) (((x) >> 40) & 0xff))
#define BYTE_6(x) ((uint8_t) (((x) >> 48) & 0xff))
#define BYTE_7(x) ((uint8_t) (((x) >> 56) & 0xff))

#define FT \
\
    V(a5, 63, 63, c6), V(84, 7c, 7c, f8), V(99, 77, 77, ee), V(8d, 7b, 7b, f6), \
    V(0d, f2, f2, ff), V(bd, 6b, 6b, d6), V(b1, 6f, 6f, de), V(54, c5, c5, 91), \
    V(50, 30, 30, 60), V(03, 01, 01, 02), V(a9, 67, 67, ce), V(7d, 2b, 2b, 56), \
    V(19, fe, fe, e7), V(62, d7, d7, b5), V(e6, ab, ab, 4d), V(9a, 76, 76, ec), \
    V(45, ca, ca, 8f), V(9d, 82, 82, 1f), V(40, c9, c9, 89), V(87, 7d, 7d, fa), \
    V(15, fa, fa, ef), V(eb, 59, 59, b2), V(c9, 47, 47, 8e), V(0b, f0, f0, fb), \
    V(ec, ad, ad, 41), V(67, d4, d4, b3), V(fd, a2, a2, 5f), V(ea, af, af, 45), \
    V(bf, 9c, 9c, 23), V(f7, a4, a4, 53), V(96, 72, 72, e4), V(5b, c0, c0, 9b), \
    V(c2, b7, b7, 75), V(1c, fd, fd, e1), V(ae, 93, 93, 3d), V(6a, 26, 26, 4c), \
    V(5a, 36, 36, 6c), V(41, 3f, 3f, 7e), V(02, f7, f7, f5), V(4f, cc, cc, 83), \
    V(5c, 34, 34, 68), V(f4, a5, a5, 51), V(34, e5, e5, d1), V(08, f1, f1, f9), \
    V(93, 71, 71, e2), V(73, d8, d8, ab), V(53, 31, 31, 62), V(3f, 15, 15, 2a), \
    V(0c, 04, 04, 08), V(52, c7, c7, 95), V(65, 23, 23, 46), V(5e, c3, c3, 9d), \
    V(28, 18, 18, 30), V(a1, 96, 96, 37), V(0f, 05, 05, 0a), V(b5, 9a, 9a, 2f), \
    V(09, 07, 07, 0e), V(36, 12, 12, 24), V(9b, 80, 80, 1b), V(3d, e2, e2, df), \
    V(26, eb, eb, cd), V(69, 27, 27, 4e), V(cd, b2, b2, 7f), V(9f, 75, 75, ea), \
    V(1b, 09, 09, 12), V(9e, 83, 83, 1d), V(74, 2c, 2c, 58), V(2e, 1a, 1a, 34), \
    V(2d, 1b, 1b, 36), V(b2, 6e, 6e, dc), V(ee, 5a, 5a, b4), V(fb, a0, a0, 5b), \
    V(f6, 52, 52, a4), V(4d, 3b, 3b, 76), V(61, d6, d6, b7), V(ce, b3, b3, 7d), \
    V(7b, 29, 29, 52), V(3e, e3, e3, dd), V(71, 2f, 2f, 5e), V(97, 84, 84, 13), \
    V(f5, 53, 53, a6), V(68, d1, d1, b9), V(00, 00, 00, 00), V(2c, ed, ed, c1), \
    V(60, 20, 20, 40), V(1f, fc, fc, e3), V(c8, b1, b1, 79), V(ed, 5b, 5b, b6), \
    V(be, 6a, 6a, d4), V(46, cb, cb, 8d), V(d9, be, be, 67), V(4b, 39, 39, 72), \
    V(de, 4a, 4a, 94), V(d4, 4c, 4c, 98), V(e8, 58, 58, b0), V(4a, cf, cf, 85), \
    V(6b, d0, d0, bb), V(2a, ef, ef, c5), V(e5, aa, aa, 4f), V(16, fb, fb, ed), \
    V(c5, 43, 43, 86), V(d7, 4d, 4d, 9a), V(55, 33, 33, 66), V(94, 85, 85, 11), \
    V(cf, 45, 45, 8a), V(10, f9, f9, e9), V(06, 02, 02, 04), V(81, 7f, 7f, fe), \
    V(f0, 50, 50, a0), V(44, 3c, 3c, 78), V(ba, 9f, 9f, 25), V(e3, a8, a8, 4b), \
    V(f3, 51, 51, a2), V(fe, a3, a3, 5d), V(c0, 40, 40, 80), V(8a, 8f, 8f, 05), \
    V(ad, 92, 92, 3f), V(bc, 9d, 9d, 21), V(48, 38, 38, 70), V(04, f5, f5, f1), \
    V(df, bc, bc, 63), V(c1, b6, b6, 77), V(75, da, da, af), V(63, 21, 21, 42), \
    V(30, 10, 10, 20), V(1a, ff, ff, e5), V(0e, f3, f3, fd), V(6d, d2, d2, bf), \
    V(4c, cd, cd, 81), V(14, 0c, 0c, 18), V(35, 13, 13, 26), V(2f, ec, ec, c3), \
    V(e1, 5f, 5f, be), V(a2, 97, 97, 35), V(cc, 44, 44, 88), V(39, 17, 17, 2e), \
    V(57, c4, c4, 93), V(f2, a7, a7, 55), V(82, 7e, 7e, fc), V(47, 3d, 3d, 7a), \
    V(ac, 64, 64, c8), V(e7, 5d, 5d, ba), V(2b, 19, 19, 32), V(95, 73, 73, e6), \
    V(a0, 60, 60, c0), V(98, 81, 81, 19), V(d1, 4f, 4f, 9e), V(7f, dc, dc, a3), \
    V(66, 22, 22, 44), V(7e, 2a, 2a, 54), V(ab, 90, 90, 3b), V(83, 88, 88, 0b), \
    V(ca, 46, 46, 8c), V(29, ee, ee, c7), V(d3, b8, b8, 6b), V(3c, 14, 14, 28), \
    V(79, de, de, a7), V(e2, 5e, 5e, bc), V(1d, 0b, 0b, 16), V(76, db, db, ad), \
    V(3b, e0, e0, db), V(56, 32, 32, 64), V(4e, 3a, 3a, 74), V(1e, 0a, 0a, 14), \
    V(db, 49, 49, 92), V(0a, 06, 06, 0c), V(6c, 24, 24, 48), V(e4, 5c, 5c, b8), \
    V(5d, c2, c2, 9f), V(6e, d3, d3, bd), V(ef, ac, ac, 43), V(a6, 62, 62, c4), \
    V(a8, 91, 91, 39), V(a4, 95, 95, 31), V(37, e4, e4, d3), V(8b, 79, 79, f2), \
    V(32, e7, e7, d5), V(43, c8, c8, 8b), V(59, 37, 37, 6e), V(b7, 6d, 6d, da), \
    V(8c, 8d, 8d, 01), V(64, d5, d5, b1), V(d2, 4e, 4e, 9c), V(e0, a9, a9, 49), \
    V(b4, 6c, 6c, d8), V(fa, 56, 56, ac), V(07, f4, f4, f3), V(25, ea, ea, cf), \
    V(af, 65, 65, ca), V(8e, 7a, 7a, f4), V(e9, ae, ae, 47), V(18, 08, 08, 10), \
    V(d5, ba, ba, 6f), V(88, 78, 78, f0), V(6f, 25, 25, 4a), V(72, 2e, 2e, 5c), \
    V(24, 1c, 1c, 38), V(f1, a6, a6, 57), V(c7, b4, b4, 73), V(51, c6, c6, 97), \
    V(23, e8, e8, cb), V(7c, dd, dd, a1), V(9c, 74, 74, e8), V(21, 1f, 1f, 3e), \
    V(dd, 4b, 4b, 96), V(dc, bd, bd, 61), V(86, 8b, 8b, 0d), V(85, 8a, 8a, 0f), \
    V(90, 70, 70, e0), V(42, 3e, 3e, 7c), V(c4, b5, b5, 71), V(aa, 66, 66, cc), \
    V(d8, 48, 48, 90), V(05, 03, 03, 06), V(01, f6, f6, f7), V(12, 0e, 0e, 1c), \
    V(a3, 61, 61, c2), V(5f, 35, 35, 6a), V(f9, 57, 57, ae), V(d0, b9, b9, 69), \
    V(91, 86, 86, 17), V(58, c1, c1, 99), V(27, 1d, 1d, 3a), V(b9, 9e, 9e, 27), \
    V(38, e1, e1, d9), V(13, f8, f8, eb), V(b3, 98, 98, 2b), V(33, 11, 11, 22), \
    V(bb, 69, 69, d2), V(70, d9, d9, a9), V(89, 8e, 8e, 07), V(a7, 94, 94, 33), \
    V(b6, 9b, 9b, 2d), V(22, 1e, 1e, 3c), V(92, 87, 87, 15), V(20, e9, e9, c9), \
    V(49, ce, ce, 87), V(ff, 55, 55, aa), V(78, 28, 28, 50), V(7a, df, df, a5), \
    V(8f, 8c, 8c, 03), V(f8, a1, a1, 59), V(80, 89, 89, 09), V(17, 0d, 0d, 1a), \
    V(da, bf, bf, 65), V(31, e6, e6, d7), V(c6, 42, 42, 84), V(b8, 68, 68, d0), \
    V(c3, 41, 41, 82), V(b0, 99, 99, 29), V(77, 2d, 2d, 5a), V(11, 0f, 0f, 1e), \
    V(cb, b0, b0, 7b), V(fc, 54, 54, a8), V(d6, bb, bb, 6d), V(3a, 16, 16, 2c)

#define V(a, b, c, d) 0x##a##b##c##d
static const uint32_t FT0[256] = {FT};
#undef V

#define V(a, b, c, d) 0x##b##c##d##a
static const uint32_t FT1[256] = {FT};
#undef V

#define V(a, b, c, d) 0x##c##d##a##b
static const uint32_t FT2[256] = {FT};
#undef V

#define V(a, b, c, d) 0x##d##a##b##c
static const uint32_t FT3[256] = {FT};
#undef V

#undef FT

#define RT \
\
    V(50, a7, f4, 51), V(53, 65, 41, 7e), V(c3, a4, 17, 1a), V(96, 5e, 27, 3a), \
    V(cb, 6b, ab, 3b), V(f1, 45, 9d, 1f), V(ab, 58, fa, ac), V(93, 03, e3, 4b), \
    V(55, fa, 30, 20), V(f6, 6d, 76, ad), V(91, 76, cc, 88), V(25, 4c, 02, f5), \
    V(fc, d7, e5, 4f), V(d7, cb, 2a, c5), V(80, 44, 35, 26), V(8f, a3, 62, b5), \
    V(49, 5a, b1, de), V(67, 1b, ba, 25), V(98, 0e, ea, 45), V(e1, c0, fe, 5d), \
    V(02, 75, 2f, c3), V(12, f0, 4c, 81), V(a3, 97, 46, 8d), V(c6, f9, d3, 6b), \
    V(e7, 5f, 8f, 03), V(95, 9c, 92, 15), V(eb, 7a, 6d, bf), V(da, 59, 52, 95), \
    V(2d, 83, be, d4), V(d3, 21, 74, 58), V(29, 69, e0, 49), V(44, c8, c9, 8e), \
    V(6a, 89, c2, 75), V(78, 79, 8e, f4), V(6b, 3e, 58, 99), V(dd, 71, b9, 27), \
    V(b6, 4f, e1, be), V(17, ad, 88, f0), V(66, ac, 20, c9), V(b4, 3a, ce, 7d), \
    V(18, 4a, df, 63), V(82, 31, 1a, e5), V(60, 33, 51, 97), V(45, 7f, 53, 62), \
    V(e0, 77, 64, b1), V(84, ae, 6b, bb), V(1c, a0, 81, fe), V(94, 2b, 08, f9), \
    V(58, 68, 48, 70), V(19, fd, 45, 8f), V(87, 6c, de, 94), V(b7, f8, 7b, 52), \
    V(23, d3, 73, ab), V(e2, 02, 4b, 72), V(57, 8f, 1f, e3), V(2a, ab, 55, 66), \
    V(07, 28, eb, b2), V(03, c2, b5, 2f), V(9a, 7b, c5, 86), V(a5, 08, 37, d3), \
    V(f2, 87, 28, 30), V(b2, a5, bf, 23), V(ba, 6a, 03, 02), V(5c, 82, 16, ed), \
    V(2b, 1c, cf, 8a), V(92, b4, 79, a7), V(f0, f2, 07, f3), V(a1, e2, 69, 4e), \
    V(cd, f4, da, 65), V(d5, be, 05, 06), V(1f, 62, 34, d1), V(8a, fe, a6, c4), \
    V(9d, 53, 2e, 34), V(a0, 55, f3, a2), V(32, e1, 8a, 05), V(75, eb, f6, a4), \
    V(39, ec, 83, 0b), V(aa, ef, 60, 40), V(06, 9f, 71, 5e), V(51, 10, 6e, bd), \
    V(f9, 8a, 21, 3e), V(3d, 06, dd, 96), V(ae, 05, 3e, dd), V(46, bd, e6, 4d), \
    V(b5, 8d, 54, 91), V(05, 5d, c4, 71), V(6f, d4, 06, 04), V(ff, 15, 50, 60), \
    V(24, fb, 98, 19), V(97, e9, bd, d6), V(cc, 43, 40, 89), V(77, 9e, d9, 67), \
    V(bd, 42, e8, b0), V(88, 8b, 89, 07), V(38, 5b, 19, e7), V(db, ee, c8, 79), \
    V(47, 0a, 7c, a1), V(e9, 0f, 42, 7c), V(c9, 1e, 84, f8), V(00, 00, 00, 00), \
    V(83, 86, 80, 09), V(48, ed, 2b, 32), V(ac, 70, 11, 1e), V(4e, 72, 5a, 6c), \
    V(fb, ff, 0e, fd), V(56, 38, 85, 0f), V(1e, d5, ae, 3d), V(27, 39, 2d, 36), \
    V(64, d9, 0f, 0a), V(21, a6, 5c, 68), V(d1, 54, 5b, 9b), V(3a, 2e, 36, 24), \
    V(b1, 67, 0a, 0c), V(0f, e7, 57, 93), V(d2, 96, ee, b4), V(9e, 91, 9b, 1b), \
    V(4f, c5, c0, 80), V(a2, 20, dc, 61), V(69, 4b, 77, 5a), V(16, 1a, 12, 1c), \
    V(0a, ba, 93, e2), V(e5, 2a, a0, c0), V(43, e0, 22, 3c), V(1d, 17, 1b, 12), \
    V(0b, 0d, 09, 0e), V(ad, c7, 8b, f2), V(b9, a8, b6, 2d), V(c8, a9, 1e, 14), \
    V(85, 19, f1, 57), V(4c, 07, 75, af), V(bb, dd, 99, ee), V(fd, 60, 7f, a3), \
    V(9f, 26, 01, f7), V(bc, f5, 72, 5c), V(c5, 3b, 66, 44), V(34, 7e, fb, 5b), \
    V(76, 29, 43, 8b), V(dc, c6, 23, cb), V(68, fc, ed, b6), V(63, f1, e4, b8), \
    V(ca, dc, 31, d7), V(10, 85, 63, 42), V(40, 22, 97, 13), V(20, 11, c6, 84), \
    V(7d, 24, 4a, 85), V(f8, 3d, bb, d2), V(11, 32, f9, ae), V(6d, a1, 29, c7), \
    V(4b, 2f, 9e, 1d), V(f3, 30, b2, dc), V(ec, 52, 86, 0d), V(d0, e3, c1, 77), \
    V(6c, 16, b3, 2b), V(99, b9, 70, a9), V(fa, 48, 94, 11), V(22, 64, e9, 47), \
    V(c4, 8c, fc, a8), V(1a, 3f, f0, a0), V(d8, 2c, 7d, 56), V(ef, 90, 33, 22), \
    V(c7, 4e, 49, 87), V(c1, d1, 38, d9), V(fe, a2, ca, 8c), V(36, 0b, d4, 98), \
    V(cf, 81, f5, a6), V(28, de, 7a, a5), V(26, 8e, b7, da), V(a4, bf, ad, 3f), \
    V(e4, 9d, 3a, 2c), V(0d, 92, 78, 50), V(9b, cc, 5f, 6a), V(62, 46, 7e, 54), \
    V(c2, 13, 8d, f6), V(e8, b8, d8, 90), V(5e, f7, 39, 2e), V(f5, af, c3, 82), \
    V(be, 80, 5d, 9f), V(7c, 93, d0, 69), V(a9, 2d, d5, 6f), V(b3, 12, 25, cf), \
    V(3b, 99, ac, c8), V(a7, 7d, 18, 10), V(6e, 63, 9c, e8), V(7b, bb, 3b, db), \
    V(09, 78, 26, cd), V(f4, 18, 59, 6e), V(01, b7, 9a, ec), V(a8, 9a, 4f, 83), \
    V(65, 6e, 95, e6), V(7e, e6, ff, aa), V(08, cf, bc, 21), V(e6, e8, 15, ef), \
    V(d9, 9b, e7, ba), V(ce, 36, 6f, 4a), V(d4, 09, 9f, ea), V(d6, 7c, b0, 29), \
    V(af, b2, a4, 31), V(31, 23, 3f, 2a), V(30, 94, a5, c6), V(c0, 66, a2, 35), \
    V(37, bc, 4e, 74), V(a6, ca, 82, fc), V(b0, d0, 90, e0), V(15, d8, a7, 33), \
    V(4a, 98, 04, f1), V(f7, da, ec, 41), V(0e, 50, cd, 7f), V(2f, f6, 91, 17), \
    V(8d, d6, 4d, 76), V(4d, b0, ef, 43), V(54, 4d, aa, cc), V(df, 04, 96, e4), \
    V(e3, b5, d1, 9e), V(1b, 88, 6a, 4c), V(b8, 1f, 2c, c1), V(7f, 51, 65, 46), \
    V(04, ea, 5e, 9d), V(5d, 35, 8c, 01), V(73, 74, 87, fa), V(2e, 41, 0b, fb), \
    V(5a, 1d, 67, b3), V(52, d2, db, 92), V(33, 56, 10, e9), V(13, 47, d6, 6d), \
    V(8c, 61, d7, 9a), V(7a, 0c, a1, 37), V(8e, 14, f8, 59), V(89, 3c, 13, eb), \
    V(ee, 27, a9, ce), V(35, c9, 61, b7), V(ed, e5, 1c, e1), V(3c, b1, 47, 7a), \
    V(59, df, d2, 9c), V(3f, 73, f2, 55), V(79, ce, 14, 18), V(bf, 37, c7, 73), \
    V(ea, cd, f7, 53), V(5b, aa, fd, 5f), V(14, 6f, 3d, df), V(86, db, 44, 78), \
    V(81, f3, af, ca), V(3e, c4, 68, b9), V(2c, 34, 24, 38), V(5f, 40, a3, c2), \
    V(72, c3, 1d, 16), V(0c, 25, e2, bc), V(8b, 49, 3c, 28), V(41, 95, 0d, ff), \
    V(71, 01, a8, 39), V(de, b3, 0c, 08), V(9c, e4, b4, d8), V(90, c1, 56, 64), \
    V(61, 84, cb, 7b), V(70, b6, 32, d5), V(74, 5c, 6c, 48), V(42, 57, b8, d0)

#define V(a, b, c, d) 0x##a##b##c##d
static const uint32_t RT0[256] = {RT};
#undef V

#define V(a, b, c, d) 0x##b##c##d##a
static const uint32_t RT1[256] = {RT};
#undef V

#define V(a, b, c, d) 0x##c##d##a##b
static const uint32_t RT2[256] = {RT};
#undef V

#define V(a, b, c, d) 0x##d##a##b##c
static const uint32_t RT3[256] = {RT};
#undef V

#undef RT

#define AES_RT0(idx) RT0[idx]
#define AES_RT1(idx) RT1[idx]
#define AES_RT2(idx) RT2[idx]
#define AES_RT3(idx) RT3[idx]

#define AES_FT0(idx) FT0[idx]
#define AES_FT1(idx) FT1[idx]
#define AES_FT2(idx) FT2[idx]
#define AES_FT3(idx) FT3[idx]

#define AES_FROUND(X0, X1, X2, X3, Y0, Y1, Y2, Y3)          \
    do {                                                    \
        (X0) = *RK++ ^ AES_FT0(BYTE_0(Y0)) ^                \
               AES_FT1(BYTE_1(Y1)) ^                        \
               AES_FT2(BYTE_2(Y2)) ^                        \
               AES_FT3(BYTE_3(Y3));                         \
                                                            \
        (X1) = *RK++ ^ AES_FT0(BYTE_0(Y1)) ^                \
               AES_FT1(BYTE_1(Y2)) ^                        \
               AES_FT2(BYTE_2(Y3)) ^                        \
               AES_FT3(BYTE_3(Y0));                         \
                                                            \
        (X2) = *RK++ ^ AES_FT0(BYTE_0(Y2)) ^                \
               AES_FT1(BYTE_1(Y3)) ^                        \
               AES_FT2(BYTE_2(Y0)) ^                        \
               AES_FT3(BYTE_3(Y1));                         \
                                                            \
        (X3) = *RK++ ^ AES_FT0(BYTE_0(Y3)) ^                \
               AES_FT1(BYTE_1(Y0)) ^                        \
               AES_FT2(BYTE_2(Y1)) ^                        \
               AES_FT3(BYTE_3(Y2));                         \
    } while (0)

#define AES_RROUND(X0, X1, X2, X3, Y0, Y1, Y2, Y3)          \
    do {                                                    \
        (X0) = *RK++ ^ AES_RT0(BYTE_0(Y0)) ^                \
               AES_RT1(BYTE_1(Y3)) ^                        \
               AES_RT2(BYTE_2(Y2)) ^                        \
               AES_RT3(BYTE_3(Y1));                         \
                                                            \
        (X1) = *RK++ ^ AES_RT0(BYTE_0(Y1)) ^                \
               AES_RT1(BYTE_1(Y0)) ^                        \
               AES_RT2(BYTE_2(Y3)) ^                        \
               AES_RT3(BYTE_3(Y2));                         \
                                                            \
        (X2) = *RK++ ^ AES_RT0(BYTE_0(Y2)) ^                \
               AES_RT1(BYTE_1(Y1)) ^                        \
               AES_RT2(BYTE_2(Y0)) ^                        \
               AES_RT3(BYTE_3(Y3));                         \
                                                            \
        (X3) = *RK++ ^ AES_RT0(BYTE_0(Y3)) ^                \
               AES_RT1(BYTE_1(Y2)) ^                        \
               AES_RT2(BYTE_2(Y1)) ^                        \
               AES_RT3(BYTE_3(Y0));                         \
    } while (0)


enum aes_mode_t {
    aes_encrypt,
    aes_decrypt,
};


static const uint8_t FSb[] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
    0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
    0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
    0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
    0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
    0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
    0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
    0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
    0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
    0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
    0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
    0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
    0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
    0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
    0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
    0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
    0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

static const uint8_t RSb[] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38,
    0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87,
    0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d,
    0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2,
    0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16,
    0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda,
    0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a,
    0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02,
    0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea,
    0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85,
    0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89,
    0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20,
    0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31,
    0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d,
    0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0,
    0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26,
    0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

static const uint32_t RCON[] = {
    0x00000001, 0x00000002, 0x00000004, 0x00000008,
    0x00000010, 0x00000020, 0x00000040, 0x00000080,
    0x0000001b, 0x00000036
};


static uint32_t reverse_u32(uint32_t value);
static uint32_t read_u32_le(const void *data, size_t offset);
static void write_u32_le(uint32_t value, void *data, size_t offset);
static void internal_encrypt(struct aes_ctx *ctx, const void *input, void *output);
static void internal_decrypt(struct aes_ctx *ctx, const void *input, void *output);
static int32_t crypt_ecb(struct aes_ctx *ctx, enum aes_mode_t mode, 
                        const void *input, void *output);
static int32_t crypt_cbc(struct aes_ctx *ctx, enum aes_mode_t mode, size_t length, 
                         void *iv, const void *input, void *output);
static int32_t setkey_enc(struct aes_ctx *ctx, const void *key, uint32_t key_bits);
static void xor(uint8_t *r, const uint8_t *a, const uint8_t *b, size_t n);


void aes_init(struct aes_ctx *ctx)
{
    ctx->nr = 0;
    ctx->rk_offset = 0;

    memset(ctx->buf, 0, sizeof(ctx->buf));
}

int32_t aes_enc_setkey(struct aes_ctx *ctx, const void *key, uint32_t key_len)
{
    return setkey_enc(ctx, key, key_len * 8);
}

int32_t aes_dec_setkey(struct aes_ctx *ctx, const void *key, uint32_t key_len)
{
    const uint32_t key_bits = key_len * 8;
    struct aes_ctx cty = {0};

    ctx->rk_offset = 0;
    uint32_t *RK = ctx->buf + ctx->rk_offset;

    /* Also checks key_bits */
    if ((setkey_enc(&cty, key, key_bits)) != 0) {
        return -1;
    }

    ctx->nr = cty.nr;
    uint32_t *SK = cty.buf + cty.rk_offset + cty.nr * 4;

    *RK++ = *SK++;
    *RK++ = *SK++;
    *RK++ = *SK++;
    *RK++ = *SK++;

    int32_t i;
    for (i = ctx->nr - 1, SK -= 8; i > 0; i--, SK -= 8) {
        for (int32_t j = 0; j < 4; j++, SK++) {
            *RK++ = AES_RT0(FSb[BYTE_0(*SK)]) ^
                    AES_RT1(FSb[BYTE_1(*SK)]) ^
                    AES_RT2(FSb[BYTE_2(*SK)]) ^
                    AES_RT3(FSb[BYTE_3(*SK)]);
        }
    }

    *RK++ = *SK++;
    *RK++ = *SK++;
    *RK++ = *SK++;
    *RK++ = *SK++;

    return 0;
}

int32_t aes_enc_cbc(struct aes_ctx *ctx, void *iv, const void *pt, 
                    size_t pt_len, void *ct)
{
    return crypt_cbc(ctx, aes_encrypt, pt_len, iv, pt, ct);
}

int32_t aes_dec_cbc(struct aes_ctx *ctx, void *iv, const void *ct, 
                    size_t ct_len, void *pt)
{
    return crypt_cbc(ctx, aes_decrypt, ct_len, iv, ct, pt);
}

static uint32_t reverse_u32(uint32_t value)
{
    return ((value & 0x000000ff) << 24) | 
           ((value & 0x0000ff00) <<  8) | 
           ((value & 0x00ff0000) >>  8) | 
           ((value & 0xff000000) >> 24);
}

static void write_u32_le(uint32_t value, void *data, size_t offset)
{
    uint32_t to_write;
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    to_write = value;
    (void) reverse_u32;
#else
    to_write = reverse_u32(value);
#endif
    memcpy((uint8_t *) data + offset, &to_write, sizeof(to_write));
}

static uint32_t read_u32_le(const void *data, size_t offset)
{
    uint32_t n;
    memcpy(&n, (uint8_t *) data + offset, sizeof(n));

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return n;
#else
    return reverse_u32(n);
#endif  
}

int32_t crypt_cbc(struct aes_ctx *ctx, enum aes_mode_t mode, size_t length, 
                  void *iv, const void *input, void *output)
{
    if (mode != aes_encrypt && mode != aes_decrypt) {
        return -1;
    }

    if (length % 16) {
        return -1;
    }

    const uint8_t *in = input;
    uint8_t *out = output;

    if (mode == aes_decrypt) {
        while (length > 0) {
            uint8_t temp[16];
            memcpy(temp, in, 16);
            crypt_ecb(ctx, mode, in, out);
            xor(out, out, iv, 16);
            memcpy(iv, temp, 16);

            in += 16;
            out += 16;
            length -= 16;
        }
    } else {
        while (length > 0) {
            xor(out, in, iv, 16);
            crypt_ecb(ctx, mode, out, out);
            memcpy(iv, out, 16);

            in += 16;
            out += 16;
            length -= 16;
        }
    }

    return 0;
}

static int32_t setkey_enc(struct aes_ctx *ctx, const void *key, uint32_t key_bits)
{
    switch (key_bits) {
        case 128: 
            ctx->nr = 10; 
            break;
        case 192: 
            ctx->nr = 12; 
            break;
        case 256: 
            ctx->nr = 14; 
            break;
        default: 
            return -1;
    }

    ctx->rk_offset = 0;
    uint32_t *RK = ctx->buf + ctx->rk_offset;

    for (uint32_t i = 0; i < (key_bits >> 5); i++) {
        RK[i] = read_u32_le(key, i << 2);
    }

    switch (ctx->nr) {
        case 10:
            for (uint32_t i = 0; i < 10; i++, RK += 4) {
                RK[4]  = RK[0] ^ RCON[i] ^
                         ((uint32_t) FSb[BYTE_1(RK[3])]) ^
                         ((uint32_t) FSb[BYTE_2(RK[3])] <<  8) ^
                         ((uint32_t) FSb[BYTE_3(RK[3])] << 16) ^
                         ((uint32_t) FSb[BYTE_0(RK[3])] << 24);

                RK[5]  = RK[1] ^ RK[4];
                RK[6]  = RK[2] ^ RK[5];
                RK[7]  = RK[3] ^ RK[6];
            }
            break;

        case 12:
            for (uint32_t i = 0; i < 8; i++, RK += 6) {
                RK[6]  = RK[0] ^ RCON[i] ^
                         ((uint32_t) FSb[BYTE_1(RK[5])]) ^
                         ((uint32_t) FSb[BYTE_2(RK[5])] <<  8) ^
                         ((uint32_t) FSb[BYTE_3(RK[5])] << 16) ^
                         ((uint32_t) FSb[BYTE_0(RK[5])] << 24);

                RK[7]  = RK[1] ^ RK[6];
                RK[8]  = RK[2] ^ RK[7];
                RK[9]  = RK[3] ^ RK[8];
                RK[10] = RK[4] ^ RK[9];
                RK[11] = RK[5] ^ RK[10];
            }
            break;

        case 14:
            for (uint32_t i = 0; i < 7; i++, RK += 8) {
                RK[8]  = RK[0] ^ RCON[i] ^
                         ((uint32_t) FSb[BYTE_1(RK[7])]) ^
                         ((uint32_t) FSb[BYTE_2(RK[7])] <<  8) ^
                         ((uint32_t) FSb[BYTE_3(RK[7])] << 16) ^
                         ((uint32_t) FSb[BYTE_0(RK[7])] << 24);

                RK[9]  = RK[1] ^ RK[8];
                RK[10] = RK[2] ^ RK[9];
                RK[11] = RK[3] ^ RK[10];

                RK[12] = RK[4] ^
                         ((uint32_t) FSb[BYTE_0(RK[11])]) ^
                         ((uint32_t) FSb[BYTE_1(RK[11])] <<  8) ^
                         ((uint32_t) FSb[BYTE_2(RK[11])] << 16) ^
                         ((uint32_t) FSb[BYTE_3(RK[11])] << 24);

                RK[13] = RK[5] ^ RK[12];
                RK[14] = RK[6] ^ RK[13];
                RK[15] = RK[7] ^ RK[14];
            }
            break;
    }

    return 0;
}

static int32_t crypt_ecb(struct aes_ctx *ctx, enum aes_mode_t mode, 
                         const void *input, void *output)
{
    if (mode != aes_encrypt && mode != aes_decrypt) {
        return -1;
    }

    if (mode == aes_encrypt) {
        internal_encrypt(ctx, input, output);
    } else {
        internal_decrypt(ctx, input, output);
    }

    return 0;
}

static void internal_encrypt(struct aes_ctx *ctx, const void *input, void *output)
{
    uint32_t *RK = ctx->buf + ctx->rk_offset;
    
    struct {
        uint32_t X[4];
        uint32_t Y[4];
    } t;

    t.X[0] = read_u32_le(input,  0); t.X[0] ^= *RK++;
    t.X[1] = read_u32_le(input,  4); t.X[1] ^= *RK++;
    t.X[2] = read_u32_le(input,  8); t.X[2] ^= *RK++;
    t.X[3] = read_u32_le(input, 12); t.X[3] ^= *RK++;

    int32_t i;
    for (i = (ctx->nr >> 1) - 1; i > 0; i--) {
        AES_FROUND(t.Y[0], t.Y[1], t.Y[2], t.Y[3], t.X[0], t.X[1], t.X[2], t.X[3]);
        AES_FROUND(t.X[0], t.X[1], t.X[2], t.X[3], t.Y[0], t.Y[1], t.Y[2], t.Y[3]);
    }

    AES_FROUND(t.Y[0], t.Y[1], t.Y[2], t.Y[3], t.X[0], t.X[1], t.X[2], t.X[3]);

    t.X[0] = *RK++ ^ \
             ((uint32_t) FSb[BYTE_0(t.Y[0])]) ^
             ((uint32_t) FSb[BYTE_1(t.Y[1])] <<  8) ^
             ((uint32_t) FSb[BYTE_2(t.Y[2])] << 16) ^
             ((uint32_t) FSb[BYTE_3(t.Y[3])] << 24);

    t.X[1] = *RK++ ^ \
             ((uint32_t) FSb[BYTE_0(t.Y[1])]) ^
             ((uint32_t) FSb[BYTE_1(t.Y[2])] <<  8) ^
             ((uint32_t) FSb[BYTE_2(t.Y[3])] << 16) ^
             ((uint32_t) FSb[BYTE_3(t.Y[0])] << 24);

    t.X[2] = *RK++ ^ \
             ((uint32_t) FSb[BYTE_0(t.Y[2])]) ^
             ((uint32_t) FSb[BYTE_1(t.Y[3])] <<  8) ^
             ((uint32_t) FSb[BYTE_2(t.Y[0])] << 16) ^
             ((uint32_t) FSb[BYTE_3(t.Y[1])] << 24);

    t.X[3] = *RK++ ^ \
             ((uint32_t) FSb[BYTE_0(t.Y[3])]) ^
             ((uint32_t) FSb[BYTE_1(t.Y[0])] <<  8) ^
             ((uint32_t) FSb[BYTE_2(t.Y[1])] << 16) ^
             ((uint32_t) FSb[BYTE_3(t.Y[2])] << 24);

    write_u32_le(t.X[0], output,  0);
    write_u32_le(t.X[1], output,  4);
    write_u32_le(t.X[2], output,  8);
    write_u32_le(t.X[3], output, 12);
}

static void internal_decrypt(struct aes_ctx *ctx, const void *input, void *output)
{
    uint32_t *RK = ctx->buf + ctx->rk_offset;

    struct {
        uint32_t X[4];
        uint32_t Y[4];
    } t;

    t.X[0] = read_u32_le(input,  0); t.X[0] ^= *RK++;
    t.X[1] = read_u32_le(input,  4); t.X[1] ^= *RK++;
    t.X[2] = read_u32_le(input,  8); t.X[2] ^= *RK++;
    t.X[3] = read_u32_le(input, 12); t.X[3] ^= *RK++;

    for (int32_t i = (ctx->nr >> 1) - 1; i > 0; i--) {
        AES_RROUND(t.Y[0], t.Y[1], t.Y[2], t.Y[3], t.X[0], t.X[1], t.X[2], t.X[3]);
        AES_RROUND(t.X[0], t.X[1], t.X[2], t.X[3], t.Y[0], t.Y[1], t.Y[2], t.Y[3]);
    }

    AES_RROUND(t.Y[0], t.Y[1], t.Y[2], t.Y[3], t.X[0], t.X[1], t.X[2], t.X[3]);

    t.X[0] = *RK++ ^ \
             ((uint32_t) RSb[BYTE_0(t.Y[0])]) ^
             ((uint32_t) RSb[BYTE_1(t.Y[3])] <<  8) ^
             ((uint32_t) RSb[BYTE_2(t.Y[2])] << 16) ^
             ((uint32_t) RSb[BYTE_3(t.Y[1])] << 24);

    t.X[1] = *RK++ ^ \
             ((uint32_t) RSb[BYTE_0(t.Y[1])]) ^
             ((uint32_t) RSb[BYTE_1(t.Y[0])] <<  8) ^
             ((uint32_t) RSb[BYTE_2(t.Y[3])] << 16) ^
             ((uint32_t) RSb[BYTE_3(t.Y[2])] << 24);

    t.X[2] = *RK++ ^ \
             ((uint32_t) RSb[BYTE_0(t.Y[2])]) ^
             ((uint32_t) RSb[BYTE_1(t.Y[1])] <<  8) ^
             ((uint32_t) RSb[BYTE_2(t.Y[0])] << 16) ^
             ((uint32_t) RSb[BYTE_3(t.Y[3])] << 24);

    t.X[3] = *RK++ ^ \
             ((uint32_t) RSb[BYTE_0(t.Y[3])]) ^
             ((uint32_t) RSb[BYTE_1(t.Y[2])] <<  8) ^
             ((uint32_t) RSb[BYTE_2(t.Y[1])] << 16) ^
             ((uint32_t) RSb[BYTE_3(t.Y[0])] << 24);

    write_u32_le(t.X[0], output,  0);
    write_u32_le(t.X[1], output,  4);
    write_u32_le(t.X[2], output,  8);
    write_u32_le(t.X[3], output, 12);
}

static void xor(uint8_t *r, const uint8_t *a, const uint8_t *b, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        r[i] = a[i] ^ b[i];
    }
}
