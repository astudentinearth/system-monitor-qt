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
    slot1->updateSubtitle(
        QString("%1 GiB free, %2 GiB swapped\n%3 / %4 GiB used")
            .arg(bytesToGiB(stats.freeBytes()), 0, 'f', 2)
            .arg(bytesToGiB(stats.swapBytes), 0, 'f', 2)
            .arg(bytesToGiB(stats.usedBytes), 0, 'f', 2)
            .arg(bytesToGiB(stats.totalBytes), 0, 'f', 2));
  }

  void cpuChanged(double user, double sys, double nice) {
    double active = user + sys + nice;
    slot2->updateDisplay(QString("%1%").arg(active, 0, 'f', 2));
    slot2->updateSubtitle(QString("%1% user, %2% system, %3% nice\n%4% idle")
                              .arg(user, 0, 'f', 2)
                              .arg(sys, 0, 'f', 2)
                              .arg(nice, 0, 'f', 2)
                              .arg(100 - active, 0, 'f', 2));
  }

public:
  explicit Sensors(QWidget *parent);

private:
  QGridLayout *layout;
  LEDView *slot1;
  LEDView *slot2;
};
