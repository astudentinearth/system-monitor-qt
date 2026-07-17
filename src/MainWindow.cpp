
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

  connect(poll, &QTimer::timeout, this, &MainWindow::querySensors);

  poll->start(pollInterval);
  querySensors();
}

void MainWindow::querySensors() {
  RamStats ramStats = getRamStats();
  auto detailedStats = getDetailedRamStats();
  _stats->setRamStats(ramStats);
  _stats->setDetailedStats(detailedStats);
}


