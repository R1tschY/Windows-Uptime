#ifndef WINEXCEPTION_H
#define WINEXCEPTION_H

#include <windows.h>

#include <string>
#include <boost/utility/string_ref.hpp>

#include "errors.h"

namespace WinUptime {

class Exception {
public:
  explicit Exception(DWORD error_code) noexcept :
    error_code_(error_code)
  { }

  virtual ~Exception() noexcept;

  virtual const wchar_t* what() const noexcept = 0;

  DWORD code() const noexcept {
    return error_code_;
  }

private:
  DWORD error_code_;
};

class WinException : Exception {
public:
  explicit WinException(DWORD error_code) noexcept;
  explicit WinException(boost::wstring_ref info, DWORD error_code) noexcept;
  explicit WinException(DWORD error_code, std::wstring message) noexcept;

  ~WinException() noexcept override;

  const wchar_t* what() const noexcept override;

private:
  std::wstring message_;
};

class InvalidQueryException : WinException {
public:
  explicit InvalidQueryException() noexcept;
};

} // namespace WinUptime

#endif // WINEXCEPTION_H
