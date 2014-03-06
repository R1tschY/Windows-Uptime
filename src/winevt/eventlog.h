#ifndef EVENTLOG_H
#define EVENTLOG_H

#include <string>

#include "eventchanneliterator.h"
#include "eventiterator.h"
#include <boost/utility/string_ref.hpp>

#include "eventhandle.h"

namespace WinUptime {

class EventLog
{
public:  
  static EventLog getLocalLog() {
    return EventLog(EventHandle(nullptr));
  }

  EventChannelIterator getChannelPaths();

  EventIterator queryFile(boost::wstring_ref path, boost::wstring_ref q, bool reverse = false) {
    DWORD dir = reverse ? EvtQueryForwardDirection : EvtQueryReverseDirection;
    return query(path, q, EvtQueryFilePath | dir);
  }

  EventIterator queryFileAll(boost::wstring_ref path, bool reverse = false) {
    return queryFile(path, L"*", reverse);
  }

  EventIterator queryChannel(boost::wstring_ref path, boost::wstring_ref q, bool reverse = false) {
    DWORD dir = reverse ? EvtQueryForwardDirection : EvtQueryReverseDirection;
    return query(path, q, EvtQueryChannelPath | dir);
  }

  EventIterator queryChannelAll(boost::wstring_ref path, bool reverse = false) {
    return queryChannel(path, L"*", reverse);
  }

private:
  EventHandle handle_;

  EventLog(EventHandle&& handle) :
    handle_(std::move(handle))
  { }

  EventIterator query(boost::wstring_ref path, boost::wstring_ref query, DWORD flags);
};

} // namespace WinUptime

#endif // EVENTLOG_H
