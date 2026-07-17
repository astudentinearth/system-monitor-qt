
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
#include "widgets/SensorsView.hpp"
#include <QTimer>

class MainWindow : public QWidget {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = nullptr);

    private:
        int _count = 0;
        int pollInterval = 1000;
        QLabel* _counter;
        StatSource* _stats;
        QVBoxLayout* _layout;
        StatTableWidget* _table;
        Sensors *_sensorsDisplay;
        QTimer* poll;
        void querySensors();
        CpuTickStats m_startTick;
        CpuTickStats m_endTick;
};

