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

#ifndef EVENTHANDLE_H
#define EVENTHANDLE_H

#include <windows.h>
#include <winevt.h>

#include <memory>

namespace WinUptime {

struct EventHandle_finalizer {
  using pointer = EVT_HANDLE;

  void operator() (EVT_HANDLE handle) const noexcept {
    EvtClose(handle);
  }
};

using EventHandle = std::unique_ptr<EVT_HANDLE, EventHandle_finalizer>;

} // namespace WinUptime

#endif // EVENTHANDLE_H
