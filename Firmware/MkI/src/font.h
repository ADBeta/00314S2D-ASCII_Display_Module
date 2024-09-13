/******************************************************************************
*
*
*
******************************************************************************/

/// @brief An enum table of all ASCII Control Bytes



/// @brief A font map for a 14 Segment LED Display.
/// Assuming bit[15] is Segment A, and bit[0] is Segment M.
/// ASCII Mapping, Ignoring 0x00 to 0x1F as control bytes (See above)
const uint16_t ascii_font[] = {
	0x0000,    // Space
	// !
	// "
	// #
	// $
	// %
	// &
	// '
	// (
	// )
	// *
	// +
	// ,
	// -
	// .
	// /
	// 0
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// :
	// ;
	// <
	// =
	// >
	// ?
	// @
	// A
	// B
	// C
	// D
	// E
	// F
	// G
	// H
	// I
	// J
	// K
	// L
	// M
	// N
	// O
	// P
	// Q
	// R
	// S
	// T
	// U
	// V
	// W
	// X
	// Y
	// Z
	// [
	// backslash
	// ]
	// ^
	// _
	// `
	// a
	// b
	// c
	// d
	// e
	// f
	// g
	// h
	// i
	// j
	// k
	// l
	// m
	// n
	// o
	// p
	// q
	// r
	// s
	// t
	// u
	// v
	// w
	// x
	// y
	// z
	// {
	// |
	// }
	// ~
};
