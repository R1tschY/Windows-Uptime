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

#ifndef EVENTLOG_H
#define EVENTLOG_H

#include <string>

#include "eventchanneliterator.h"
#include "eventiterator.h"

#include "eventhandle.h"

namespace WinUptime {

class EventLog
{
public:  
  static EventLog getLocalChannel(const std::wstring& channel_name) {
    return EventLog(EventHandle(nullptr), channel_name, EvtQueryChannelPath);
  }

  static EventLog getFileLog(const std::wstring& path) {
    return EventLog(EventHandle(nullptr), path, EvtQueryFilePath);
  }

  static EventChannelIterator getLocalChannels();

  EventIterator query(const std::wstring& q, bool reverse = false) {
    return query(q, reverse ? EvtQueryForwardDirection : EvtQueryReverseDirection);
  }

  EventIterator queryAll(bool reverse = false) {
    return query(L"*", reverse);
  }

private:
  EventHandle handle_;
  std::wstring path_;
  DWORD flags_;

  EventLog(EventHandle&& handle, const std::wstring& path, EVT_QUERY_FLAGS flags);

  EventIterator query(const std::wstring& query, EVT_QUERY_FLAGS flags);
};

} // namespace WinUptime

#endif // EVENTLOG_H
