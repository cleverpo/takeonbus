//
//  DifficultMonitor.h
//  takeonbus
//
//  Created by luozhipeng on 15/1/28.
//
//

#ifndef takeonbus_DifficultMonitor_h
#define takeonbus_DifficultMonitor_h

#include "GameConfig.h"

class DifficultMonitor{
public:
    static DifficultMonitor* getInstance();

public:
    void destroy();
    
    void setDifficult(int difficult);
    void increateDifficult();
    void decreateDifficult();
    
protected:
    DifficultMonitor();
    ~DifficultMonitor();

private:
    GameConfig* m_config;
};

#endif
