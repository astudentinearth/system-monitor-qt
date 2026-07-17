
#include "MainWindow.hpp"
#include "system/Sensors.hpp"
#include "util.hpp"
#include <QString>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
  _layout = new QVBoxLayout(this);
  _btn = new QPushButton(this);
  _btn->setText("Reload");
  this->setWindowTitle("System Monitor");
  this->resize(300, 300);
  this->setMinimumSize(300, 300);

  _stats = new StatSource(this);

  _ramStats = new QLabel(this);
  _ramStats->setAlignment(Qt::AlignTop | Qt::AlignLeft);

  _table = new StatTableWidget(this);
  _table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  _layout->addWidget(this->_ramStats, 0, Qt::AlignTop | Qt::AlignLeft);
  _layout->addWidget(this->_btn, 0, Qt::AlignTop | Qt::AlignLeft);
  _layout->addWidget(this->_table, 1, Qt::AlignBottom);

  connect(_btn, &QPushButton::clicked, this, &MainWindow::querySensors);
  connect(_stats, &StatSource::ramStatsChanged, this,
          [this](RamStats stats) { _updateRamText(stats); });

  connect(_stats, &StatSource::detailedStatsChanged, _table,
          &StatTableWidget::render);

  querySensors();
}

void MainWindow::querySensors() {
  RamStats ramStats = getRamStats();
  auto detailedStats = getRamStats2();
  _stats->setRamStats(ramStats);
  _stats->setDetailedStats(detailedStats);
}

void MainWindow::_updateRamText(RamStats stats) {

  QString txt =
      QString("Total physical RAM: %1 bytes\nPage size: %2 bytes\nActive "
              "pages: %3 \nInactive pages: %4 \nFree pages: "
              "%5\n-----------\nUsed memory: %6MiB")
          .arg(stats.totalPhysicalRam)
          .arg(stats.pageSize)
          .arg(stats.activePages)
          .arg(stats.inactivePages)
          .arg(stats.freePages)
          .arg(bytesToMiB(stats.getUsedRam()));

  _ramStats->setText(txt);
}
