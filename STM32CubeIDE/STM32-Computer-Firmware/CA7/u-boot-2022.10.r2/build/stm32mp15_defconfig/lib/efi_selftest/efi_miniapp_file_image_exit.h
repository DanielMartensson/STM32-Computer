/* SPDX-License-Identifier: GPL-2.0+ */
/*
 *  Non-zero 8 byte strings of a disk image
 *
 *  Generated with tools/file2include
 */

#define EFI_ST_DISK_IMG { 0x00000800, { \
	{0x00000000, "\x4d\x5a\x00\x00\x00\x00\x00\x00"}, /* MZ...... */ \
	{0x00000038, "\x00\x00\x00\x00\x40\x00\x00\x00"}, /* ....@... */ \
	{0x00000040, "\x50\x45\x00\x00\xc2\x01\x02\x00"}, /* PE...... */ \
	{0x00000050, "\x00\x00\x00\x00\x90\x00\x0e\x03"}, /* ........ */ \
	{0x00000058, "\x0b\x01\x02\x14\x00\x06\x00\x00"}, /* ........ */ \
	{0x00000068, "\x00\x02\x00\x00\x00\x02\x00\x00"}, /* ........ */ \
	{0x00000078, "\x00\x02\x00\x00\x00\x02\x00\x00"}, /* ........ */ \
	{0x00000090, "\x00\x08\x00\x00\x00\x02\x00\x00"}, /* ........ */ \
	{0x00000098, "\x00\x00\x00\x00\x0a\x00\x00\x00"}, /* ........ */ \
	{0x000000b0, "\x00\x00\x00\x00\x06\x00\x00\x00"}, /* ........ */ \
	{0x000000e8, "\x2e\x72\x65\x6c\x6f\x63\x00\x00"}, /* .reloc.. */ \
	{0x00000108, "\x00\x00\x00\x00\x40\x00\x10\x42"}, /* ....@..B */ \
	{0x00000110, "\x2e\x74\x65\x78\x74\x00\x00\x00"}, /* .text... */ \
	{0x00000118, "\x00\x06\x00\x00\x00\x02\x00\x00"}, /* ........ */ \
	{0x00000120, "\x00\x06\x00\x00\x00\x02\x00\x00"}, /* ........ */ \
	{0x00000130, "\x00\x00\x00\x00\x20\x00\x50\xe0"}, /* .... .P. */ \
	{0x00000138, "\x00\xf0\x20\xe3\x00\xf0\x20\xe3"}, /* .. ... . */ \
	{0x00000200, "\x07\x40\x2d\xe9\x24\x10\x8f\xe2"}, /* .@-.$... */ \
	{0x00000208, "\x00\x00\x91\xe5\x01\x10\x80\xe0"}, /* ........ */ \
	{0x00000210, "\x86\x0f\x4f\xe2\x39\x00\x00\xfa"}, /* ..O.9... */ \
	{0x00000218, "\x00\x00\x30\xe3\x01\x00\x00\x1a"}, /* ..0..... */ \
	{0x00000220, "\x03\x00\x9d\xe8\x1c\x00\x00\xfa"}, /* ........ */ \
	{0x00000228, "\x0c\xd0\x8d\xe2\x04\xf0\x9d\xe4"}, /* ........ */ \
	{0x00000230, "\xd0\x03\x00\x00\x02\x23\x30\xb5"}, /* .....#0. */ \
	{0x00000238, "\x85\xb0\xca\x6b\xcd\x6a\x01\x93"}, /* ...k.j.. */ \
	{0x00000240, "\x00\x23\x12\x49\x00\x93\xd2\xf8"}, /* .#.I.... */ \
	{0x00000248, "\x98\x40\x79\x44\x03\xaa\xa0\x47"}, /* .@yD...G */ \
	{0x00000250, "\x04\x46\x38\xb1\x0e\x49\x28\x46"}, /* .F8..I(F */ \
	{0x00000258, "\x6b\x68\x79\x44\x98\x47\x20\x46"}, /* khyD.G F */ \
	{0x00000260, "\x05\xb0\x30\xbd\x03\x99\x0b\x4a"}, /* ..0....J */ \
	{0x00000268, "\x0b\x6a\x7a\x44\x93\x42\x03\xd8"}, /* .jzD.B.. */ \
	{0x00000270, "\x89\x6a\x0b\x44\x93\x42\xf2\xd8"}, /* .j.D.B.. */ \
	{0x00000278, "\x07\x49\x28\x46\x6b\x68\x79\x44"}, /* .I(FkhyD */ \
	{0x00000280, "\x98\x47\x01\x4c\xeb\xe7\x00\xbf"}, /* .G.L.... */ \
	{0x00000288, "\x0e\x00\x00\x80\xf2\x03\x00\x00"}, /* ........ */ \
	{0x00000290, "\x5c\x01\x00\x00\xc7\xff\xff\xff"}, /* \....... */ \
	{0x00000298, "\x82\x01\x00\x00\xf0\xb5\x85\xb0"}, /* ........ */ \
	{0x000002a0, "\x6f\x46\x05\x46\x0c\x46\xce\x6a"}, /* oF.F.F.j */ \
	{0x000002a8, "\x12\x49\x10\x22\x79\x44\x38\x46"}, /* .I."yD8F */ \
	{0x000002b0, "\x00\xf0\x77\xf8\x10\x49\x30\x46"}, /* ..w..I0F */ \
	{0x000002b8, "\x79\x44\x73\x68\x98\x47\x21\x46"}, /* yDsh.G!F */ \
	{0x000002c0, "\x28\x46\xff\xf7\xb7\xff\x70\xb1"}, /* (F....p. */ \
	{0x000002c8, "\x0c\x49\x30\x46\x79\x44\x73\x68"}, /* .I0FyDsh */ \
	{0x000002d0, "\x98\x47\x06\x49\xe3\x6b\x10\x22"}, /* .G.I.k." */ \
	{0x000002d8, "\x9c\x6f\x28\x46\x3b\x46\xa0\x47"}, /* .o(F;F.G */ \
	{0x000002e0, "\x00\x20\x05\xb0\xf0\xbd\x02\x49"}, /* . .....I */ \
	{0x000002e8, "\xf4\xe7\x00\xbf\x0e\x00\x00\x80"}, /* ........ */ \
	{0x000002f0, "\x03\x00\x00\x80\x16\x02\x00\x00"}, /* ........ */ \
	{0x000002f8, "\x90\x01\x00\x00\xb8\x01\x00\x00"}, /* ........ */ \
	{0x00000300, "\x00\x23\x30\xb5\x1a\x46\x1c\x46"}, /* .#0..F.F */ \
	{0x00000308, "\x0d\x68\x1d\xb9\x83\xb9\x82\xb9"}, /* .h...... */ \
	{0x00000310, "\x00\x20\x30\xbd\x12\x2d\x07\xd0"}, /* . 0..-.. */ \
	{0x00000318, "\x13\x2d\x07\xd0\x11\x2d\x01\xd1"}, /* .-...-.. */ \
	{0x00000320, "\x4b\x68\x03\x44\x08\x31\xef\xe7"}, /* Kh.D.1.. */ \
	{0x00000328, "\x4c\x68\xfb\xe7\x4a\x68\xf9\xe7"}, /* Lh..Jh.. */ \
	{0x00000330, "\x5a\xb9\x07\x48\xed\xe7\x19\x79"}, /* Z..H...y */ \
	{0x00000338, "\xa4\x1a\x17\x29\x08\xbf\x1d\x68"}, /* ...)...h */ \
	{0x00000340, "\x13\x44\x02\xbf\x41\x59\x09\x18"}, /* .D..AY.. */ \
	{0x00000348, "\x41\x51\x00\x2c\xf3\xdc\xdf\xe7"}, /* AQ.,.... */ \
	{0x00000350, "\x01\x00\x00\x80\x10\xb5\x01\x39"}, /* .......9 */ \
	{0x00000358, "\x02\x44\x90\x42\x01\xd1\x00\x20"}, /* .D.B...  */ \
	{0x00000360, "\x05\xe0\x03\x78\x11\xf8\x01\x4f"}, /* ...x...O */ \
	{0x00000368, "\xa3\x42\x01\xd0\x18\x1b\x10\xbd"}, /* .B...... */ \
	{0x00000370, "\x01\x30\xf2\xe7\x88\x42\x10\xb5"}, /* .0...B.. */ \
	{0x00000378, "\x01\xeb\x02\x03\x08\xd8\x42\x1e"}, /* ......B. */ \
	{0x00000380, "\x99\x42\x00\xd1\x10\xbd\x11\xf8"}, /* .B...... */ \
	{0x00000388, "\x01\x4b\x02\xf8\x01\x4f\xf7\xe7"}, /* .K...O.. */ \
	{0x00000390, "\x01\x46\x02\x44\x8a\x42\xf5\xd0"}, /* .F.D.B.. */ \
	{0x00000398, "\x13\xf8\x01\x4d\x02\xf8\x01\x4d"}, /* ...M...M */ \
	{0x000003a0, "\xf8\xe7\xff\xf7\xe7\xbf\x03\x46"}, /* .......F */ \
	{0x000003a8, "\x02\x44\x93\x42\x00\xd1\x70\x47"}, /* .D.B..pG */ \
	{0x000003b0, "\x03\xf8\x01\x1b\xf9\xe7\x70\x47"}, /* ......pG */ \
	{0x000003b8, "\x70\x47\x43\x00\x6f\x00\x75\x00"}, /* pGC.o.u. */ \
	{0x000003c0, "\x6c\x00\x64\x00\x20\x00\x6e\x00"}, /* l.d. .n. */ \
	{0x000003c8, "\x6f\x00\x74\x00\x20\x00\x6f\x00"}, /* o.t. .o. */ \
	{0x000003d0, "\x70\x00\x65\x00\x6e\x00\x20\x00"}, /* p.e.n. . */ \
	{0x000003d8, "\x6c\x00\x6f\x00\x61\x00\x64\x00"}, /* l.o.a.d. */ \
	{0x000003e0, "\x65\x00\x64\x00\x20\x00\x69\x00"}, /* e.d. .i. */ \
	{0x000003e8, "\x6d\x00\x61\x00\x67\x00\x65\x00"}, /* m.a.g.e. */ \
	{0x000003f0, "\x20\x00\x70\x00\x72\x00\x6f\x00"}, /*  .p.r.o. */ \
	{0x000003f8, "\x74\x00\x6f\x00\x63\x00\x6f\x00"}, /* t.o.c.o. */ \
	{0x00000400, "\x6c\x00\x00\x00\x49\x00\x6e\x00"}, /* l...I.n. */ \
	{0x00000408, "\x63\x00\x6f\x00\x72\x00\x72\x00"}, /* c.o.r.r. */ \
	{0x00000410, "\x65\x00\x63\x00\x74\x00\x20\x00"}, /* e.c.t. . */ \
	{0x00000418, "\x69\x00\x6d\x00\x61\x00\x67\x00"}, /* i.m.a.g. */ \
	{0x00000420, "\x65\x00\x5f\x00\x62\x00\x61\x00"}, /* e._.b.a. */ \
	{0x00000428, "\x73\x00\x65\x00\x20\x00\x6f\x00"}, /* s.e. .o. */ \
	{0x00000430, "\x72\x00\x20\x00\x69\x00\x6d\x00"}, /* r. .i.m. */ \
	{0x00000438, "\x61\x00\x67\x00\x65\x00\x5f\x00"}, /* a.g.e._. */ \
	{0x00000440, "\x73\x00\x69\x00\x7a\x00\x65\x00"}, /* s.i.z.e. */ \
	{0x00000448, "\x0a\x00\x00\x00\x45\x00\x46\x00"}, /* ....E.F. */ \
	{0x00000450, "\x49\x00\x20\x00\x61\x00\x70\x00"}, /* I. .a.p. */ \
	{0x00000458, "\x70\x00\x6c\x00\x69\x00\x63\x00"}, /* p.l.i.c. */ \
	{0x00000460, "\x61\x00\x74\x00\x69\x00\x6f\x00"}, /* a.t.i.o. */ \
	{0x00000468, "\x6e\x00\x20\x00\x63\x00\x61\x00"}, /* n. .c.a. */ \
	{0x00000470, "\x6c\x00\x6c\x00\x69\x00\x6e\x00"}, /* l.l.i.n. */ \
	{0x00000478, "\x67\x00\x20\x00\x45\x00\x78\x00"}, /* g. .E.x. */ \
	{0x00000480, "\x69\x00\x74\x00\x0a\x00\x00\x00"}, /* i.t..... */ \
	{0x00000488, "\x4c\x00\x6f\x00\x61\x00\x64\x00"}, /* L.o.a.d. */ \
	{0x00000490, "\x65\x00\x64\x00\x20\x00\x69\x00"}, /* e.d. .i. */ \
	{0x00000498, "\x6d\x00\x61\x00\x67\x00\x65\x00"}, /* m.a.g.e. */ \
	{0x000004a0, "\x20\x00\x70\x00\x72\x00\x6f\x00"}, /*  .p.r.o. */ \
	{0x000004a8, "\x74\x00\x6f\x00\x63\x00\x6f\x00"}, /* t.o.c.o. */ \
	{0x000004b0, "\x6c\x00\x20\x00\x6d\x00\x69\x00"}, /* l. .m.i. */ \
	{0x000004b8, "\x73\x00\x73\x00\x69\x00\x6e\x00"}, /* s.s.i.n. */ \
	{0x000004c0, "\x67\x00\x0a\x00\x00\x00\x53\x00"}, /* g.....S. */ \
	{0x000004c8, "\x55\x00\x43\x00\x43\x00\x45\x00"}, /* U.C.C.E. */ \
	{0x000004d0, "\x53\x00\x53\x00\x00\x00\x00\x00"}, /* S.S..... */ \
	{0x00000600, "\x10\x00\x00\x00\x00\x00\x00\x00"}, /* ........ */ \
	{0x00000608, "\x1e\x00\x00\x00\x02\x00\x00\x00"}, /* ........ */ \
	{0x00000640, "\xa1\x31\x1b\x5b\x62\x95\xd2\x11"}, /* .1.[b... */ \
	{0x00000648, "\x8e\x3f\x00\xa0\xc9\x69\x72\x3b"}, /* .?...ir; */ \
	{0x00000650, "\x00\x06\x00\x00\x00\x00\x00\x00"}, /* ........ */ \
	{0, NULL} } }