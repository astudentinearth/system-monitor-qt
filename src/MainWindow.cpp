
#include "MainWindow.hpp"
#include "system/Sensors.hpp"
#include <QString>

double bytesToMiB(uint64_t bytes) {
    return ((double)bytes / 1024) / 1024;
}

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
  _layout = new QVBoxLayout(this);
  _btn = new QPushButton(this);
  _btn->setText("Reload");
  this->setWindowTitle("System Monitor");
  this->resize(100, 100);
  this->setMinimumSize(100, 100);

  _stats = new StatSource(this);

  _ramStats = new QLabel(this);
  _ramStats->setAlignment(Qt::AlignTop | Qt::AlignLeft);

  _layout->addWidget(this->_ramStats, 0, Qt::AlignTop | Qt::AlignLeft);
  _layout->addWidget(this->_btn, 0, Qt::AlignTop | Qt::AlignLeft);

  connect(_btn, &QPushButton::clicked, this, &MainWindow::querySensors);
  connect(_stats, &StatSource::ramStatsChanged, this,
          [this](RamStats stats) { _updateRamText(stats); });

  querySensors();
}

void MainWindow::querySensors() {
  RamStats ramStats = getRamStats();
  _stats->setRamStats(ramStats);
}

void MainWindow::_updateRamText(RamStats stats) {

  QString txt = QString("Total physical RAM: %1 bytes\nPage size: %2 bytes\nActive "
                        "pages: %3 \nInactive pages: %4 \nFree pages: %5\n-----------\nUsed memory: %6MiB")
                    .arg(stats.totalPhysicalRam)
                    .arg(stats.pageSize)
                    .arg(stats.activePages)
                    .arg(stats.inactivePages)
                    .arg(stats.freePages)
                    .arg(bytesToMiB(stats.getUsedRam()));

  _ramStats->setText(txt);
}
