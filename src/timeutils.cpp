#include "timeutils.h"

namespace WinUptime {

QString humanize(const QTime& time, hhmmss_tag)
{
   return time.toString("h'h'm'm's's'");
}



} // namespace WinUptime
