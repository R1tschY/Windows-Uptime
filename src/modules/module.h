#ifndef WINUPTIME_MODULE_H
#define WINUPTIME_MODULE_H

#include <QObject>
#include <QTime>
#include <windows.h>

#include "../data/itemmodel.h"

namespace WinUptime {

class SystemEventLog;
class EventLog;

class ModuleModel : public QObject
{
  Q_OBJECT
public:

  ModuleModel(QObject* parent = nullptr);

  virtual Items scanRange(SystemTime start, SystemTime end, EventLog& log) = 0;
};

class Module
{
public:
  virtual ~Module();

  virtual ModuleModel* createModel(QObject* parent) = 0;
};

} // namespace WinUptime

#endif // WINUPTIME_MODULE_H
