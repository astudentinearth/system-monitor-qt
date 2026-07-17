#pragma once
#include "../system/Sensors.hpp"
#include "../util.hpp"
#include <QGridLayout>
#include <QLabel>
#include <QWidget>

class LEDView : public QWidget {
  Q_OBJECT

public:
  explicit LEDView(QWidget *parent, QString title);

public slots:
  void updateDisplay(QString _display) { display->setText(_display); }

  void updateSubtitle(QString _title) { subtitle->setText(_title); }

private:
  QVBoxLayout *layout;
  QLabel *title;
  QLabel *display;
  QLabel *subtitle;
};

class Sensors : public QWidget {
  Q_OBJECT

public slots:
  void ramChanged(RamStats stats) {
    slot1->updateDisplay(QString("%1%").arg(stats.usedPercent(), 0, 'f', 2));
    slot1->updateSubtitle(QString("%1 GiB free, %2 GiB swapped\n%3 / %4 GiB used")
                              .arg(bytesToGiB(stats.freeBytes()), 0, 'f', 2)
                              .arg(bytesToGiB(stats.swapBytes), 0, 'f', 2)
                              .arg(bytesToGiB(stats.usedBytes), 0, 'f', 2)
                              .arg(bytesToGiB(stats.totalBytes), 0, 'f', 2));
  }

public:
  explicit Sensors(QWidget *parent);

private:
  QGridLayout *layout;
  LEDView *slot1;
  LEDView *slot2;
};
