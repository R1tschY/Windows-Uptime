/*
 *  This file is part of Windows-Uptime.
 *
 *  Copyright (C) 2014-2015 R1tschY <r1tschy@yahoo.de>
 *
 *  Windows-Uptime is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TrafficIndicator is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DAYUPTIMECALCULATOR_H
#define DAYUPTIMECALCULATOR_H

#include <QTime>

#include "uptimerequest.h"

namespace WinUptime {

enum class PowerState {
  On,
  Off,
  Suspended,
  Unknown
};

class EventModel;

class DayUptimeCalculator {
public:
  DayUptimeCalculator(EventModel* model);

  void operator()(PowerEvent event);

  void finish(unsigned last_day_of_mouth);

private:
  PowerState state_ = PowerState::Unknown;
  EventModel* model_;
  unsigned last_day_;
  QTime last_time_;
  QTime uptime_;
  QTime ontime_;
};

} // namespace WinUptime

#endif // DAYUPTIMECALCULATOR_H
