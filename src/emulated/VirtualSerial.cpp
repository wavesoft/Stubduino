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

#include "VirtualSerial.h"
#include "Hardware.h"

VirtualSerial::VirtualSerial(byte index)
  : category(HWOP_SERIAL + index), pending("")
{ }

/**
 * Wait for data to become available, and stack them to the pending
 * string queue.
 */
unsigned char VirtualSerial::available()
{
  int ret = Hardware::layer->hwop(category, HWA_PEEK, 0);
  if (ret >= 0) {
    pending += (char) Hardware::layer->hwop(category, HWA_READ, 0);
  }

  return pending.length();
}

/**
 * Void function
 */
void VirtualSerial::begin(int baud)
{
  Hardware::layer->hwop(
    category, HWA_RESET, baud
  );
}

/**
 * Flush out stream
 */
void VirtualSerial::flush()
{
  Hardware::layer->hwop(
    category, HWA_FLUSH, 0
  );
}

/**
 * Read a character
 */
char VirtualSerial::read()
{
  if (pending.length() == 0) {
    available();
  }

  std::string::iterator it = pending.begin();
  char c = *it;

  pending.erase(it);

  return c;
}

/**
 * Write a character
 */
void VirtualSerial::write(char data)
{
  Hardware::layer->hwop(
    category, HWA_WRITE, (unsigned char) data
  );
}
