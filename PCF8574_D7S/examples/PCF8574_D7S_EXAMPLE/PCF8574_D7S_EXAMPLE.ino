/*
              PCF8574 Remote 8-Bit I/O Expander for I2C Bus                     ||                                                                     ||
MIT License                                                                     ||
                                                                                ||
Copyright (c) 2020 Fernando Marcos Marcos All right reserved.                   ||
                                                                                ||
Permission is hereby granted, free of charge, to any person obtaining a copy    ||
of this software and associated documentation files (the "Software"), to deal   ||
in the Software without restriction, including without limitation the rights    ||
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell       ||
copies of the Software, and to permit persons to whom the Software is           ||
furnished to do so, subject to the following conditions:                        ||
                                                                                ||
The above copyright notice and this permission notice shall be included in all  ||
copies or substantial portions of the Software.                                 ||
                                                                                ||
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      ||
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        ||
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     ||
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          ||
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   ||
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   ||
SOFTWARE.                                                                       ||
_________________________________________________________________________________
---------------------------------------------------------------------------------
*/

/*
WEBSITE WITH PRACTICES OF ARDUINO, FPGA, MATLAB AND
TOPICS ON DIGITAL ELECTRONICS, ANALOGIC, DIGITAL
SIGNAL PROCESSING, COMMUNICATIONS, ETC.
www.slideshare.net/FerMarcos2

PROFESSIONAL CURRICULUM
www.linkedin.com/in/FerMarcos2

YOUTUBE CHANNEL
www.youtube.com/channel/UCLGgLo5SKhDz3cae4YOQosg

________________________________________________________________________________________________________________
-------------------------------------------------CONECTIONS-----------------------------------------------------
----------------------------------------------------------------------------------------------------------------
    ||====BUS I2C ARDUINO====|| ||==CONFIGURATION ADDRESS PCF8574==||
    ||--MODELO--|--SDA-|-SCK-|| ||--A2-|-A1-|-A0---|--HEX(7Bites)--||
    ||=======================|| ||=================================||
    ||Uno-------|--A4--|--A5 || ||--0--|-0--|-0----|-----0x20------|| (I USED THE ADDRESS 0X20 IN THIS EXAMPLE)
    ||Nano------|--A4--|--A5 || ||--0--|-0--|-1----|-----0x21------||
    ||Mini Pro--|--A4--|--A5 || ||--0--|-1--|-0----|-----0x22------||
    ||Mega------|--20--|--21 || ||--0--|-1--|-1----|-----0x23------||
    ||=======================|| ||--1--|-0--|-0----|-----0x24------||
                                ||--1--|-0--|-1----|-----0x25------||
                                ||--1--|-1--|-0----|-----0x26------||
                                ||--1--|-1--|-1----|-----0x27------||
                                ||=================================||
 FOR MORE INFORMATION READ THE DATA SHEET OF EXPANDER
 DATA SHEET PCF8574 http://www.ti.com/lit/ds/symlink/pcf8574.pdf
 
                          (DISPLAY'S)
       D(n)      D(n-1)     D(n-2)             D1                                  PCF8574
      ------     ------     ------            ------  (SEGMENTS)       (P-PORT)    ---------  (ADDRESS)
     |  --- | ~ |  --- | ~ |  --- |          |  --- |------A----~~~~~~----P7------|         |-----A0---~~---GND          
     | |  | | ~ | |  | | ~ | |  | |          | |  | |------B----~~~~~~----P6------|         |-----A1---~~---GND
     | |  | | ~ | |  | | ~ | |  | |          | |  | |------C----~~~~~~----P5------|         |-----A2---~~---GND
     | |__| | ~ | |__| | ~ | |__| |   (8)    | |__| |------D----~~~~~~----P4------|         |
     | |  | | ~ | |  | | ~ | |  | | --~//~-- | |  | |------E----~~~~~~----P3------|         |
     | |  | | ~ | |  | | ~ | |  | |          | |  | |------F----~~~~~~----P2------|         |
     | |  | | ~ | |  | | ~ | |  | |          | |  | |------G----~~~~~~----P1------|         |    ___
     | --- o| ~ | --- o| ~ | --- o|          | --- o|------Dp---~~~~~~----P0------|         |----INT---~~---NOT CONNECTED
      ------     ------     ------            ------                               --------- 
         |          |          |                 |                                  |     |
       C(n)       C(n-2)     C(n-3)             C1(COMMON)                         SCL   SDA(BUS I2C PCF8574)
         |          |          |                 |                    Vcc---R10K----|     |----R10K----Vcc
         |          |          |                 |                                  |     |
       D(m-1)     D(m-2)    D(m-2)               D1(DIGITAL PORTS ARDUINO)         SCK   SDA(BUS I2C ARDUINO)  
         |          |          |                 |                                  |     |
     |-----------------------------------------------------------------------------------------|
     |                                                                                         |
     |                                                                                         |
     |                                    BOAR ARDUINO                                         |
     |                                                                                         |
     |                                                                                         |
      -----------------------------------------------------------------------------------------|
________________________________________________________________________________________________________________
-----------------------------------LIBRARY (DATA TYPES, METHODS AND FUNCTIONS-----------------------------------
----------------------------------------------------------------------------------------------------------------
DATA TYPES - CONSTRUCTOR - PCF8574_D7S I2CDISPLAY(address,common,ndigits,pin);
    * address (Variable type byte) = Addres of the PCF8574, it can be declared in two different ways,  Fox Example:
    *                        byte address = 0x20;      //Format Hex
    *                        byte address = B00100000; //Fotmat Binary
    * common (Variable type Boolean)= Select the type of Display to use, for example:
                             boolean common=false; (For Common Anode Display)
                             boolean common=true; (For Common Catode Display)
    * ndigits (Variable type int) = Select the amount of display to use. for example: 
                             int ndigits=8; (Eight Displays)
    * pin (Variable type char) = The digital ports to be used are placed in a char-type chain, displays must be ordered 
      according to ports,considering that ports do not necessarily have to be declared consecutively, for example:
                Example 1.   char pin[]={D1}; (Order 1)
                             char pin[]={2}; (One Digital Port)
                Example 2.   char pin[]={D4,D3,D2 D1}; (Order 2)
                             char pin[]={5,4,3,2};  (Four Digital ports)      
                Example 3.   char pin[]={D8,D7,D6,D5,D4,D3,D2 D1}; (Order 3)
                             char pin[]={9,8,7,6,5,4,3,2};  (Eight Digital ports)
                Example 4.   char pin[]={D8,D7,D6,D5,D4,D3,D2 D1}; (Order 4)
                             char pin[]={2,5,7,3,8,4,9,6};  (Eight Digital ports)
                Example 5.   char pin[]={D5,D4,D3,D2 D1}; (Order 5)
                             char pin[]={5,7,4,9,6};  (Five Digital ports)
        ***It's importan than the amount of digits of "ndigits" be equal to the number of declared ports of "pin", 
           otherwise there will be a shift of the number, equal to the difference between the number of digits 
           declared on "ndigits" and the number of ports declared on "pin", to the left if the number "ndigits" is 
           less than the number of ports declared, and to the right if the number is greater. For example
           
           Case 1, When int ndigits=7;           // Seven digits for seven display
                   char pin[]={9,8,7,6,5,4,3,2}; // Eight digital ports to eight display   
                |------------------|-------------|--------------------Displays---------------------|
                |---- Numbers -----|----Cases----|--D8----D7----D6----D5----D4----D3----D2----D1---|
                |------------------|-------------|-------------------------------------------------|
                | int n1=12345678--|-ndigits<pin-|--2-----3-----4-----5-----6-----7-----8-----OFF--|                   
                ------------------------------------------------------------------------------------
                *Displacement is a space to the left
           Case 2, When int ndigits=8;         // Eight digits for eight display
                   char pin[]={8,7,6,5,4,3,2}; // Seven digital ports to seven display   
                |------------------|-------------|--------------------Displays---------------------|
                |---- Numbers -----|----Cases----|--D8----D7----D6----D5----D4----D3----D2----D1---|
                |------------------|-------------|-------------------------------------------------|
                | int n1=12345678--|-ndigits>pin-|--OFF---1-----2-----3-----4-----5-----6-----7----|                   
                ------------------------------------------------------------------------------------
                *Displacement is a space to the right

METHODS AND FUNCTIONS - I2CDISPLAY.Multiplexer(number,pin,wait);
    * pin (Variable type int) = It's the same variable char type used on the Constructor PCF8574_D7S. The digital 
      ports will be connected to the common displays.
    * number (Variable type int, unsigned int, float, long, unsigned long) = This variable will save the number 
      displayed on the displays, considering the storage capacity of each type variable.
                  int (2 bytes = 16 bites): The number range is -32,768 up to 32,767.
                  unsigned int (2 bytes = 16 bites): The number range is 0 up to 65,535.
                  float (4 bytes = 32 bites): The number range is -3.4028235E+38 up to +3.4028235E+38.
                  long (4 bytes = 32 bites): The number range is -2,147,483,648 up to 2,147,483,647.
                  unsigned long (4 bytes = 32 bites): The number range is 0 up to ‭4,294,967,294‬.
                  double (8 byes = 64 bites): It can save decimal numbers.
          *** It is important that the number you want to display corresponds to the type of variable required to 
              store it, otherwise the number saved in the variable will be the number corresponding to its maximum 
              capacity.
    * wait (Variable type int)= This variable will save the time out between every digit displayed on the displays.
     
________________________________________________________________________________________________________________
--------------------------------------------   EXAMPLES---------------------------------------------------------
|-----------------------------------|-------------------DISPLAYS---------------------|--------------------------
|-- number -------------|--#DIGITS--|--D8----D7----D6----D5----D4----D3----D2----D1--|--------------------------
|-----------------------|-----------|------------------------------------------------|--------------------------
| int n1=1;-------------|-----1-----|--OFF---OFF---OFF---OFF---OFF---OFF---OFF---1---|---WORKS------------------                    
| int n2=1234-----------|-----4-----|--OFF---OFF---OFF---OFF---1-----2-----3-----4---|---WORKS------------------
| int n3=12345678-------|-----8-----|--1-----2-----3-----4-----5-----6-----7-----8---|---WORKS------------------
| float f1=1.0----------|-----1-----|--OFF---OFF---OFF---OFF---OFF---OFF---OFF---1---|---WORKS------------------
| int n4=01;------------|-----1-----|--OFF---OFF---OFF---OFF---OFF---OFF---OFF---1---|---WORKS- *n4 can be equal to 01 up to 07      
| int n5=07;------------|-----1-----|--OFF---OFF---OFF---OFF---OFF---OFF---OFF---7---|---WORKS- *n5 can be equal to 01 up to 07                                    
| float f2=1.00---------|-----1-----|--OFF---OFF---OFF---OFF---OFF---OFF---OFF---1---|---WORKS------------------
| float f3=1.-----------|-----1-----|--OFF---OFF---OFF---OFF---OFF---OFF---OFF---1---|---WORKS- *The point won't need to be displayed because there're no decimal places
| float f4=1.1----------|-----2-----|--OFF---OFF---OFF---OFF---OFF---OFF---1.----1---|---WORKS------------------
| float f5=1.10---------|-----2-----|--OFF---OFF---OFF---OFF---OFF---OFF---1.----1---|---WORKS- *Number Zero won't need to be shown
| float f6=1.01---------|-----3-----|--OFF---OFF---OFF---OFF---OFF---1.----0-----1---|---WORKS------------------
| float f7=1.1397-------|-----3-----|--OFF---OFF---OFF---OFF---OFF---1.----1-----3---|---WORKS- *Only two decimal places will be shown without exception
| float f8=1234567.80---|-----8-----|--1-----2-----3-----4-----5-----6-----7.----8---|---WORKS------------------
| float f9=12345.678----|-----7-----|--OFF---1-----2-----3-----4-----5.----6-----7---|---WORKS------------------
| float f10=0.13--------|-----3-----|--OFF---OFF---OFF---OFF---OFF---0.----1-----3---|---WORKS------------------
| float f10=0.03--------|-----3-----|--OFF---OFF---OFF---OFF---OFF---0.----0-----3---|---WORKS------------------
| float f10=0.1---------|-----2-----|--OFF---OFF---OFF---OFF---OFF---OFF---0.----1---|---WORKS------------------
| int n4=08;------------|-----1-----|--OFF---OFF---OFF---OFF---OFF---OFF---OFF---1---|---IT DOESN'T WORKS- *It also doesn't work for other numbers with zero on the left, except for the examples shown above                     
----------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------
________________________________________________________________________________________________________________*/

//PROYECT ON YOUTUBE https://youtu.be/ENlRaSL34h0

#include <PCF8574_D7S.h>                              //Library
byte address = B00100000;                             // Address of PCF8574
boolean common = false;                               //false for Common Anode & true for Common Catode
int ndigits = 6;                                      //Quantity of Digits (1 digit up to 8 digits)
int wait = 6;                                         //Timeout between digits shown
char pin[] = {13,12,11,10,9,8};                       //Ports Digital ORDER {D(n),D(n-1),D(n-2),---~~---,D1}
PCF8574_D7S I2CDISPLAY(address,common,ndigits,pin);   // 
void setup() {
}
void loop() {
  int i=0;
  int repeat=61;      //MULTIPLEXER REPLAYS
  float number=0.01;  //NUMBER 1                               //Number displayed on the displays
  for(i=0;i<repeat;i++){
    I2CDISPLAY.Multiplexer(number,pin,wait);        //(Number Send to the Display,Ports connected to common,Timeout between digits shown);
  }
  number=0.12;        //NUMBER 2                                       //Number displayed on the displays
  for(i=0;i<repeat;i++){
    I2CDISPLAY.Multiplexer(number,pin,wait);        //(Number Send to the Display,Ports connected to common,Timeout between digits shown);
  }
  number=1.23;        //NUMBER 3                                       //Number displayed on the displays
  for(i=0;i<repeat;i++){
  I2CDISPLAY.Multiplexer(number,pin,wait);          //(Number Send to the Display,Ports connected to common,Timeout between digits shown);
  }
  number=12.34;       //NUMBER 4                                     //Number displayed on the displays
  for(i=0;i<repeat;i++){
  I2CDISPLAY.Multiplexer(number,pin,wait);          //(Number Send to the Display,Ports connected to common,Timeout between digits shown);
  }
  number=123.45;      //NUMBER 5                                    //Number displayed on the displays
  for(i=0;i<repeat;i++){
  I2CDISPLAY.Multiplexer(number,pin,wait);          //(Number Send to the Display,Ports connected to common,Timeout between digits shown);
  }
  number=1234.56;     //NUMBER 6                                   //Number displayed on the displays
  for(i=0;i<repeat;i++){
  I2CDISPLAY.Multiplexer(number,pin,wait);          //(Number Send to the Display,Ports connected to common,Timeout between digits shown);
  }
  number=12345.6;     //NUMBER 7                                   //Number displayed on the displays
  for(i=0;i<repeat;i++){
    I2CDISPLAY.Multiplexer(number,pin,wait);        //(Number Send to the Display,Ports connected to common,Timeout between digits shown);
  }
  number=123456.;     //NUMBER 8                                   //Number displayed on the displays
  for(i=0;i<repeat;i++){
    I2CDISPLAY.Multiplexer(number,pin,wait);        //(Number Send to the Display,Ports connected to common,Timeout between digits shown);
  }
}
