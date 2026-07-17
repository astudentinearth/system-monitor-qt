
#pragma once
#include <QObject>
#include <vector>
#include "system/Sensors.hpp"

class StatSource : public QObject {
    Q_OBJECT

    public:
        explicit StatSource(QObject *parent = nullptr);

    signals:
        void ramStatsChanged(RamStats stats);
        void detailedStatsChanged(std::vector<Stat> stats);

    public slots:
        void setRamStats(RamStats stats) {
            _ramStats = stats;
            emit(ramStatsChanged(_ramStats));
        }

        void setDetailedStats(std::vector<Stat> stats) {
            _stats = stats;
            emit(detailedStatsChanged(stats));
        }

    private:
        RamStats _ramStats;
        std::vector<Stat> _stats;
};

