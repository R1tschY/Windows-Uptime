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

#include "dayuptimecalculator.h"

#include <cassert>
#include <QDebug>
#include "uptimeview.h"

namespace WinUptime {

DayUptimeCalculator::DayUptimeCalculator(EventModel* model):
  model_(model), last_time_(0,0), uptime_(0, 0), ontime_(0, 0)
{ }

void DayUptimeCalculator::operator()(const PowerEvent& event) {
  QDateTime date = event.getTime().toDateTime();
  int day = date.date().day(); // FIXME

  if (day != last_day_) {
    assert(day - last_day_ >= 0);
    processUntilDay(day);
  }

  int newMs = last_time_.msecsTo(date.time());
  process(newMs);

  last_time_ = date.time();
  advance(event);
}

void DayUptimeCalculator::finish(int last_day_of_mouth) {
  processUntilDay(last_day_of_mouth+1);
}

void DayUptimeCalculator::processUntilDay(int day)
{
  int remainingMs = MsPerDay - last_time_.msecsSinceStartOfDay();

  // process last time of day
  process(remainingMs);
  model_->addRow(last_day_, uptime_, ontime_);

  // process days between
  for (int i = last_day_ + 1; i < day; i++) {
    uptime_ = QTime(0,0);
    ontime_ = QTime(0,0);
    last_time_ = QTime(0,0);
    last_day_ = i;

    process(MsPerDay);
    model_->addRow(i, uptime_, ontime_);
  }

  // start new day
  uptime_ = QTime(0,0);
  ontime_ = QTime(0,0);
  last_time_ = QTime(0,0);
  last_day_ = day;
}

void DayUptimeCalculator::process(int time_step)
{
  // process time
  assert(time_step >= 0);

  switch (state_) {
  case PowerState::Off:
    break;

  case PowerState::On:
    uptime_ = uptime_.addMSecs(time_step);

  case PowerState::Suspended:
    ontime_ = ontime_.addMSecs(time_step);
    break;

  case PowerState::Unknown:
    // TODO
    break;
  }
}

void DayUptimeCalculator::advance(const PowerEvent& event)
{
  // set new state
  switch (event.getType()) {
  case PowerEvent::Type::SUSPEND:
    state_ = PowerState::Suspended;
    break;

  case PowerEvent::Type::SHUTDOWN:
    state_ = PowerState::Off;
    break;

  case PowerEvent::Type::BOOT_UP:
  case PowerEvent::Type::RESUME:
    state_ = PowerState::On;
    break;
  }
}

} // namespace WinUptime
