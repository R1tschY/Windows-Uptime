#ifndef VARIANT_H
#define VARIANT_H

#include <windows.h>
#include <winevt.h>

#include <cstdint>

namespace WinUptime {

class Variant
{
public:
  Variant();

  DWORD getType() const noexcept {
    return variant_.Type;
  }

  const wchar_t* getString() const noexcept {
    return (variant_.Type != EvtVarTypeString) ? nullptr : variant_.StringVal;
  }

  const ULONGLONG* getFileTime() const noexcept {
    return (variant_.Type != EvtVarTypeFileTime) ? nullptr : &variant_.FileTimeVal;
  }

  const uint16_t* getUInt16() const noexcept {
    return (variant_.Type != EvtVarTypeUInt16) ? nullptr : &variant_.UInt16Val;
  }

  int32_t getInt(bool* ok) const noexcept;
  uint32_t getUInt(bool* ok) const noexcept;

private:
  EVT_VARIANT variant_;
};

} // namespace WinUptime

#endif // VARIANT_H
