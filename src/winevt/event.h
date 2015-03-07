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

#ifndef EVENT_H
#define EVENT_H

#include "eventhandle.h"

namespace WinUptime {

class Event {
public:
  explicit Event() noexcept :
    handle_(nullptr)
  {
  }

  explicit Event(EventHandle&& handle) noexcept :
    handle_(std::move(handle))
  { }

  explicit Event(Event&& other) noexcept :
    handle_(std::move(other.handle_))
  { }

  EVT_HANDLE getHandle() const {
    return handle_.get();
  }

private:
  EventHandle handle_;
};

} // namespace WinUptime

#endif // EVENT_H
