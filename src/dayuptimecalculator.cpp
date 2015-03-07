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

#include "uptimeview.h"

namespace WinUptime {

DayUptimeCalculator::DayUptimeCalculator(EventModel* model):
  model_(model), last_day_(1), uptime_(0, 0), ontime_(0, 0)
{ }

void DayUptimeCalculator::operator()(PowerEvent event) {
  QDateTime date = event.getTime().toDateTime();
  unsigned day = date.date().day();

  if (day != last_day_) {
    int remainingMs = 24*3600*1000 - last_time_.msecsSinceStartOfDay();

    switch (event.getType()) {
    case PowerEvent::Type::BOOT_UP:
      model_->addRow(last_day_, uptime_, ontime_);
      break;

    case PowerEvent::Type::SUSPEND:
    case PowerEvent::Type::SHUTDOWN:
      uptime_ = uptime_.addMSecs(remainingMs);
      ontime_ = ontime_.addMSecs(remainingMs);
      model_->addRow(last_day_, uptime_, ontime_);
      break;

    case PowerEvent::Type::RESUME:
      ontime_ = ontime_.addMSecs(remainingMs);
      model_->addRow(last_day_, uptime_, ontime_);
      break;
    }

    for (unsigned i = last_day_ + 1; i < day; i++) {
      switch (event.getType()) {
      case PowerEvent::Type::BOOT_UP:
        model_->addRow(i, QTime(0, 0, 0), QTime(0, 0, 0));
        break;

      case PowerEvent::Type::SUSPEND:
      case PowerEvent::Type::SHUTDOWN:
        model_->addRow(i, QTime(24, 0, 0), QTime(24, 0, 0));
        break;

      case PowerEvent::Type::RESUME:
        model_->addRow(i, QTime(0, 0, 0), QTime(24, 0, 0));
        break;
      }
    }

    uptime_ = QTime(0,0);
    ontime_ = QTime(0,0);
    last_day_ = day;
  }

  int newMs = last_time_.msecsTo(date.time());
  switch (event.getType()) {
  case PowerEvent::Type::SUSPEND:
  case PowerEvent::Type::SHUTDOWN:
    uptime_ = uptime_.addMSecs(newMs);
    ontime_ = ontime_.addMSecs(newMs);
    break;

  case PowerEvent::Type::RESUME:
    ontime_ = ontime_.addMSecs(newMs);
    break;
  }
  last_time_ = date.time();

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

void DayUptimeCalculator::finish(unsigned last_day_of_mouth) {
  int remainingMs = 24*3600*1000 - last_time_.msecsSinceStartOfDay();

  switch (state_) {
  case PowerState::Off:
    model_->addRow(last_day_, uptime_, ontime_);
    break;

  case PowerState::On:
    uptime_ = uptime_.addMSecs(remainingMs);
    ontime_ = ontime_.addMSecs(remainingMs);
    model_->addRow(last_day_, uptime_, ontime_);
    break;

  case PowerState::Suspended:
    ontime_ = ontime_.addMSecs(remainingMs);
    model_->addRow(last_day_, uptime_, ontime_);
    break;
  }

  for (unsigned i = last_day_ + 1; i < last_day_of_mouth; i++) {
    switch (state_) {
    case PowerState::Off:
      model_->addRow(i, QTime(0, 0, 0), QTime(0, 0, 0));
      break;

    case PowerState::On:
      model_->addRow(i, QTime(24, 0, 0), QTime(24, 0, 0));
      break;

    case PowerState::Suspended:
      model_->addRow(i, QTime(0, 0, 0), QTime(24, 0, 0));
      break;
    }
  }
}


} // namespace WinUptime
