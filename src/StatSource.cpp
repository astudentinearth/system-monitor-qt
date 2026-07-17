#include "StatSource.hpp"
#include "system/Sensors.hpp"

StatSource::StatSource(QObject *parent) : QObject(parent) {
    m_startTick = getCpuStats();
    m_endTick = getCpuStats();
}

