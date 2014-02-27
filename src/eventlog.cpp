#include "eventlog.h"

#include <memory>

namespace WinUptime {

WinException::WinException(DWORD error_code) :
  error_code_(error_code), message_()
{
  wchar_t* buffer;
  DWORD bytes = FormatMessageW(
      FORMAT_MESSAGE_ALLOCATE_BUFFER |
      FORMAT_MESSAGE_FROM_SYSTEM |
      FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      error_code,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      (wchar_t*)&buffer,
      0, NULL);

  if (bytes == 0 || buffer == NULL) {
    return;
  }

  std::size_t src_size = wcslen(buffer);
  std::unique_ptr<char[]> ansi(new char [src_size + 1]);
  WideCharToMultiByte(CP_UTF8, 0, buffer, src_size, ansi.get(), src_size, NULL, NULL);
  LocalFree(buffer);
  message_.assign(ansi.get());
}

WinException::~WinException()
{
}

const char* WinException::what() const throw()
{
  return message_.c_str();
}

DWORD WinException::code() const throw()
{
  return error_code_;
}

EventLog::EventLog()
{
}

ChannelPathIterator EventLog::getChannelPaths()
{
  EventHandle handle(EvtOpenChannelEnum(nullptr, 0));
  if (handle == nullptr) {
    throw WinException(GetLastError());
  }
  return ChannelPathIterator(std::move(handle));
}

EventIterator EventLog::query(const std::wstring& path, const std::wstring& query, DWORD flags)
{
  EventHandle handle(EvtQuery(nullptr, path.c_str(), query.c_str(), flags));
  if (handle == nullptr) {
    throw WinException(GetLastError());
  }
  return EventIterator(std::move(handle));
}

EventIterator EventLog::queryAll(const std::wstring& path, DWORD flags)
{
  EventHandle handle(EvtQuery(nullptr, path.c_str(), nullptr, flags));
  if (handle == nullptr) {
    throw WinException(GetLastError());
  }
  return EventIterator(std::move(handle));
}

bool ChannelPathIterator::next(std::wstring& path)
{
  wchar_t buffer[1024];
  DWORD size;

  bool success = EvtNextChannelPath(handle_.get(), sizeof(buffer), buffer, &size);
  if (success) {
    path.assign(buffer, buffer + size);
    return true;

  } else {
    auto error = GetLastError();
    if (error == ERROR_INSUFFICIENT_BUFFER) {
      wchar_t bigbuffer[size];
      bool success = EvtNextChannelPath(handle_.get(), sizeof(bigbuffer), bigbuffer, &size);
      if (success) {
        path.assign(bigbuffer, bigbuffer + size);
        return true;
      }
    }

    return false;
  }
}

bool EventIterator::next(std::vector<EventHandle>& events, int n,
                         std::chrono::milliseconds timeout)
{
  DWORD size;

  events.resize(n);
  bool success = EvtNext(handle_.get(), n, (void**)events.data(),
                         timeout.count(), 0, &size);
  if (success) {
    events.resize(size);
    return true;

  } else {
    events.clear();

    DWORD error = GetLastError();
    if (error == ERROR_NO_MORE_ITEMS) {
      return false;
    }

    throw WinException(error);
  }
}

} // namespace WinUptime
