# 00314S2D-ASCII 14 Segment 2 Digit LED Display Module    MkI

`00314S2D-ASCII` is a Daisychain-able 14 Segment, 2 Digit Display Module,
designed around the CH32V003 MCU.  
It receives `115200 Baud` UART Data, in standard ASCII Format, then displays
up to 2 Chars per Module. Any Additional chars will be transmitted down the 
chain of displays.

![PCB CAD](/Images/pcb_cad_drawing.jpg?raw=true)
![PCB 3D](/Images/pcb_cad_3d.jpg?raw=true)

## How to Use

## Building a Module
BOM
```
RN1        330R 4*0603 Resistor Array
RN2        330R 4*0603 Resistor Array
RN3        330R 4*0603 Resistor Array
RN4        330R 4*0603 Resistor Array
R1         10R 0603
R2         2K2 0603
Q1         AO3400 N-Ch MOSFET SOT-23
Q2         AO3400 N-Ch MOSFET SOT-23
Q3         AO3400 N-Ch MOSFET SOT-23
C1         100nF 0603 MLCC
C2         1uF 0603 MLCC
U1         CH32V003FxPx TSSOP-20
DISP1      Red/Green/Blue 14 Segment 2 Digit Common Cathode Display
```

## Programming
Programming is done via the SWIO TestPoint on the back of the PCB.
Once programmed, SWIO is disabled to be used as a normal GPIO.
To Reprogram the deivce, connect a programmer to the SWIO pin, and
power cycle the device - this will put the device into program mode.

1) Connect SWIO Pin
2) Apply Power to the Module
3) Run `minichlink -D` to allow NRST to be a GPIO Pin
4) Flash the binary to the IC

----
MIT License
Copyright (c) 2024 ADBeta
