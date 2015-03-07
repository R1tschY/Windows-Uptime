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

#ifndef EVENTITERATOR_H
#define EVENTITERATOR_H

#include <vector>
#include <chrono>

#include "event.h"

namespace WinUptime {

class EventIterator {
public:
  explicit EventIterator(EventHandle&& handle) :
    handle_(std::move(handle))
  { }

  bool next(Event& event);

private:
  EventHandle handle_;
};

} // namespace WinUptime

#endif // EVENTITERATOR_H
