#include "uptimerequest.h"

#include "winevt/eventlog.h"
#include "winevt/rendercontext.h"

#include <QDebug>

namespace WinUptime {

UptimeRequest::UptimeRequest()
{
}

void WinUptime::UptimeRequest::loadAll()
{
  EventLog log = EventLog::getLocalLog();
  EventIterator iter = log.queryChannel(
        L"System",
        L"*[System/Provider[@Name=\"Microsoft-Windows-Kernel-General\"]]");

  RenderContext context;
  const wchar_t* queryValues[] = {
    L"Event/System/EventID",
//    L"Event/System/TimeCreated[@SystemTime]"
  };
  context.openForValues(queryValues);

  Event event;
  Variant results[1];
  while (iter.next(event)) {
    context.getValues(event, results);

    PowerEvent::Type type;
    const uint16_t* eventid = results[0].getUInt16();
    if (eventid) {
      switch (*eventid) {
      case 1:
        type = PowerEvent::Type::RESUME;
        break;

      case 12:
        type = PowerEvent::Type::BOOT_UP;
        break;

      case 13:
        type = PowerEvent::Type::SHUTDOWN;
        break;
      }

    }

    database_.emplace_back(type, FILETIME{0, 0});

    /*
    FILETIME* time = results[1].getFileTime();
    if (time) {

    }*/

  }
}

} // namespace WinUptime
