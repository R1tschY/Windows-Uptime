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

#include "variant.h"

#include "../utils.h"

namespace WinUptime {

Variant::Variant()
{
  variant_.Count = 0;
  variant_.Type = EvtVarTypeNull;
}

int32_t Variant::getInt(bool* ok) const noexcept
{
  *ok = true;

  switch (getType()) {
  case EvtVarTypeSByte:
    return variant_.SByteVal;

  case EvtVarTypeByte:
    return variant_.ByteVal;

  case EvtVarTypeInt16:
    return variant_.Int16Val;

  case EvtVarTypeUInt16:
    return variant_.UInt16Val;

  case EvtVarTypeInt32:
    return variant_.Int32Val;

  default:
    *ok = false;
    return 0;
  }
}

uint32_t Variant::getUInt(bool* ok) const noexcept
{
  *ok = true;

  switch (getType()) {
  case EvtVarTypeByte:
    return variant_.ByteVal;

  case EvtVarTypeUInt16:
    return variant_.UInt16Val;

  case EvtVarTypeUInt32:
    return variant_.UInt32Val;

  default:
    *ok = false;
    return 0;
  }
}

} // namespace WinUptime
