# 00314S2D-ASCII 14 Segment 2 Digit LED Display Module    MkI

`00314S2D-ASCII` is a Daisychain-able 14 Segment, 2 Digit Display Module,
designed around the CH32V003 MCU.  
It receives `115200 Baud` UART Data, in standard ASCII Format, then displays
up to 2 Chars per Module. Any Additional chars will be transmitted down the 
chain of displays.

![PCB CAD](/images/pcb_cad_drawing.jpg)
![PCB 3D](/images/pcb_cad_3d.jpg)

## How to Use

## Building a Module
BOM
```
RN1        150R 4*0603 Resistor Array
RN2        150R 4*0603 Resistor Array
RN3        150R 4*0603 Resistor Array
RN4        150R 4*0603 Resistor Array
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

Programming is done via the SWIO TestPoint on the back of the PCB.
Once programmed, the module can be re-programmed by TODO

## TODO

----
MIT License
Copyright (c) 2024 ADBeta
