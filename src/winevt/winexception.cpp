#include "winexception.h"

#include <winevt.h>

#include <memory>
#include <QDebug>

namespace WinUptime {

Exception::~Exception() noexcept
{
}

namespace {

std::wstring get_windows_error_string(DWORD error_code) {
  std::wstring result;

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
    return result;
  }

  result.assign(buffer);
  LocalFree(buffer);

  return result;
}

}

WinException::WinException(DWORD error_code) noexcept :
  Exception(error_code), message_(get_windows_error_string(error_code))
{
}

WinException::WinException(boost::wstring_ref info, DWORD error_code) noexcept :
  Exception(error_code), message_(std::wstring(info) + L": " + get_windows_error_string(error_code))
{
}

WinException::WinException(DWORD error_code, std::wstring message) noexcept :
  Exception(error_code), message_(std::move(message))
{
}

WinException::~WinException()
{
}

const wchar_t* WinException::what() const noexcept
{
  return message_.c_str();
}

namespace {

std::wstring get_query_error_string() {
  wchar_t buffer[1024];
  std::wstring result = get_windows_error_string(ERROR_EVT_INVALID_QUERY);
  DWORD size;

  bool success = EvtGetExtendedStatus(sizeof(buffer), buffer, &size);
  if (success) {
    result.append(L": ");
    result.append(buffer, buffer + size);

  } else {
    if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
      wchar_t bigbuffer[size];
      bool success = EvtGetExtendedStatus(sizeof(bigbuffer), bigbuffer, &size);
      if (success) {
        result.append(L": ");
        result.append(bigbuffer, bigbuffer + size);
      }
    }
  }

  return result;
}

}

InvalidQueryException::InvalidQueryException() noexcept :
  WinException(ERROR_EVT_INVALID_QUERY, get_query_error_string())
{
}

} // namespace WinUptime

