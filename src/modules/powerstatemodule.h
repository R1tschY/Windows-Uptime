#ifndef WINUPTIME_POWERSTATEMODULE_H
#define WINUPTIME_POWERSTATEMODULE_H

#include "module.h"

namespace WinUptime {

class PowerStateModule : public Module
{
public:
  ModuleModel* createModel(QObject* parent) override;
};

} // namespace WinUptime

#endif // WINUPTIME_POWERSTATEMODULE_H
