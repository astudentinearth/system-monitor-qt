
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
        void cpuUtilizationChanged(double percent);
        void cpuTick(double user, double system, double nice);

    public slots:
        void setRamStats(RamStats stats) {
            _ramStats = stats;
            emit(ramStatsChanged(_ramStats));
        }

        void setDetailedStats(std::vector<Stat> stats) {
            _stats = stats;
            auto cpu = m_startTick.statAgainst(m_endTick);
            _stats.insert(_stats.end(), cpu.begin(), cpu.end());
            emit(detailedStatsChanged(_stats));
        }

        void setCpuUtilization(double percent) {
            m_cpuPercent = percent;
            emit(cpuUtilizationChanged(m_cpuPercent));
        }

        void pushCpuTick(CpuTickStats tick) {
            m_startTick = m_endTick;
            m_endTick = tick;
            emit(cpuTick(
                    m_startTick.userPercent(m_endTick),
                    m_startTick.sysPercent(m_endTick),
                    m_startTick.nicePercent(m_endTick)
                        ));
        }

    private:
        RamStats _ramStats;
        double m_cpuPercent;
        std::vector<Stat> _stats;
        CpuTickStats m_startTick;
        CpuTickStats m_endTick;
};

