
#include "MainWindow.hpp"
#include "system/Sensors.hpp"
#include <QString>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
  _layout = new QVBoxLayout(this);
  this->setWindowTitle("System Monitor");
  this->resize(300, 300);
  this->setMinimumSize(300, 300);

  _stats = new StatSource(this);

  _table = new StatTableWidget(this);
  _table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  _sensorsDisplay = new Sensors(this);
  _sensorsDisplay->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

  _layout->addWidget(_sensorsDisplay, 1, Qt::AlignTop);
  _layout->addWidget(this->_table, 1, Qt::AlignBottom);

  poll = new QTimer(this);


  connect(_stats, &StatSource::detailedStatsChanged, _table,
          &StatTableWidget::render);

  connect(_stats, &StatSource::ramStatsChanged, _sensorsDisplay, &Sensors::ramChanged);
  connect(_stats, &StatSource::cpuUtilizationChanged, _sensorsDisplay, &Sensors::cpuChanged);

  connect(poll, &QTimer::timeout, this, &MainWindow::querySensors);

  poll->start(pollInterval);
  m_startTick = getCpuStats();
  m_endTick = getCpuStats();
  querySensors();
}

void MainWindow::querySensors() {
  RamStats ramStats = getRamStats();
  auto detailedStats = getDetailedRamStats();
  auto cpu = getCpuStats();
  _stats->setRamStats(ramStats);
  _stats->setDetailedStats(detailedStats);
  m_startTick = m_endTick;
  m_endTick = cpu;
  _stats->setCpuUtilization(m_startTick.percentAgainst(m_endTick));
}


