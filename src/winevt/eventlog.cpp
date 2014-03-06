#include "eventlog.h"

#include "winexception.h"

namespace WinUptime {

EventChannelIterator EventLog::getChannelPaths()
{
  EventHandle handle(EvtOpenChannelEnum(handle_.get(), 0));
  if (handle == nullptr) {
    throw WinException(L"EvtOpenChannelEnum", GetLastError());
  }
  return EventChannelIterator(std::move(handle));
}

EventIterator EventLog::query(boost::wstring_ref path, boost::wstring_ref query, DWORD flags)
{
  SetLastError(0);
  EventHandle handle(EvtQuery(handle_.get(), path.data(), query.data(), flags));
  if (handle == nullptr || GetLastError() != ERROR_SUCCESS) {
    throw WinException(L"EvtQuery", GetLastError());
  }
  return EventIterator(std::move(handle));
}

} // namespace WinUptime
