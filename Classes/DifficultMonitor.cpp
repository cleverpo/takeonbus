//
//  DifficultMonitor.cpp
//  takeonbus
//
//  Created by luozhipeng on 15/1/28.
//
//

#include "DifficultMonitor.h"

static DifficultMonitor* m_DifficultMonitorInstance = nullptr;

DifficultMonitor* DifficultMonitor::getInstance(){
    if(m_DifficultMonitorInstance == nullptr)
        m_DifficultMonitorInstance = new DifficultMonitor();
    
    return m_DifficultMonitorInstance;
}


DifficultMonitor::DifficultMonitor(){
    m_gameConfigInstance = GameConfig::getInstance();
}


DifficultMonitor::~DifficultMonitor(){
    m_gameConfigInstance = nullptr;
}

void DifficultMonitor::destroy(){
    delete m_DifficultMonitorInstance;
    
    m_DifficultMonitorInstance = nullptr;
}

void DifficultMonitor::increateDifficult(){
    m_gameConfigInstance->passengerMaxCount = MIN(11, m_gameConfigInstance->passengerMaxCount + 1);
    m_gameConfigInstance->passengerCreateRate = MAX(1, m_gameConfigInstance->passengerCreateRate - 1);
    m_gameConfigInstance->passengerLeaveSec = MAX(3, m_gameConfigInstance->passengerLeaveSec - 1);
    m_gameConfigInstance->passengerThingSec = MAX(3, m_gameConfigInstance->passengerThingSec - 1);
    m_gameConfigInstance->numberRange.y = MIN(10, m_gameConfigInstance->numberRange.y + 1);
    m_gameConfigInstance->carSpeed = MAX(5, m_gameConfigInstance->carSpeed - 1);
    m_gameConfigInstance->carCreateRate = MAX(1, m_gameConfigInstance->carCreateRate - 1);
    m_gameConfigInstance->carMatchRate = MAX(0.5, m_gameConfigInstance->carMatchRate - 0.1);
}

void DifficultMonitor::decreateDifficult(){

}
