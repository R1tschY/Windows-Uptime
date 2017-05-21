#ifndef WINUPTIME_ITEMS_H
#define WINUPTIME_ITEMS_H

#include <windows.h>

#include <vector>
#include <QDateTime>
#include <QString>
#include <QTime>
#include <QColor>
#include <QIcon>

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

  QString caption() const { return caption_; }
  void setCaption(const QString& caption) { caption_ = caption; }

  QString description() const { return description_; }
  void setDescription(const QString& description) { description_ = description; }

  QColor color() const { return color_; }
  void setColor(QColor color) { color_ = color; }

  const Module* sourceModule() const { return source_; }

  QString type() const { return type_; }
  void setType(const QString& type) { type_ = type; }

private:
  const Module* source_;
  QString type_;
  QString caption_;
  QString description_;
  QColor color_;
};

class RangeItem : public Item
{
public:
  RangeItem(const Module* source) : Item(source) { }

  void setRange(const QDateTime& start, const QDateTime& end);

  QDateTime start() const { return start_; }
  QDateTime end() const { return end_; }

private:
  QDateTime start_;
  QDateTime end_;
};

class SpotItem : public Item
{
public:
  SpotItem(const Module* source) : Item(source) { }

  void setTime(const QDateTime& time) { time_ = time; }
  const QDateTime& time() const { return time_; }

  QIcon icon() const { return icon_; }
  void setIcon(const QIcon& icon) { icon_ = icon; }

private:
  QDateTime time_;
  QIcon icon_;
};

class Items
{
public:
  Items();

  void clear();
  void merge(const Items& items);

  void addRange(const RangeItem& item);
  void addRange(RangeItem&& item);

  void addSpot(const SpotItem& item);
  void addSpot(SpotItem&& item);

  const std::vector<RangeItem>& ranges() const { return ranges_; }
  const std::vector<SpotItem>& spots() const { return spots_; }

private:
  std::vector<RangeItem> ranges_;
  std::vector<SpotItem> spots_;
};

} // namespace WinUptime

#endif // WINUPTIME_ITEMS_H
