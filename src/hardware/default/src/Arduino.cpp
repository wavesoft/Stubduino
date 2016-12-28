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

#include <Arduino.h>
#include <emulated/Hardware.h>

void pinMode(const int pin, const unsigned char mode)
{
  Hardware::layer->hwop(HWOP_CONFIGURATION, mode, pin);
}

void digitalWrite(const int pin, const unsigned char value)
{
  Hardware::layer->hwop(HWOP_DIGITAL, value, pin);
}

unsigned char digitalRead(const int pin)
{
  return Hardware::layer->hwop(HWOP_DIGITAL, HWA_READ, pin);
}

void delay(const int ms)
{

}
