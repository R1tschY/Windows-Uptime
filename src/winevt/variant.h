/*
 *  This file is part of Windows-Uptime.
 *
 *  Copyright (C) 2014-2015 R1tschY <r1tschy@yahoo.de>
 *
 *  Windows-Uptime is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TrafficIndicator is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef VARIANT_H
#define VARIANT_H

#include <windows.h>
#include <winevt.h>

#include <cstdint>

namespace WinUptime {

class Variant
{
public:
  Variant();

  DWORD getType() const noexcept {
    return variant_.Type;
  }

  const wchar_t* getString() const noexcept {
    return (variant_.Type != EvtVarTypeString) ? nullptr : variant_.StringVal;
  }

  const ULONGLONG* getFileTime() const noexcept {
    return (variant_.Type != EvtVarTypeFileTime) ? nullptr : &variant_.FileTimeVal;
  }

  const uint16_t* getUInt16() const noexcept {
    return (variant_.Type != EvtVarTypeUInt16) ? nullptr : &variant_.UInt16Val;
  }

  int32_t getInt(bool* ok) const noexcept;
  uint32_t getUInt(bool* ok) const noexcept;

private:
  EVT_VARIANT variant_;
};

} // namespace WinUptime

#endif // VARIANT_H
