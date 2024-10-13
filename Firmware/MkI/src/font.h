/******************************************************************************
* 14 Segment LED Display ASCII Font and control bytes enum
* ADBeta (c) 2024
******************************************************************************/

/// @brief An enum table of all ASCII Control Bytes
typedef enum {
	ASCII_CONTROL_NULL                             = 0x00,
	ASCII_CONTROL_START_HEADING                    = 0x01,
	ASCII_CONTROL_START_TEXT                       = 0x02,
	ASCII_CONTROL_END_TEXT                         = 0x03,
	ASCII_CONTROL_END_TRANSMISSION                 = 0x04,
	ASCII_CONTROL_ENQUIRY                          = 0x05,
	ASCII_CONTROL_ACK                              = 0x06,
	ASCII_CONTROL_BELL                             = 0x07,
	ASCII_CONTROL_BACKSPACE                        = 0x08,
	ASCII_CONTROL_HORIZ_TAB                        = 0x09,
	ASCII_CONTROL_LINE_FEED                        = 0x0A,
	ASCII_CONTROL_VERT_TAB                         = 0x0B,
	ASCII_CONTROL_FORM_FEED                        = 0x0C,
	ASCII_CONTROL_CARRIAGE_RETURN                  = 0x0D,
	ASCII_CONTROL_SHIFT_OUT                        = 0x0E,
	ASCII_CONTROL_SHIFT_IN                         = 0x0F,
	ASCII_CONTROL_DATA_LINK_ESCAPE                 = 0x10,
	ASCII_CONTROL_DEVICE_CONTROL_1                 = 0x11,
	ASCII_CONTROL_DEVICE_CONTROL_2                 = 0x12,
	ASCII_CONTROL_DEVICE_CONTROL_3                 = 0x13,
	ASCII_CONTROL_DEVICE_CONTROL_4                 = 0x14,
	ASCII_CONTROL_NACK                             = 0x15,
	ASCII_CONTROL_SYNC_IDLE                        = 0x16,
	ASCII_CONTROL_END_TRANSMISSION_BLOCK           = 0x17,
	ASCII_CONTROL_CANCEL                           = 0x18,
	ASCII_CONTROL_END_MEDIUM                       = 0x19,
	ASCII_CONTROL_SUBSTITUTE                       = 0x1A,
	ASCII_CONTROL_ESCAPE                           = 0x1B,
	ASCII_CONTROL_FILE_SEPARATOR                   = 0x1C,
	ASCII_CONTROL_GROUP_SEPARATOR                  = 0x1D,
	ASCII_CONTROL_RECORD_SEPARATOR                 = 0x1E,
	ASCII_CONTROL_UNIT_SEPARATOR                   = 0x1F,
} ascii_control;


/// @brief A font map for a 14 Segment LED Display.
/// Assuming bit[15] is Segment A, and bit[0] is Segment M.
/// ASCII Mapping, Ignoring 0x00 to 0x1F as control bytes (See above)
static const uint16_t ascii_font[] = {
	0x0000,    // Space
	0x4006,    // !
	0x0202,    // "
	0x12CE,    // #
	0x12ED,    // $
	0x2424,    // %
	0x09DB,    // &
	0x0200,    // '
	0x0039,    // (
	0x000F,    // )
	0x3FC0,    // *
	0x12C0,    // +
	0x2000,    // ,
	0x00C0,    // -
	0x4000,    // .
	0x2400,    // /
	0x243F,    // 0
	0x0406,    // 1
	0x00DB,    // 2
	0x008F,    // 3
	0x00E6,    // 4
	0x00ED,    // 5
	0x00FD,    // 6
	0x2401,    // 7
	0x00FF,    // 8
	0x00E7,    // 9
	0x1200,    // :
	0x2200,    // ;
	0x0C00,    // <
	0x00C8,    // =
	0x2100,    // >
	0x50A3,    // ?
	0x02BB,    // @
	0x00F7,    // A
	0x128F,    // B
	0x0039,    // C
	0x120F,    // D
	0x0079,    // E
	0x0071,    // F
	0x00BD,    // G
	0x00F6,    // H
	0x1209,    // I
	0x001E,    // J
	0x0C70,    // K
	0x0038,    // L
	0x0536,    // M
	0x0936,    // N
	0x003F,    // O
	0x00F3,    // P
	0x083F,    // Q
	0x08F3,    // R
	0x00ED,    // S
	0x1201,    // T
	0x003E,    // U
	0x2430,    // V
	0x2836,    // W
	0x2D00,    // X
	0x00EE,    // Y
	0x2409,    // Z
	0x0039,    // [
	0x0900,    // backslash
	0x000F,    // ]
	0x2800,    // ^
	0x0008,    // _
	0x0100,    // `
	0x1058,    // a
	0x0878,    // b
	0x00D8,    // c
	0x208E,    // d
	0x2058,    // e
	0x14C0,    // f
	0x048E,    // g
	0x1070,    // h
	0x1000,    // i
	0x2210,    // j
	0x1E00,    // k
	0x0030,    // l
	0x10D4,    // m
	0x1050,    // n
	0x00DC,    // o
	0x0170,    // p
	0x0486,    // q
	0x0050,    // r
	0x0888,    // s
	0x0078,    // t
	0x001C,    // u
	0x2010,    // v
	0x2814,    // w
	0x2D00,    // x
	0x028E,    // y
	0x2048,    // z
	0x2149,    // {
	0x1200,    // |
	0x0C89,    // }
	0x24C0,    // ~
};
