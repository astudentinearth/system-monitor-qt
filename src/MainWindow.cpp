
#include "MainWindow.hpp"
#include "system/Sensors.hpp"
#include <QString>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
  _layout = new QVBoxLayout(this);
  this->setWindowTitle("System Monitor");
  this->resize(600, 350);
  this->setMinimumSize(300, 300);
  _layout->setContentsMargins(0, 0, 0, 0);

  _stats = new StatSource(this);

  _table = new StatTableWidget(this);
  _table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  _sensorsDisplay = new Sensors(this);
  _sensorsDisplay->setSizePolicy(QSizePolicy::Expanding,
                                 QSizePolicy::Fixed);

  _layout->addWidget(_sensorsDisplay, 0, Qt::AlignTop);
  _layout->addWidget(this->_table, 1);

  poll = new QTimer(this);

  connect(_stats, &StatSource::detailedStatsChanged, _table,
          &StatTableWidget::render);

  connect(_stats, &StatSource::ramStatsChanged, _sensorsDisplay,
          &Sensors::ramChanged);
  connect(_stats, &StatSource::cpuTick, _sensorsDisplay, &Sensors::cpuChanged);

  connect(poll, &QTimer::timeout, this, &MainWindow::querySensors);

  poll->start(pollInterval);
  querySensors();
}

void MainWindow::querySensors() {
  RamStats ramStats = getRamStats();
  auto detailedStats = getDetailedRamStats();
  auto cpu = getCpuStats();
  _stats->setRamStats(ramStats);
  _stats->setDetailedStats(detailedStats);
  _stats->pushCpuTick(cpu);
}
