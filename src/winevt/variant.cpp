#include "variant.h"

namespace WinUptime {

Variant::Variant()
{
  variant_.Count = 0;
  variant_.Type = EvtVarTypeNull;
}

int32_t Variant::getInt(bool* ok) const noexcept
{
  *ok = true;

  switch (getType()) {
  case EvtVarTypeSByte:
    return variant_.SByteVal;

  case EvtVarTypeByte:
    return variant_.ByteVal;

  case EvtVarTypeInt16:
    return variant_.Int16Val;

  case EvtVarTypeUInt16:
    return variant_.UInt16Val;

  case EvtVarTypeInt32:
    return variant_.Int32Val;

  default:
    *ok = false;
    return 0;
  }
}

uint32_t Variant::getUInt(bool* ok) const noexcept
{
  *ok = true;

  switch (getType()) {
  case EvtVarTypeByte:
    return variant_.ByteVal;

  case EvtVarTypeUInt16:
    return variant_.UInt16Val;

  case EvtVarTypeUInt32:
    return variant_.UInt32Val;

  default:
    *ok = false;
    return 0;
  }
}

} // namespace WinUptime
