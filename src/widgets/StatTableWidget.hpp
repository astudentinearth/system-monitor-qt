
#include "../system/Sensors.hpp"
#include <QHeaderView>
#include <QString>
#include <QTableWidget>
#include <vector>

class StatTableWidget : public QTableWidget {

public:
  explicit StatTableWidget(QWidget *parent);

public slots:
  void render(std::vector<Stat> stats) {
    setRowCount(stats.size());
    setColumnCount(2);

    for (int i = 0; i < stats.size(); i++) {
      Stat s = stats[i];
      setItem(i, 0, new QTableWidgetItem(QString::fromStdString(s.label)));
      setItem(i, 1,
              new QTableWidgetItem(QString("%1 %2").arg(s.value, 0, 'f', 2).arg(s.unit)));
    }
    resizeColumnsToContents();
    setAlternatingRowColors(true);
    setHorizontalHeaderLabels({"Stat", "Value"});
    this->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    this->horizontalHeader()->setSectionResizeMode(
        1, QHeaderView::ResizeToContents);
    this->verticalHeader()->hide();
  }
};
