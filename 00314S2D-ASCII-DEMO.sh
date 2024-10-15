#!/bin/bash

# 00314s2d-ASCII CH32V003 14 Segment 2 Digit Display test and demo script
# Built for 4 display modules in a chain, adjustable with settings
# ADBeta (c) 2024    15 Oct 2024    v0.1.1

DISP_PORT="/dev/ttyUSB0"
DISP_CHARS=8

DISP_STRING_SPLASH=" ADBeta "
DISP_STRING_SCROLL="        00314S2D-ASCII DISPLAY MODULE FONT TEST        \
!\"#\$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\`abcdefghijklmnopqrstuvwxyz{|}~        "

# Function simplifies the method of sending strings to the display
# not limited by size
function set_disp() {
	echo -e -n "\n$1" >> $DISP_PORT
}




### Init the display to be 115200 buad
stty -F $DISP_PORT 115200

### Splash screen for a couple seconds
set_disp "$DISP_STRING_SPLASH"
sleep 2

### Scroll through the first string
STRING_ITER=$(( ${#DISP_STRING_SCROLL} - $DISP_CHARS ))

for offset in $(seq 0 $STRING_ITER); do
	SUBSTR="${DISP_STRING_SCROLL:offset:DISP_CHARS}"
	set_disp "$SUBSTR"

	sleep 0.15
done
