#ifndef WINUPTIME_ITEMS_H
#define WINUPTIME_ITEMS_H

#include <windows.h>

#include <vector>
#include <QDateTime>
#include <QString>
#include <QTime>

namespace WinUptime {

class Module;
class ModuleModel;

class SystemTime {
public:
  explicit SystemTime(int64_t time = 0) :
    time_(time)
  { }

  SystemTime(FILETIME time) :
    time_((uint64_t(time.dwHighDateTime) << 32) + time.dwLowDateTime)
  { }

  SystemTime(const QDateTime& time);

  int64_t toInt() const {
    return time_;
  }

  FILETIME toFileTime() const {
    return FILETIME{uint32_t(time_), uint32_t(time_ >> 32)};
  }

  QDateTime toDateTime() const;

  bool operator< (const SystemTime& other) const { return time_ <  other.time_;  }
  bool operator> (const SystemTime& other) const { return time_ >  other.time_;  }
  bool operator>=(const SystemTime& other) const { return time_ >= other.time_;  }
  bool operator<=(const SystemTime& other) const { return time_ <= other.time_;  }
  bool operator==(const SystemTime& other) const { return time_ == other.time_;  }
  bool operator!=(const SystemTime& other) const { return time_ != other.time_;  }

  int64_t operator-(const SystemTime& other) const { return time_ - other.time_;  }

private:
  int64_t time_;
};

class Item
{
public:
  Item(const Module* source) : source_(source) { }

  void setCaption(const QString& caption) { caption_ = caption; }
  void setReduceText(const QString& text);
  void setReduceId(const QString& text);
  void setDescription(const QString& description) { description_ = description; }

  QString caption() const { return caption_; }
  QString description() const { return description_; }

private:
  const Module* source_;
  QString caption_;
  QString description_;
};

class RangeItem : public Item
{
public:
  RangeItem(const Module* source) : Item(source) { }

  void setRange(SystemTime start, SystemTime end);

  SystemTime start() const { return start_; }
  SystemTime end() const { return end_; }

private:
  SystemTime start_;
  SystemTime end_;
};

class SpotItem : public Item
{
public:
  SpotItem(const Module* source) : Item(source) { }

  void setTime(SystemTime time) { time_ = time; }
  SystemTime time() const { return time_; }

private:
  SystemTime time_;
};

class Items
{
public:
  Items();

  void clear();
  void merge(const Items& items);

  void addRange(const RangeItem& item);
  void addRange(RangeItem&& item);

  const std::vector<RangeItem>& ranges() const { return ranges_; }
  const std::vector<SpotItem>& spots() const { return spots_; }

private:
  std::vector<RangeItem> ranges_;
  std::vector<SpotItem> spots_;
};

} // namespace WinUptime

#endif // WINUPTIME_ITEMS_H
