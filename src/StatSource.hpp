
#pragma once
#include <QObject>
#include "system/Sensors.hpp"

class StatSource : public QObject {
    Q_OBJECT

    public:
        explicit StatSource(QObject *parent = nullptr);

    signals:
        void ramStatsChanged(RamStats stats);

    public slots:
        void setRamStats(RamStats stats) {
            _ramStats = stats;
            emit(ramStatsChanged(_ramStats));
        }

    private:
        RamStats _ramStats;
};

