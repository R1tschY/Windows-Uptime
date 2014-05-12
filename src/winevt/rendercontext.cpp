#include "rendercontext.h"

#include <QDebug>

#include "winexception.h"

namespace WinUptime {

void RenderContext::openForValues(const wchar_t* const* values, std::size_t n)
{
  handle_.reset(EvtCreateRenderContext(n, const_cast<const wchar_t**>(values), EvtRenderEventValues));
  auto error_code = GetLastError();
  if (!handle_ || error_code)
  {
      throw WinException(L"EvtCreateRenderContext", error_code);
  }
}

void RenderContext::getValues(const Event& event, Variant* values, std::size_t n)
{
  EVT_HANDLE handle = handle_.get();
  DWORD used = 0;
  DWORD properties = 0;
  bool success = EvtRender(
        handle,
        event.getHandle(),
        EvtRenderEventValues,
        n * sizeof(EVT_VARIANT),
        values,
        &used,
        &properties);

  if (!success) {
    throw WinException(L"EvtRender: ", GetLastError());
  }

  if (n * sizeof(EVT_VARIANT) > used || n != properties) {
    throw WinException(L"EvtRender: ", ERROR_BAD_ARGUMENTS);
  }
}

} // namespace WinUptime
