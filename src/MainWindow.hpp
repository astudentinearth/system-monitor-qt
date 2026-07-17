
#pragma once
#include <QApplication> 
#include <QWindow> 
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include "StatSource.hpp"
#include "widgets/StatTableWidget.hpp"

class MainWindow : public QWidget {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = nullptr);

    private:
        int _count = 0;
        QLabel* _counter;
        QLabel* _ramStats;
        StatSource* _stats;
        QVBoxLayout* _layout;
        StatTableWidget* _table;
        QPushButton* _btn;
        void querySensors();
        void _updateRamText(RamStats stats);
};

