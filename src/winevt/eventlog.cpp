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

#include "eventlog.h"

#include "winexception.h"

#include <QDebug>

namespace WinUptime {

EventLog::EventLog(EventHandle&& handle, const std::wstring& path, EVT_QUERY_FLAGS flags) :
  handle_(std::move(handle)), path_(path.begin(), path.end()), flags_(flags)
{ }

EventChannelIterator EventLog::getLocalChannels()
{
  EventHandle handle(EvtOpenChannelEnum(nullptr, 0));
  if (handle == nullptr) {
    throw WinException(L"EvtOpenChannelEnum", GetLastError());
  }
  return EventChannelIterator(std::move(handle));
}

EventIterator EventLog::query(const std::wstring& query, EVT_QUERY_FLAGS flags)
{
  EventHandle handle(EvtQuery(handle_.get(), path_.data(), query.data(), flags_ | flags));
  if (handle == nullptr) {
    throw WinException(L"EvtQuery", GetLastError());
  }
  return EventIterator(std::move(handle));
}

} // namespace WinUptime
