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

#include "eventchanneliterator.h"
#include "winexception.h"

namespace WinUptime {

bool EventChannelIterator::next(std::wstring& path)
{
  wchar_t buffer[1024];
  DWORD size;

  bool success = EvtNextChannelPath(handle_.get(), sizeof(buffer), buffer, &size);
  if (success) {
    path.assign(buffer, buffer + size);
    return true;
  }

  auto error = GetLastError();
  switch (error) {
  case ERROR_NO_MORE_ITEMS:
    return false;

  case ERROR_INSUFFICIENT_BUFFER: {
    wchar_t bigbuffer[size];
    bool success = EvtNextChannelPath(handle_.get(), sizeof(bigbuffer), bigbuffer, &size);
    if (success) {
      path.assign(bigbuffer, bigbuffer + size);
      return true;
    }
    error = GetLastError();
    break;
  }
  }

  throw WinException(L"EvtNext", error);
}

} // namespace WinUptime

