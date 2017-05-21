#ifndef TIMEUTILS_H
#define TIMEUTILS_H

#include <QDate>
#include <QString>

namespace WinUptime {

struct hhmmss_tag {};
struct hms_tag {};

QString humanize(const QTime& time, hms_tag);
QString humanize(const QTime& time, hhmmss_tag);

} // namespace WinUptime

#endif // TIMEUTILS_H
