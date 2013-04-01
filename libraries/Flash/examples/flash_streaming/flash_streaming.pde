/*
  flash_streaming.pde - A program to demonstrate the Arduino Flash library
  Copyright (C) 2009 Mikal Hart
  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Flash.h>

/*
  This version of the Flash library example program demonstrates streaming of 
  inline flash-based strings (and other data) using the F("...") macro syntax 
  and the << operator.
*/

/* named strings */
FLASH_STRING(big_string, 
                  "Stately, plump Buck Mulligan came from the stairhead, bearing a bowl of\n"
                  "lather on which a mirror and a razor lay crossed. A yellow dressinggown,\n"
                  "ungirdled, was sustained gently behind him on the mild morning air. He\n"
                  "held the bowl aloft and intoned:\n"
                  "--_Introibo ad altare Dei_.\n"
                  "Halted, he peered down the dark winding stairs and called out coarsely:\n"
                  "--Come up, Kinch! Come up, you fearful jesuit!\n"
                  "Solemnly he came forward and mounted the round gunrest. He faced about\n"
                  "and blessed gravely thrice the tower, the surrounding land and the\n"
                  "awaking mountains. Then, catching sight of Stephen Dedalus, he bent\n"
                  "towards him and made rapid crosses in the air, gurgling in his throat\n"
                  "and shaking his head. Stephen Dedalus, displeased and sleepy, leaned\n"
                  "his arms on the top of the staircase and looked coldly at the shaking\n"
                  "gurgling face that blessed him, equine in its length, and at the light\n"
                  "untonsured hair, grained and hued like pale oak.\n");

/* named arrays */
FLASH_ARRAY(int, sine_table, 
                  0,6,12,18,25,31,37,43,49,56,62,68,74,80,86,92,97,103,109,115,120,126,131,136,142,
                  147,152,157,162,167,171,176,181,185,189,193,197,201,205,209,212,216,219,222,225,
                  228,231,234,236,238,241,243,244,246,248,249,251,252,253,254,254,255,255,255,256,
                  255,255,255,254,254,253,252,251,249,248,246,244,243,241,238,236,234,231,228,225,
                  222,219,216,212,209,205,201,197,193,189,185,181,176,171,167,162,157,152,147,142,
                  136,131,126,120,115,109,103,97,92,86,80,74,68,62,56,49,43,37,31,25,18,12,6,0,-6,
                  -12,-18,-25,-31,-37,-43,-49,-56,-62,-68,-74,-80,-86,-92,-97,-103,-109,-115,-120,
                  -126,-131,-136,-142,-147,-152,-157,-162,-167,-171,-176,-181,-185,-189,-193,-197,
                  -201,-205,-209,-212,-216,-219,-222,-225,-228,-231,-234,-236,-238,-241,-243,-244,
                  -246,-248,-249,-251,-252,-253,-254,-254,-255,-255,-255,-256,-255,-255,-255,-254,
                  -254,-253,-252,-251,-249,-248,-246,-244,-243,-241,-238,-236,-234,-231,-228,-225,
                  -222,-219,-216,-212,-209,-205,-201,-197,-193,-189,-185,-181,-176,-171,-167,-162,
                  -157,-152,-147,-142,-136,-131,-126,-120,-115,-109,-103,-97,-92,-86,-80,-74,-68,
                  -62,-56,-49,-43,-37,-31,-25,-18,-12,-6,-6,-12,-18,-25,-31,-37,-43,-49,-56,-62,
                  -68,-74,-80,-86,-92,-97,-103,-109,-115,-120,-126,-131,-136,-142,-147,-152,-157,
                  -162,-167,-171,-176,-181,-185,-189,-193,-197,-201,-205,-209,-212,-216,-219,-222,
                  -225,-228,-231,-234,-236,-238,-241,-243,-244,-246,-248,-249,-251,-252,-253,-254,
                  -254,-255,-255,-255,-256,-255,-255,-255,-254,-254,-253,-252,-251,-249,-248,-246,
                  -244,-243,-241,-238,-236,-234,-231,-228,-225,-222,-219,-216,-212,-209,-205,-201,
                  -197,-193,-189,-185,-181,-176,-171,-167,-162,-157,-152,-147,-142,-136,-131,-126,
                  -120,-115,-109,-103,-97,-92,-86,-80,-74,-68,-62,-56,-49,-43,-37,-31,-25,-18,-12,-6);

/* named tables */
FLASH_TABLE(int, font_table, 5 /* width of table */, 
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //   0 NUL
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //   1 SOH
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //   2 STX
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //   3 ETX
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //   4 EOT
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //   5 ENQ
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //   6 ACK
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //   7 BEL
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //   8 BS
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //   9 HT
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //  10 LF
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //  11 VT
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //  12 FF
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //  13 CR
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //  14 SO
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //  15 SI
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //  16 DLE
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //  17 DC1
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //  18 DC2
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //  19 DC3
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //  20 DC4
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //  21 NAK
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //  22 SYN
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //  23 ETB
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //  24 CAN
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //  25 EM
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //  26 SUB
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //  27 ESC
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //  28 FS
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //  29 GS
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //  30 RS
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //  31 US
                  {0x00, 0x00, 0x00, 0x00, 0x00}, //  32 SP
                  {0x00, 0x00, 0x00, 0x5E, 0x00}, //  33 !
                  {0x00, 0x06, 0x00, 0x06, 0x00}, //  34 "
                  {0x14, 0x3E, 0x14, 0x3E, 0x14}, //  35 #
                  {0x00, 0x24, 0x6A, 0x2B, 0x12}, //  36 $
                  {0x4C, 0x2C, 0x10, 0x68, 0x64}, //  37 %
                  {0x34, 0x4A, 0x52, 0x24, 0x10}, //  38 &
                  {0x00, 0x00, 0x06, 0x00, 0x00}, //  39 '
                  {0x00, 0x00, 0x3C, 0x42, 0x00}, //  40 (
                  {0x00, 0x00, 0x42, 0x3C, 0x00}, //  41 )
                  {0x00, 0x04, 0x0E, 0x04, 0x00}, //  42 *
                  {0x08, 0x08, 0x3E, 0x08, 0x08}, //  43 +
                  {0x00, 0x00, 0x40, 0x20, 0x00}, //  44 ,
                  {0x00, 0x08, 0x08, 0x08, 0x08}, //  45 -
                  {0x00, 0x00, 0x00, 0x20, 0x00}, //  46 .
                  {0x00, 0x60, 0x18, 0x06, 0x00}, //  47 /
                  {0x00, 0x3C, 0x42, 0x42, 0x3C}, //  48 0
                  {0x00, 0x00, 0x04, 0x7E, 0x00}, //  49 1
                  {0x00, 0x64, 0x52, 0x52, 0x4C}, //  50 2
                  {0x00, 0x42, 0x4A, 0x4A, 0x34}, //  51 3
                  {0x00, 0x18, 0x14, 0x7E, 0x10}, //  52 4
                  {0x00, 0x4E, 0x4A, 0x4A, 0x32}, //  53 5
                  {0x00, 0x3C, 0x4A, 0x4A, 0x32}, //  54 6
                  {0x00, 0x02, 0x72, 0x0A, 0x06}, //  55 7
                  {0x00, 0x34, 0x4A, 0x4A, 0x34}, //  56 8
                  {0x00, 0x4C, 0x52, 0x52, 0x3C}, //  57 9
                  {0x00, 0x00, 0x00, 0x24, 0x00}, //  58 :
                  {0x00, 0x00, 0x40, 0x24, 0x00}, //  59 ;
                  {0x00, 0x00, 0x08, 0x14, 0x22}, //  60 <
                  {0x00, 0x14, 0x14, 0x14, 0x14}, //  61 =
                  {0x00, 0x22, 0x14, 0x08, 0x00}, //  62 >
                  {0x00, 0x04, 0x02, 0x52, 0x0C}, //  63 ?
                  {0x00, 0x34, 0x54, 0x64, 0x38}, //  64 @
                  {0x00, 0x7C, 0x0A, 0x0A, 0x7C}, //  65 A
                  {0x00, 0x7E, 0x4A, 0x4A, 0x34}, //  66 B
                  {0x00, 0x3C, 0x42, 0x42, 0x24}, //  67 C
                  {0x00, 0x7E, 0x42, 0x42, 0x3C}, //  68 D
                  {0x00, 0x3E, 0x4A, 0x4A, 0x42}, //  69 E
                  {0x00, 0x7E, 0x0A, 0x0A, 0x02}, //  70 F
                  {0x00, 0x3C, 0x42, 0x52, 0x32}, //  71 G
                  {0x00, 0x7E, 0x08, 0x08, 0x7E}, //  72 H
                  {0x00, 0x00, 0x00, 0x7E, 0x00}, //  73 I
                  {0x00, 0x20, 0x40, 0x42, 0x3E}, //  74 J
                  {0x00, 0x7E, 0x10, 0x28, 0x46}, //  75 K
                  {0x00, 0x3E, 0x40, 0x40, 0x40}, //  76 L
                  {0x7E, 0x04, 0x08, 0x04, 0x7E}, //  77 M
                  {0x00, 0x7E, 0x04, 0x08, 0x7E}, //  78 N
                  {0x00, 0x3C, 0x42, 0x42, 0x3C}, //  79 O
                  {0x00, 0x7E, 0x0A, 0x0A, 0x04}, //  80 P
                  {0x00, 0x3C, 0x42, 0x22, 0x5C}, //  81 Q
                  {0x00, 0x7E, 0x12, 0x32, 0x4C}, //  82 R
                  {0x00, 0x44, 0x4A, 0x4A, 0x32}, //  83 S
                  {0x00, 0x02, 0x7E, 0x02, 0x00}, //  84 T
                  {0x00, 0x3E, 0x40, 0x40, 0x3E}, //  85 U
                  {0x00, 0x3E, 0x40, 0x20, 0x1E}, //  86 V
                  {0x1E, 0x60, 0x18, 0x60, 0x1E}, //  87 W
                  {0x00, 0x76, 0x08, 0x08, 0x76}, //  88 X
                  {0x00, 0x46, 0x48, 0x48, 0x3E}, //  89 Y
                  {0x00, 0x62, 0x52, 0x4A, 0x46}, //  90 Z
                  {0x00, 0x7E, 0x42, 0x00, 0x00}, //  91 [
                  {0x00, 0x06, 0x18, 0x60, 0x00}, //  92 backslash
                  {0x00, 0x42, 0x7E, 0x00, 0x00}, //  93 ]
                  {0x00, 0x04, 0x02, 0x04, 0x00}, //  94 ^
                  {0x00, 0x80, 0x80, 0x80, 0x80}, //  95 _
                  {0x00, 0x00, 0x01, 0x02, 0x00}, //  96 `
                  {0x00, 0x30, 0x48, 0x28, 0x78}, //  97 a
                  {0x00, 0x7E, 0x50, 0x48, 0x30}, //  98 b
                  {0x00, 0x30, 0x48, 0x50, 0x00}, //  99 c
                  {0x00, 0x30, 0x48, 0x50, 0x7E}, // 100 d
                  {0x00, 0x30, 0x68, 0x58, 0x00}, // 101 e
                  {0x00, 0x00, 0x7C, 0x0A, 0x00}, // 102 f
                  {0x00, 0x18, 0xA4, 0xA8, 0x7C}, // 103 g
                  {0x00, 0x7E, 0x10, 0x08, 0x70}, // 104 h
                  {0x00, 0x00, 0x74, 0x00, 0x00}, // 105 i
                  {0x00, 0x80, 0x88, 0x7A, 0x00}, // 106 j
                  {0x00, 0x7E, 0x10, 0x28, 0x40}, // 107 k
                  {0x00, 0x00, 0x3E, 0x40, 0x00}, // 108 l
                  {0x78, 0x08, 0x70, 0x08, 0x70}, // 109 m
                  {0x00, 0x78, 0x10, 0x08, 0x70}, // 110 n
                  {0x00, 0x30, 0x48, 0x48, 0x30}, // 111 o
                  {0x00, 0xFC, 0x28, 0x24, 0x18}, // 112 p
                  {0x00, 0x18, 0x24, 0x28, 0xFC}, // 113 q
                  {0x00, 0x78, 0x10, 0x08, 0x10}, // 114 r
                  {0x00, 0x4C, 0x54, 0x64, 0x00}, // 115 s
                  {0x00, 0x00, 0x3C, 0x48, 0x00}, // 116 t
                  {0x00, 0x38, 0x40, 0x20, 0x78}, // 117 u
                  {0x00, 0x38, 0x40, 0x20, 0x18}, // 118 v
                  {0x38, 0x40, 0x30, 0x40, 0x38}, // 119 w
                  {0x00, 0x68, 0x10, 0x68, 0x00}, // 120 x
                  {0x00, 0x1C, 0xA0, 0x90, 0x7C}, // 121 y
                  {0x00, 0x64, 0x54, 0x4C, 0x00}, // 122 z
                  {0x00, 0x08, 0x36, 0x41, 0x00}, // 123 {
                  {0x00, 0x00, 0x7F, 0x00, 0x00}, // 124 |
                  {0x00, 0x41, 0x36, 0x08, 0x00}, // 125 }
                  {0x00, 0x08, 0x04, 0x08, 0x04}, // 126 ~
                  {0x00, 0x00, 0x00, 0x00, 0x00});// 127 DEL

void setup()
{
  Serial.begin(115200);

  // This proves that all the big data is really in flash memory and not consuming RAM
  Serial << F("Despite there being an enormous amount of data in this program, the available RAM is still ") << availableMemory() << "\r\n";

  // Example 1: Inline flash strings
  Serial << F("\n****Example 1: This is an example of an inline flash (PROGMEM) string.\r\n");
  
  // Example 2: Named flash strings
  Serial << F("\n****Example 2: How to use named flash strings.\r\n");
  
  // 2.b Find the length of flash string (internally uses strlen_P)
  Serial << F("The (flash) size of the big string is ") << big_string.length() << "\r\n";
  Serial << F("But the RAM consumption is only ") << sizeof(big_string) << "\r\n";
  
  // 2.c Access individual characters of the flash string using array [] notation
  int comma_count = 0;
  for (int i=0; big_string[i]; ++i) // inspect each character in the string
    if (big_string[i] == ',')
      ++comma_count;
  Serial << F("There are ") << comma_count << F(" commas in the big string.\r\n");
  
  // 2.d Using the flash string copy() method, which copies some or all characters (internally uses strncpy_P)
  char extract[8] = {0};
  big_string.copy(extract, 7, 471); // Copy 7 characters from offset 471
  Serial << F("Extracted the word \"") << extract << F("\" from the big string using big_string.copy().\r\n");
  
  // 2.e Print the string
  Serial << F("The big string is, quote:\r\n") << big_string;

  // Example 3: Flash arrays
  Serial << F("\n****Example 3: How to use named flash arrays.\r\n");

  // 3.a Print out the array
  Serial << F("The sine table is ") << sine_table << "\r\n";

  // 3.b Determine the size of the array
  Serial << F("The (flash) size of the sine array is ") << sine_table.count() << F(" ints.\r\n");
  Serial << F("But the RAM consumption is only ") << sizeof(sine_table) << ".\r\n";
  
  // 3.c Access individual elements of the array using [] notation
  int maximum = -1000;
  for (int i=0; i<sine_table.count(); ++i)
    if (sine_table[i] > maximum)
      maximum = sine_table[i];
  Serial << F("The peak of the sine wave is ") << maximum << "\r\n";

  // Example 4: Two-dimensional tables
  Serial << F("\n****Example 4: How to use named 2D tables.\r\n");

  // 4.a Print out the font table
  Serial << F("The font table is \r\n") << font_table << "\r\n";

  // 4.b Determine the size of the array
  Serial << F("The (flash) size of the flash table is ") << font_table.rows() << F(" rows by ") <<
    font_table.cols() << F(" columns of ints.\r\n");
  Serial << F("But the RAM size consumption is only ") << sizeof(font_table) << "\r\n";
  
  // 4.c Access individual elements of the array using [] notation
  Serial << F("Font data for letter 'q':\r\n") << font_table['q'] << "\r\n";
  
  // Example 5: Arrays of strings
  // String arrays require PSTR notation
  // and must be declared inside a function body
  Serial << F("\n****Example 5: How to use arrays of strings.\r\n");

  FLASH_STRING_ARRAY(pips, 
    PSTR("Ace"), PSTR("Two"), PSTR("Three"), PSTR("Four"), PSTR("Five"), PSTR("Six"),
    PSTR("Seven"), PSTR("Eight"), PSTR("Nine"), PSTR("Ten"), PSTR("Jack"), PSTR("Queen"), PSTR("King"));
  
  FLASH_STRING_ARRAY(suits,
    PSTR("Hearts"), PSTR("Spades"), PSTR("Diamonds"), PSTR("Clubs"));

  Serial << F("\nPIPS\r\n");
  Serial << pips << "\r\n";
  for (int i=0; i<pips.count(); ++i)
  {
    Serial << i << " " << pips[i] << F("(string length=") << pips[i].length() << ")\r\n";
  }

  Serial << F("\nSUITS\r\n");
  Serial << suits << "\r\n";
  for (int i=0; i<suits.count(); ++i)
  {
    Serial << i << " " << suits[i] << F("(string length=") << suits[i].length() << ")\r\n";
  }
    
  Serial << F("\nChoosing 10 cards at random:\r\n");
  for (int i=0; i<10; ++i)
  {
    int pip = random(13);
    int suit = random(4);
    Serial << F("The ") << pips[pip] << F(" of ") << suits[suit] << "\r\n";
  }  
}

void loop()
{
}

int availableMemory() 
{
  int size = 1024;
  byte *buf;
  while ((buf = (byte *) malloc(--size)) == NULL);
  free(buf);
  return size;
}
