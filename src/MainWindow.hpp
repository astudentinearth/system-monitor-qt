
#pragma once
#include <QApplication> 
#include <QWindow> 
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include "StatSource.hpp"

class MainWindow : public QWidget {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = nullptr);

    signals:
        void countChanged(int newCount);

    public slots:
        void increment() {
            _count++;
            emit countChanged(_count);
        }

    private:
        int _count = 0;
        QLabel* _counter;
        QLabel* _ramStats;
        StatSource* _stats;
        QVBoxLayout* _layout;
        QPushButton* _btn;
        void querySensors();
        void _updateRamText(RamStats stats);
};

