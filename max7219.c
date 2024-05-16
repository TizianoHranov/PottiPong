#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "armbianio.h"
#include "max7219.h"
//
// MAX7219 LED Matrix controller library
// Copyright (c) 2018 BitBank Software, Inc.
// Written by Larry Bank
// bitbank@pobox.com
// Project started 3/10/2018
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// I purchased a 4-module unit of red 8x8 LED matrices. They're wired such that the leftmost module is the last
// in the chain and the bits are arranged such that the MSB is on the left and row 0 is the top row.
//
// This library contains a basic set of functions to initialize the modules, set the intensity, draw text in 2 font
// sizes and scroll a bitmap (which can contain text or any graphics you put there
//
// The modules are connected to the SPI clock and data lines as well as a digital output pin to control the latching
// of the data. The SPI CS line is not sufficient to properly control the loading of the data. Each module expects
// to receive 16-bits of data at a time with D15 sent first and D0 sent last. The data contains an instruction in
// D11-D8 and data for that instruction in D7-D0. In my case of 4 modules cascaded together, a data stream of
// 16 x 4 = 64 bits must be transmitted at a time to latch data to all modules simultaneously. The controllers
// randomly power up in test mode (all LEDs on at their brightest). For this reason, the init function disables
// test mode just in case.
//
// Each module shifts bits in and the next bit is shifted out. For example, in the case of my setup with 4 modules
  const uint8_t ucFont[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7e,0x81,0x95,0xb1,0xb1,0x95,0x81,0x7e,
  0x7e,0xff,0xeb,0xcf,0xcf,0xeb,0xff,0x7e,0x0e,0x1f,0x3f,0x7e,0x3f,0x1f,0x0e,0x00,
  0x08,0x1c,0x3e,0x7f,0x3e,0x1c,0x08,0x00,0x38,0x9a,0x9f,0xff,0x9f,0x9a,0x38,0x00,
  0x10,0xb8,0xfc,0xfe,0xfc,0xb8,0x10,0x00,0x00,0x00,0x18,0x3c,0x3c,0x18,0x00,0x00,
  0xff,0xff,0xe7,0xc3,0xc3,0xe7,0xff,0xff,0x00,0x3c,0x66,0x42,0x42,0x66,0x3c,0x00,
  0xff,0xc3,0x99,0xbd,0xbd,0x99,0xc3,0xff,0x70,0xf8,0x88,0x88,0xfd,0x7f,0x07,0x0f,
  0x00,0x4e,0x5f,0xf1,0xf1,0x5f,0x4e,0x00,0xc0,0xe0,0xff,0x7f,0x05,0x05,0x07,0x07,
  0xc0,0xff,0x7f,0x05,0x05,0x65,0x7f,0x3f,0x99,0x5a,0x3c,0xe7,0xe7,0x3c,0x5a,0x99,
  0x7f,0x3e,0x3e,0x1c,0x1c,0x08,0x08,0x00,0x08,0x08,0x1c,0x1c,0x3e,0x3e,0x7f,0x00,
  0x00,0x24,0x66,0xff,0xff,0x66,0x24,0x00,0x00,0x5f,0x5f,0x00,0x00,0x5f,0x5f,0x00,
  0x06,0x0f,0x09,0x7f,0x7f,0x01,0x7f,0x7f,0xc0,0x9a,0xbf,0xa5,0xbd,0xd9,0x43,0x02,
  0x00,0x70,0x70,0x70,0x70,0x70,0x70,0x00,0x80,0x94,0xb6,0xff,0xff,0xb6,0x94,0x80,
  0x00,0x04,0x06,0x7f,0x7f,0x06,0x04,0x00,0x00,0x10,0x30,0x7f,0x7f,0x30,0x10,0x00,
  0x08,0x08,0x08,0x2a,0x3e,0x1c,0x08,0x00,0x08,0x1c,0x3e,0x2a,0x08,0x08,0x08,0x00,
  0x3c,0x3c,0x20,0x20,0x20,0x20,0x20,0x00,0x08,0x1c,0x3e,0x08,0x08,0x3e,0x1c,0x08,
  0x30,0x38,0x3c,0x3e,0x3e,0x3c,0x38,0x30,0x06,0x0e,0x1e,0x3e,0x3e,0x1e,0x0e,0x06,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x5f,0x5f,0x06,0x00,0x00,
  0x00,0x07,0x07,0x00,0x07,0x07,0x00,0x00,0x14,0x7f,0x7f,0x14,0x7f,0x7f,0x14,0x00,
  0x24,0x2e,0x2a,0x6b,0x6b,0x3a,0x12,0x00,0x46,0x66,0x30,0x18,0x0c,0x66,0x62,0x00,
  0x30,0x7a,0x4f,0x5d,0x37,0x7a,0x48,0x00,0x00,0x04,0x07,0x03,0x00,0x00,0x00,0x00,
  0x00,0x1c,0x3e,0x63,0x41,0x00,0x00,0x00,0x00,0x41,0x63,0x3e,0x1c,0x00,0x00,0x00,
  0x08,0x2a,0x3e,0x1c,0x1c,0x3e,0x2a,0x08,0x00,0x08,0x08,0x3e,0x3e,0x08,0x08,0x00,
  0x00,0x00,0x80,0xe0,0x60,0x00,0x00,0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x00,
  0x00,0x00,0x00,0x60,0x60,0x00,0x00,0x00,0x60,0x30,0x18,0x0c,0x06,0x03,0x01,0x00,
  0x3e,0x7f,0x59,0x4d,0x47,0x7f,0x3e,0x00,0x40,0x42,0x7f,0x7f,0x40,0x40,0x00,0x00,
  0x62,0x73,0x59,0x49,0x6f,0x66,0x00,0x00,0x22,0x63,0x49,0x49,0x7f,0x36,0x00,0x00,
  0x18,0x1c,0x16,0x53,0x7f,0x7f,0x50,0x00,0x27,0x67,0x45,0x45,0x7d,0x39,0x00,0x00,
  0x3c,0x7e,0x4b,0x49,0x79,0x30,0x00,0x00,0x03,0x03,0x71,0x79,0x0f,0x07,0x00,0x00,
  0x36,0x7f,0x49,0x49,0x7f,0x36,0x00,0x00,0x06,0x4f,0x49,0x69,0x3f,0x1e,0x00,0x00,
  0x00,0x00,0x00,0x66,0x66,0x00,0x00,0x00,0x00,0x00,0x80,0xe6,0x66,0x00,0x00,0x00,
  0x08,0x1c,0x36,0x63,0x41,0x00,0x00,0x00,0x00,0x14,0x14,0x14,0x14,0x14,0x14,0x00,
  0x00,0x41,0x63,0x36,0x1c,0x08,0x00,0x00,0x00,0x02,0x03,0x59,0x5d,0x07,0x02,0x00,
  0x3e,0x7f,0x41,0x5d,0x5d,0x5f,0x0e,0x00,0x7c,0x7e,0x13,0x13,0x7e,0x7c,0x00,0x00,
  0x41,0x7f,0x7f,0x49,0x49,0x7f,0x36,0x00,0x1c,0x3e,0x63,0x41,0x41,0x63,0x22,0x00,
  0x41,0x7f,0x7f,0x41,0x63,0x3e,0x1c,0x00,0x41,0x7f,0x7f,0x49,0x5d,0x41,0x63,0x00,
  0x41,0x7f,0x7f,0x49,0x1d,0x01,0x03,0x00,0x1c,0x3e,0x63,0x41,0x51,0x33,0x72,0x00,
  0x7f,0x7f,0x08,0x08,0x7f,0x7f,0x00,0x00,0x00,0x41,0x7f,0x7f,0x41,0x00,0x00,0x00,
  0x30,0x70,0x40,0x41,0x7f,0x3f,0x01,0x00,0x41,0x7f,0x7f,0x08,0x1c,0x77,0x63,0x00,
  0x41,0x7f,0x7f,0x41,0x40,0x60,0x70,0x00,0x7f,0x7f,0x0e,0x1c,0x0e,0x7f,0x7f,0x00,
  0x7f,0x7f,0x06,0x0c,0x18,0x7f,0x7f,0x00,0x1c,0x3e,0x63,0x41,0x63,0x3e,0x1c,0x00,
  0x41,0x7f,0x7f,0x49,0x09,0x0f,0x06,0x00,0x1e,0x3f,0x21,0x31,0x61,0x7f,0x5e,0x00,
  0x41,0x7f,0x7f,0x09,0x19,0x7f,0x66,0x00,0x26,0x6f,0x4d,0x49,0x59,0x73,0x32,0x00,
  0x03,0x41,0x7f,0x7f,0x41,0x03,0x00,0x00,0x7f,0x7f,0x40,0x40,0x7f,0x7f,0x00,0x00,
  0x1f,0x3f,0x60,0x60,0x3f,0x1f,0x00,0x00,0x3f,0x7f,0x60,0x30,0x60,0x7f,0x3f,0x00,
  0x63,0x77,0x1c,0x08,0x1c,0x77,0x63,0x00,0x07,0x4f,0x78,0x78,0x4f,0x07,0x00,0x00,
  0x47,0x63,0x71,0x59,0x4d,0x67,0x73,0x00,0x00,0x7f,0x7f,0x41,0x41,0x00,0x00,0x00,
  0x01,0x03,0x06,0x0c,0x18,0x30,0x60,0x00,0x00,0x41,0x41,0x7f,0x7f,0x00,0x00,0x00,
  0x08,0x0c,0x06,0x03,0x06,0x0c,0x08,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x00,0x00,0x03,0x07,0x04,0x00,0x00,0x00,0x20,0x74,0x54,0x54,0x3c,0x78,0x40,0x00,
  0x41,0x7f,0x3f,0x48,0x48,0x78,0x30,0x00,0x38,0x7c,0x44,0x44,0x6c,0x28,0x00,0x00,
  0x30,0x78,0x48,0x49,0x3f,0x7f,0x40,0x00,0x38,0x7c,0x54,0x54,0x5c,0x18,0x00,0x00,
  0x48,0x7e,0x7f,0x49,0x03,0x06,0x00,0x00,0x98,0xbc,0xa4,0xa4,0xf8,0x7c,0x04,0x00,
  0x41,0x7f,0x7f,0x08,0x04,0x7c,0x78,0x00,0x00,0x44,0x7d,0x7d,0x40,0x00,0x00,0x00,
  0x60,0xe0,0x80,0x84,0xfd,0x7d,0x00,0x00,0x41,0x7f,0x7f,0x10,0x38,0x6c,0x44,0x00,
  0x00,0x41,0x7f,0x7f,0x40,0x00,0x00,0x00,0x7c,0x7c,0x18,0x78,0x1c,0x7c,0x78,0x00,
  0x7c,0x78,0x04,0x04,0x7c,0x78,0x00,0x00,0x38,0x7c,0x44,0x44,0x7c,0x38,0x00,0x00,
  0x84,0xfc,0xf8,0xa4,0x24,0x3c,0x18,0x00,0x18,0x3c,0x24,0xa4,0xf8,0xfc,0x84,0x00,
  0x44,0x7c,0x78,0x4c,0x04,0x0c,0x18,0x00,0x48,0x5c,0x54,0x74,0x64,0x24,0x00,0x00,
  0x04,0x04,0x3e,0x7f,0x44,0x24,0x00,0x00,0x3c,0x7c,0x40,0x40,0x3c,0x7c,0x40,0x00,
  0x1c,0x3c,0x60,0x60,0x3c,0x1c,0x00,0x00,0x3c,0x7c,0x60,0x30,0x60,0x7c,0x3c,0x00,
  0x44,0x6c,0x38,0x10,0x38,0x6c,0x44,0x00,0x9c,0xbc,0xa0,0xa0,0xfc,0x7c,0x00,0x00,
  0x4c,0x64,0x74,0x5c,0x4c,0x64,0x00,0x00,0x08,0x08,0x3e,0x77,0x41,0x41,0x00,0x00,
  0x00,0x00,0x00,0x77,0x77,0x00,0x00,0x00,0x41,0x41,0x77,0x3e,0x08,0x08,0x00,0x00,
  0x02,0x03,0x01,0x03,0x02,0x03,0x01,0x00,0x70,0x78,0x4c,0x46,0x4c,0x78,0x70,0x00};

// 5x7 font (in 6x8 cell)
const uint8_t ucSmallFont[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3e,0x45,0x51,0x45,0x3e,0x00,0x3e,0x6b,0x6f,
  0x6b,0x3e,0x00,0x1c,0x3e,0x7c,0x3e,0x1c,0x00,0x18,0x3c,0x7e,0x3c,0x18,0x00,0x30,
  0x36,0x7f,0x36,0x30,0x00,0x18,0x5c,0x7e,0x5c,0x18,0x00,0x00,0x18,0x18,0x00,0x00,
  0x00,0xff,0xe7,0xe7,0xff,0xff,0x00,0x3c,0x24,0x24,0x3c,0x00,0x00,0xc3,0xdb,0xdb,
  0xc3,0xff,0x00,0x30,0x48,0x4a,0x36,0x0e,0x00,0x06,0x29,0x79,0x29,0x06,0x00,0x60,
  0x70,0x3f,0x02,0x04,0x00,0x60,0x7e,0x0a,0x35,0x3f,0x00,0x2a,0x1c,0x36,0x1c,0x2a,
  0x00,0x00,0x7f,0x3e,0x1c,0x08,0x00,0x08,0x1c,0x3e,0x7f,0x00,0x00,0x14,0x36,0x7f,
  0x36,0x14,0x00,0x00,0x5f,0x00,0x5f,0x00,0x00,0x06,0x09,0x7f,0x01,0x7f,0x00,0x22,
  0x4d,0x55,0x59,0x22,0x00,0x60,0x60,0x60,0x60,0x00,0x00,0x14,0xb6,0xff,0xb6,0x14,
  0x00,0x04,0x06,0x7f,0x06,0x04,0x00,0x10,0x30,0x7f,0x30,0x10,0x00,0x08,0x08,0x3e,
  0x1c,0x08,0x00,0x08,0x1c,0x3e,0x08,0x08,0x00,0x78,0x40,0x40,0x40,0x40,0x00,0x08,
  0x3e,0x08,0x3e,0x08,0x00,0x30,0x3c,0x3f,0x3c,0x30,0x00,0x03,0x0f,0x3f,0x0f,0x03,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x5f,0x06,0x00,0x00,0x07,0x03,0x00,
  0x07,0x03,0x00,0x24,0x7e,0x24,0x7e,0x24,0x00,0x24,0x2b,0x6a,0x12,0x00,0x00,0x63,
  0x13,0x08,0x64,0x63,0x00,0x36,0x49,0x56,0x20,0x50,0x00,0x00,0x07,0x03,0x00,0x00,
  0x00,0x00,0x3e,0x41,0x00,0x00,0x00,0x00,0x41,0x3e,0x00,0x00,0x00,0x08,0x3e,0x1c,
  0x3e,0x08,0x00,0x08,0x08,0x3e,0x08,0x08,0x00,0x00,0xe0,0x60,0x00,0x00,0x00,0x08,
  0x08,0x08,0x08,0x08,0x00,0x00,0x60,0x60,0x00,0x00,0x00,0x20,0x10,0x08,0x04,0x02,
  0x00,0x3e,0x51,0x49,0x45,0x3e,0x00,0x00,0x42,0x7f,0x40,0x00,0x00,0x62,0x51,0x49,
  0x49,0x46,0x00,0x22,0x49,0x49,0x49,0x36,0x00,0x18,0x14,0x12,0x7f,0x10,0x00,0x2f,
  0x49,0x49,0x49,0x31,0x00,0x3c,0x4a,0x49,0x49,0x30,0x00,0x01,0x71,0x09,0x05,0x03,
  0x00,0x36,0x49,0x49,0x49,0x36,0x00,0x06,0x49,0x49,0x29,0x1e,0x00,0x00,0x6c,0x6c,
  0x00,0x00,0x00,0x00,0xec,0x6c,0x00,0x00,0x00,0x08,0x14,0x22,0x41,0x00,0x00,0x24,
  0x24,0x24,0x24,0x24,0x00,0x00,0x41,0x22,0x14,0x08,0x00,0x02,0x01,0x59,0x09,0x06,
  0x00,0x3e,0x41,0x5d,0x55,0x1e,0x00,0x7e,0x11,0x11,0x11,0x7e,0x00,0x7f,0x49,0x49,
  0x49,0x36,0x00,0x3e,0x41,0x41,0x41,0x22,0x00,0x7f,0x41,0x41,0x41,0x3e,0x00,0x7f,
  0x49,0x49,0x49,0x41,0x00,0x7f,0x09,0x09,0x09,0x01,0x00,0x3e,0x41,0x49,0x49,0x7a,
  0x00,0x7f,0x08,0x08,0x08,0x7f,0x00,0x00,0x41,0x7f,0x41,0x00,0x00,0x30,0x40,0x40,
  0x40,0x3f,0x00,0x7f,0x08,0x14,0x22,0x41,0x00,0x7f,0x40,0x40,0x40,0x40,0x00,0x7f,
  0x02,0x04,0x02,0x7f,0x00,0x7f,0x02,0x04,0x08,0x7f,0x00,0x3e,0x41,0x41,0x41,0x3e,
  0x00,0x7f,0x09,0x09,0x09,0x06,0x00,0x3e,0x41,0x51,0x21,0x5e,0x00,0x7f,0x09,0x09,
  0x19,0x66,0x00,0x26,0x49,0x49,0x49,0x32,0x00,0x01,0x01,0x7f,0x01,0x01,0x00,0x3f,
  0x40,0x40,0x40,0x3f,0x00,0x1f,0x20,0x40,0x20,0x1f,0x00,0x3f,0x40,0x3c,0x40,0x3f,
  0x00,0x63,0x14,0x08,0x14,0x63,0x00,0x07,0x08,0x70,0x08,0x07,0x00,0x71,0x49,0x45,
  0x43,0x00,0x00,0x00,0x7f,0x41,0x41,0x00,0x00,0x02,0x04,0x08,0x10,0x20,0x00,0x00,
  0x41,0x41,0x7f,0x00,0x00,0x04,0x02,0x01,0x02,0x04,0x00,0x80,0x80,0x80,0x80,0x80,
  0x00,0x00,0x03,0x07,0x00,0x00,0x00,0x20,0x54,0x54,0x54,0x78,0x00,0x7f,0x44,0x44,
  0x44,0x38,0x00,0x38,0x44,0x44,0x44,0x28,0x00,0x38,0x44,0x44,0x44,0x7f,0x00,0x38,
  0x54,0x54,0x54,0x08,0x00,0x08,0x7e,0x09,0x09,0x00,0x00,0x18,0xa4,0xa4,0xa4,0x7c,
  0x00,0x7f,0x04,0x04,0x78,0x00,0x00,0x00,0x00,0x7d,0x40,0x00,0x00,0x40,0x80,0x84,
  0x7d,0x00,0x00,0x7f,0x10,0x28,0x44,0x00,0x00,0x00,0x00,0x7f,0x40,0x00,0x00,0x7c,
  0x04,0x18,0x04,0x78,0x00,0x7c,0x04,0x04,0x78,0x00,0x00,0x38,0x44,0x44,0x44,0x38,
  0x00,0xfc,0x44,0x44,0x44,0x38,0x00,0x38,0x44,0x44,0x44,0xfc,0x00,0x44,0x78,0x44,
  0x04,0x08,0x00,0x08,0x54,0x54,0x54,0x20,0x00,0x04,0x3e,0x44,0x24,0x00,0x00,0x3c,
  0x40,0x20,0x7c,0x00,0x00,0x1c,0x20,0x40,0x20,0x1c,0x00,0x3c,0x60,0x30,0x60,0x3c,
  0x00,0x6c,0x10,0x10,0x6c,0x00,0x00,0x9c,0xa0,0x60,0x3c,0x00,0x00,0x64,0x54,0x54,
  0x4c,0x00,0x00,0x08,0x3e,0x41,0x41,0x00,0x00,0x00,0x00,0x77,0x00,0x00,0x00,0x00,
  0x41,0x41,0x3e,0x08,0x00,0x02,0x01,0x02,0x01,0x00,0x00,0x3c,0x26,0x23,0x26,0x3c};

static uint8_t iNumControllers, iCSPin;
static int file_spi = -1;

//
// Transmit a sequence of N x 16 bits to the cascaded controllers
//
// Send an atomic sequence of uint8_ts for loading all chained controllers (16 bits per controller)
// Each controller acts like a 16-bit shift register and passes on the bits to the next controller. The
// last 16-bits to sit in the controller will be latched when the CS line goes high.
// uint8_t 0 -> D15-D8 (XXXX A3 A2 A1 A0), 4-bits unused and 4-bit register address
// uint8_t 1 -> D7-D0 (8-bit data)
//
void maxSendSequence(uint8_t *pSequence, uint8_t len)
{
  // The CS line stays low throughout a "transaction". Send all of the control uint8_ts for all of the chained
  // controllers in a single transaction. When the CS line rises, the data will be latched
   AIOWriteGPIO(iCSPin, 0);
   AIOWriteSPI(file_spi, pSequence, len);
   AIOWriteGPIO(iCSPin, 1);
   
} /* maxSendSequence() */

//
// Power on or off the LED controllers
//
void maxPowerUp(uint8_t bPowerUp)
{
uint8_t i;
uint8_t *d, bTemp[32]; // up to 16 controllers
  d = bTemp;
  for (i=0; i<iNumControllers; i++)
  {
     *d++ = 0x0C; // power up/down
     *d++ = bPowerUp;
  }
  maxSendSequence(bTemp, iNumControllers * 2); // send the power up/down instruction
} /* maxPowerUp() */
//
// Set the intensity (duty cycle of PWM signal) for the LED segments
// valid values are 0 (dimmest) to 15 (brightest)
//
void maxSetIntensity(uint8_t bIntensity)
{
uint8_t *d, bTemp[32];
uint8_t i;

  d = bTemp;
  for (i=0; i<iNumControllers; i++)
  {
      *d++ = 0x0A; // set intensity
      *d++ = bIntensity;
  } // for i
  maxSendSequence(bTemp, iNumControllers * 2);
} /* maxSetIntensity() */
//
// Set the segment decode mode (BCD or none)
//
void maxSetSegmentMode(uint8_t bMode)
{
uint8_t i, *d, bTemp[32];

  d = bTemp;
  for (i=0; i<iNumControllers; i++)
  {
      *d++ = 0x09; // decode mode
      *d++ = (bMode) ? 0xff : 0x00;
  } // for i
  maxSendSequence(bTemp, iNumControllers * 2); // send the scan limit instructions to all controllers
} /* maxSetSegmentMode() */

//
// Send image data to the array of controllers
// The image data is transmitted as N by 8 lines tall (N is the number of MAX7219 controllers)
// The pitch (uint8_ts per line) can be any value
//
void maxSendImage(uint8_t *pImage, int iPitch)
{
uint8_t i, j;
uint8_t *s, *d, bTemp[32];

   for (j=0; j<8; j++) // 8 rows to transmit
   {
      s = &pImage[iPitch * j];
      d = bTemp;
      for (i=0; i<iNumControllers; i++)
      {
         *d++ = (j+1); // row number is the "instruction"
         *d++ = *s++; // image data
      } // for each controller
      maxSendSequence(bTemp, iNumControllers * 2);
   } // for each row of image
} /* maxSendImage() */

//
// Enable (1) or disable (0) test mode
// This mode lights up every LED at max brightness
// It can sometimes power up in test mode
//
void maxSetTestMode(uint8_t bOn)
{
uint8_t i, *d, bTemp[32];

  d = bTemp;
  for (i=0; i<iNumControllers; i++)
  {
      *d++ = 0x0F; // test mode
      *d++ = bOn;
  } // for i
  maxSendSequence(bTemp, iNumControllers * 2); // send the scan limit instructions to all controllers
} /* maxSetTestMode() */
//
// Number of "digits/rows" to control
// valid values are 1-8 active digits/rows
//
void maxSetLimit(uint8_t bLimit)
{
uint8_t i, *d, bTemp[32];

  d = bTemp;
  for (i=0; i<iNumControllers; i++)
  {
      *d++ = 0x0B; // set scan limit
      *d++ = (bLimit - 1);
  } // for i
  maxSendSequence(bTemp, iNumControllers * 2); // send the scan limit instructions to all controllers
} /* maxSetLimit() */

//
// Send an ASCII string of numbers/spaces/decimal points
// to a 7-segment display
//
void maxSegmentString(char *pString)
{
unsigned char ucTemp[4];
int iDigit;

	memset(ucTemp, 0, sizeof(ucTemp));
	iDigit = 0;
	while (*pString && iDigit < 8)
	{
		ucTemp[0] = 8 - (iDigit & 7); // cmd byte to write
		if (pString[0] >= '0' && pString[0] <= '9')
		{
			ucTemp[1] = *pString++; // store digit
			if (pString[0] == '.')
			{
				ucTemp[1] |= 0x80; // turn on decimal point
				pString++;
			}
		}
		else
		{
			ucTemp[1] = 0xf; // space = all segments off
			pString++;
		}
		iDigit++;
		maxSendSequence(ucTemp, 2); // need to latch each byte pair
	}
	while (iDigit < 8) // blank out remaining digits
	{
		ucTemp[0] = 8 - (iDigit & 7);
		ucTemp[1] = 0xf; // all segments off
		iDigit++;
		maxSendSequence(ucTemp, 2);
	}
} /* maxSegmentString() */
//
// Draw a string of characters into the image buffer
// Normal characters are 8x8 and drawn on uint8_t boundaries
// Small characters are 6x8 and drawn on bit boundaries
//
void maxDrawString(char *pString, uint8_t *pImage, uint8_t iPitch, uint8_t bSmall)
{
uint8_t b, bMask, i, j, *d, bCol;
const uint8_t *pFont;
const uint8_t *s;
int iWidth;

   d = pImage;
   bCol = 0;
   if (bSmall)
   {
      pFont = ucSmallFont;
      iWidth = 6;
   }
   else
   {
      pFont = ucFont;
      iWidth = 8;
   }
   while (*pString)
   {
      b = *pString++;
      s = &pFont[(int)b * iWidth]; // 6 or 8 uint8_ts per character in ASCII order
      for (i=0; i<iWidth; i++) // column
      {
         bMask = (0x80 >> (bCol & 7));
         b = *s++; // current font uint8_t
         for (j=0; j<8; j++) // bit number of source becomes destination row
         {
            if (b & 1) // start from LSB
               d[j*iPitch] |= bMask;
            else
               d[j*iPitch] &= ~bMask;
            b >>= 1; // shift down font uint8_t
         } // for j
         bCol++;
         if ((bCol & 7) == 0) // next uint8_t
            d++;
      } // for i
   } // while string
} /* maxDrawString() */

//
// Scroll a bitmap N bits left (positive) or right (negative)
// Valid scroll values are +1 to +7 and -1 to -7
// A bitmap is assumed to be iPitch uint8_ts wide by 8 rows tall
// Bits which scroll off one end are added back to the other end
//
void maxScrollBitmap(uint8_t *pBitmap, int iPitch, int iScroll)
{
uint8_t b, bEdge, *s;
int col, row;

  if (iScroll > 0) // scroll left
  {
     for (row=0; row<8; row++)
     {
        s = &pBitmap[row * iPitch];
        bEdge = s[0] >> (8-iScroll);
        for (col=0; col<iPitch; col++)
        {
           b = s[col] << iScroll;
           b |= (col == iPitch-1) ? bEdge : (s[col+1] >> (8-iScroll));
           s[col] = b;
        } // for col
     } // for row
  }
  else // scroll right
  {
     iScroll = 0 - iScroll; // make it a positive number
     for (row=0; row<8; row++)
     {
        s = &pBitmap[row * iPitch];
        bEdge = s[iPitch-1] << (8-iScroll);
        for (col=iPitch-1; col>=0; col--)
        {
           b = s[col] >> iScroll;
           b |= (col == 0) ? bEdge : (s[col-1] << (8-iScroll));
           s[col] = b;
        } // for col
     } // for row    
  }
} /* maxScrollBitmap() */
//
// Initialize the controllers
//
int maxInit(uint8_t iNum, uint8_t bDecodeMode, uint8_t iChannel, uint8_t iSelect)
{

  if (!AIOInit())
  {
     fprintf(stderr, "Error initializing ArmbianIO library\n");
     return -1;
  }
  iNumControllers = iNum;
  iCSPin = iSelect; // header pin number used for select line
  file_spi = AIOOpenSPI(iChannel, 2000000); // 2Mhz is a reasonable speed
  if (file_spi == -1)
  {
     fprintf(stderr, "Failed to open the SPI bus\n");
     file_spi = -1;
     return -1;
  }
  AIOAddGPIO(iCSPin, GPIO_OUT);
  AIOWriteGPIO(iCSPin, 1);

  maxPowerUp(1); // turn on the LED controllers
  maxSetLimit(8); // tell it to display 8 rows of 8 pixels
  maxSetIntensity(0); // set the minimum intensity to start (1/32 duty cycle)
  maxSetSegmentMode(bDecodeMode); // sets BCD (7-segment), or none (pixels
  maxSetTestMode(0); // disable test mode (it can accidentally get set at power up) 
  return 0;
} /* maxInit() */

void maxShutdown(void)
{
   maxPowerUp(0); // turn off the LED controllers
   AIOCloseSPI(file_spi);
   file_spi = -1;
   AIOShutdown();
} /* maxShutdown() */
