/**
 *
 * CC-Debugger Protocol Library Simulator Utilities
 * Copyright (c) 2014-2016 Ioannis Charalampidis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HARDWARELAYER_H
#define HARDWARELAYER_H

#include <emulated/ArduinoTypes.h>

/**
 * Operations
 */

const byte HWOP_CONFIGURATION  = 0x00;
const byte HWOP_DIGITAL        = 0x01;
const byte HWOP_ANALOG         = 0x02;
const byte HWOP_SERIAL         = 0xA0;

const byte HWOP_SERIAL1        = HWOP_SERIAL + 0;
const byte HWOP_SERIAL2        = HWOP_SERIAL + 1;
const byte HWOP_SERIAL3        = HWOP_SERIAL + 2;
const byte HWOP_SERIAL4        = HWOP_SERIAL + 3;

/**
 * Actions
 */

const byte HWA_RESET           = 0x00;
const byte HWA_SET             = 0x01;
const byte HWA_READ            = 0x02;
const byte HWA_PEEK            = 0x03;
const byte HWA_WRITE           = 0x04;
const byte HWA_INPUT           = 0x05;
const byte HWA_OUTPUT          = 0x06;
const byte HWA_FLUSH           = 0x07;

class HardwareLayer {
public:

  /**
   * Perform a hardware operation
   */
  int hwop(const byte operation, const byte action, int value);

};

#endif
