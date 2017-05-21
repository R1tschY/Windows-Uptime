#include "powerstatemodule.h"

#include <cstdint>
#include <algorithm>
#include <QDebug>
#include <QDateTime>
#include <QTime>
#include <QThread>
#include <QDir>
#include <cassert>
#include <deque>
#include <QApplication>
#include <QStyle>

#include "../winevt/eventlog.h"
#include "../winevt/rendercontext.h"
#include "../winevt/winexception.h"
#include "../data/itemmodel.h"

namespace WinUptime {

struct PowerEvent {
public:
  enum class Type {
    BootUp,
    Shutdown,
    Suspend,
    Resume,
    SystemTimeTurnForth,
    SystemTimeTurnedBack
  };

  PowerEvent(Type type, ULONGLONG time, ULONGLONG time_needed = 0) :
    time_needed_(time_needed), time_(time), type_(type)
  { }

  void print() const;

  Type getType() const { return type_; }
  SystemTime getTime() const { return time_; }
  ULONGLONG getTimeNeeded() const { return time_needed_; }
  QString getTypeString() const;

private:
  ULONGLONG time_needed_;
  SystemTime time_;
  Type type_;
};

class PowerStateModel : public ModuleModel
{
  Q_DECLARE_TR_FUNCTIONS(PowerStateModel)
public:
  PowerStateModel(const PowerStateModule* module, QObject* parent);
  ~PowerStateModel();

  Items scanRange(SystemTime start, SystemTime end, EventLog& log) override;

  const PowerStateModule* getModule() const { return module_; }

private:
  std::deque<PowerEvent> database_;
  EventLog* log_;
  const PowerStateModule* module_;

  void scanLog();
};

enum class PowerState {
  On,
  Off,
  Suspended,
  Unknown
};

class DayUptimeCalculator {
public:
  DayUptimeCalculator(Items& items, const PowerStateModule* module);

  void operator()(const PowerEvent& event);
  void finish();

private:
  PowerState state_ = PowerState::Unknown;
  SystemTime state_start_time_ { 0 };
  Items& items_;
  const PowerStateModule* module_;
};


class PowerEventSpotsAdder {
public:
  PowerEventSpotsAdder(Items& items, const PowerStateModule* module)
    : items_(items), module_(module)
  { }

  void operator()(const PowerEvent& event);

private:
  Items& items_;
  const PowerStateModule* module_;
};


ModuleModel* PowerStateModule::createModel(QObject* parent)
{
  return new PowerStateModel(this, parent);
}

PowerStateModel::PowerStateModel(const PowerStateModule* module, QObject* parent)
  : ModuleModel(parent), module_(module)
{ }

PowerStateModel::~PowerStateModel() = default;

Items PowerStateModel::scanRange(SystemTime start, SystemTime end, EventLog& log)
{
  if (log_ != &log)
  {
    log_ = & log;

    database_.clear();
    scanLog();
  }

  Items result;
  DayUptimeCalculator c(result, module_);
  PowerEventSpotsAdder pesa(result, module_);

  // do better with std::sort, std::lower_bound and std::upper_bound
  for (auto i = database_.cbegin(); i != database_.cend(); i++) {
    const auto& event = *i;
    if (event.getTime() > start && event.getTime() < end) {
      event.print();
      c(event);
      pesa(event);
    }
  }
  return result;
}

void PowerStateModel::scanLog()
{
  try {
    RenderContext id_context(L"Event/System/EventID");
    Event event;

    EventIterator kernel_general = log_->query(L"*[System/Provider[@Name=\"Microsoft-Windows-Kernel-General\"]]");
    RenderContext time_changed_context({
                                   L"Event/EventData/Data[@Name=\"OldTime\"]",
                                   L"Event/EventData/Data[@Name=\"NewTime\"]"
                                 });
    RenderContext boot_up_context(L"Event/EventData/Data[@Name=\"StartTime\"]");
    RenderContext shutdown_context(L"Event/EventData/Data[@Name=\"StopTime\"]");
    while (kernel_general.next(event)) {
      Variant result = id_context.getValue(event);

      const uint16_t* eventid = result.getUInt16();
      if (eventid) {
        switch (*eventid) {
        case 1: {
          Variant results[2];
          time_changed_context.getValues(event, results);
          uint64_t old_time = *results[0].getFileTimeChecked();
          uint64_t new_time = *results[1].getFileTimeChecked();

          if (old_time < new_time) {
            database_.emplace_back(
                  PowerEvent::Type::SystemTimeTurnForth,
                  old_time,
                  new_time - old_time);
          } else {
            database_.emplace_back(
                  PowerEvent::Type::SystemTimeTurnedBack,
                  old_time,
                  old_time - new_time);
          }
          break;
        }

        case 12:
          database_.emplace_back(
                PowerEvent::Type::BootUp,
                *boot_up_context.getValue(event).getFileTimeChecked());
          break;

        case 13:
          database_.emplace_back(
                PowerEvent::Type::Shutdown,
                *shutdown_context.getValue(event).getFileTimeChecked());
          break;
        }
      }
    }

    EventIterator kernel_power = log_->query(L"*[System/Provider[@Name=\"Microsoft-Windows-Kernel-Power\"]]");
    RenderContext time_created_context(L"Event/System/TimeCreated/@SystemTime");
    while (kernel_power.next(event)) {
      Variant result = id_context.getValue(event);

      const uint16_t* eventid = result.getUInt16();
      if (eventid && *eventid == 42) {
        database_.emplace_back(
              PowerEvent::Type::Suspend,
              *time_created_context.getValue(event).getFileTimeChecked());
      }
    }

    EventIterator power_troubleshooting = log_->query(L"*[System/Provider[@Name=\"Microsoft-Windows-Power-Troubleshooter\"]]");
    RenderContext wake_time(L"Event/EventData/Data[@Name=\"WakeTime\"]");
    while (power_troubleshooting.next(event)) {
      Variant result = id_context.getValue(event);

      const uint16_t* eventid = result.getUInt16();
      if (eventid && *eventid == 1) {
        database_.emplace_back(
              PowerEvent::Type::Resume,
              *wake_time.getValue(event).getFileTimeChecked());
      }
    }

    std::sort(database_.begin(), database_.end(),
              [](const PowerEvent& a, const PowerEvent& b) -> bool {
      return a.getTime() < b.getTime();
    });

    qDebug() << "Database size: " << database_.size() * sizeof(PowerEvent);
  }
  catch (WinException exp)
  {
    qDebug() << QString::fromWCharArray(exp.what());
  }
  catch (std::exception exp)
  {
    qDebug() << QString(exp.what());
  }
}

DayUptimeCalculator::DayUptimeCalculator(Items& items, const PowerStateModule* module):
  items_(items), module_(module)
{ }

void DayUptimeCalculator::operator()(const PowerEvent& event)
{
  RangeItem item(module_);

  if (event.getType() != PowerEvent::Type::SystemTimeTurnForth &&
      event.getType() != PowerEvent::Type::SystemTimeTurnedBack)
  {
    switch (state_) {
    case PowerState::Suspended:
      item.setRange(state_start_time_.toDateTime().toLocalTime(), event.getTime().toDateTime().toLocalTime());
      item.setCaption("Suspended");
      item.setColor(Qt::yellow);
      item.setDescription(
            QString("Suspended from %1 to %2").arg(
              item.start().toString(Qt::TextDate),
              item.end().toString(Qt::TextDate)));
      item.setType("powerstate.suspended");
      items_.addRange(item);
      break;

    case PowerState::On:
      item.setRange(state_start_time_.toDateTime().toLocalTime(), event.getTime().toDateTime().toLocalTime());
      item.setCaption("On");
      item.setColor(Qt::green);
      item.setDescription(
            QString("On from %1 to %2").arg(
              item.start().toString(Qt::TextDate),
              item.end().toString(Qt::TextDate)));
      item.setType("powerstate.on");
      items_.addRange(item);
      break;

    case PowerState::Off:
      break;

    case PowerState::Unknown:
      switch (event.getType()) {
      case PowerEvent::Type::Shutdown:
      case PowerEvent::Type::Suspend:
        item.setRange(state_start_time_.toDateTime().toLocalTime(), event.getTime().toDateTime().toLocalTime());
        item.setCaption("On");
        item.setColor(Qt::green);
        item.setType("powerstate.on");
        items_.addRange(item);
        break;

      case PowerEvent::Type::Resume:
        item.setRange(state_start_time_.toDateTime().toLocalTime(), event.getTime().toDateTime().toLocalTime());
        item.setCaption("Suspended");
        item.setColor(Qt::yellow);
        item.setType("powerstate.suspended");
        items_.addRange(item);
        break;

      case PowerEvent::Type::BootUp:
        break;
      }
      break;

    default:
      break;
    }
  }

  // set new state
  switch (event.getType()) {
  case PowerEvent::Type::Suspend:
    state_ = PowerState::Suspended;
    state_start_time_ = event.getTime();
    break;

  case PowerEvent::Type::Shutdown:
    state_ = PowerState::Off;
    state_start_time_ = event.getTime();
    break;

  case PowerEvent::Type::BootUp:
  case PowerEvent::Type::Resume:
    state_ = PowerState::On;
    state_start_time_ = event.getTime();
    break;

  case PowerEvent::Type::SystemTimeTurnForth:
    // TODO
    break;

  case PowerEvent::Type::SystemTimeTurnedBack:
    // TODO
    break;
  }
}

void DayUptimeCalculator::finish()
{
  // TODO
}

void PowerEventSpotsAdder::operator()(const PowerEvent& event)
{
  SpotItem spot(module_);
  spot.setTime(event.getTime().toDateTime().toLocalTime());

  switch (event.getType()) {
  case PowerEvent::Type::Suspend:
    spot.setCaption(PowerStateModel::tr("Suspended"));
    spot.setType("powerstate.suspend");
    spot.setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPause));
    break;

  case PowerEvent::Type::Shutdown:
    spot.setCaption(PowerStateModel::tr("Shutdown"));
    spot.setIcon(QIcon(":/img/down.png"));
    spot.setType("powerstate.shutdown");
    break;

  case PowerEvent::Type::BootUp:
    spot.setCaption(PowerStateModel::tr("Boot up"));
    spot.setIcon(QIcon(":/img/up.png"));
    spot.setType("powerstate.boot_up");
    break;

  case PowerEvent::Type::Resume:
    spot.setCaption(PowerStateModel::tr("Resume"));
    spot.setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    spot.setType("powerstate.resume");
    break;

  case PowerEvent::Type::SystemTimeTurnForth:
    spot.setCaption(PowerStateModel::tr("System Time Changed"));
    spot.setIcon(QIcon(":/img/app.png"));
    spot.setType("powerstate.systemtime.turn_forth");
    break;

  case PowerEvent::Type::SystemTimeTurnedBack:
    spot.setCaption(PowerStateModel::tr("System Time Changed"));
    spot.setIcon(QIcon(":/img/app.png"));
    spot.setType("powerstate.systemtime.turn_back");
    break;
  }

  items_.addSpot(spot);
}

void PowerEvent::print() const
{
  qDebug() << getTypeString()
           << " @" << time_.toDateTime().toString(Qt::RFC2822Date);
}

QString PowerEvent::getTypeString() const
{
  switch (type_) {
  case Type::BootUp:
    return QStringLiteral("Boot up");

  case Type::Shutdown:
    return QStringLiteral("Shutdown");

  case Type::Suspend:
    return QStringLiteral("Suspend");

  case Type::Resume:
    return QStringLiteral("Resume");

  case Type::SystemTimeTurnForth:
    return QStringLiteral("SystemTimeTurnForth");

  case Type::SystemTimeTurnedBack:
    return QStringLiteral("SystemTimeTurnedBack");

  default:
    return QString();
  }
}

} // namespace WinUptime
