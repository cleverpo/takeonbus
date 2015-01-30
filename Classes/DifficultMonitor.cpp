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
    m_config = GameConfig::getInstance();
}


DifficultMonitor::~DifficultMonitor(){
    m_config = nullptr;
}

void DifficultMonitor::destroy(){
    delete m_DifficultMonitorInstance;
    
    m_DifficultMonitorInstance = nullptr;
}

void DifficultMonitor::increateDifficult(){
    m_config->passengerMaxCount = MIN(11, m_config->passengerMaxCount + 1);
    m_config->passengerCreateRate = MAX(1, m_config->passengerCreateRate - 1);
    m_config->passengerLeaveSec = MAX(3, m_config->passengerLeaveSec - 1);
    m_config->passengerThingSec = MAX(3, m_config->passengerThingSec - 1);
    m_config->numberRange.y = MIN(10, m_config->numberRange.y + 1);
    m_config->carSpeed = MAX(5, m_config->carSpeed - 1);
    m_config->carCreateRate = MAX(1, m_config->carCreateRate - 1);
    m_config->carMatchRate = MAX(0.5, m_config->carMatchRate - 0.1);
}

void DifficultMonitor::setDifficult(int i){
    m_config->setConfig("json/difficult.json", i);
}

void DifficultMonitor::decreateDifficult(){

}
