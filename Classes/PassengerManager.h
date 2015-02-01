#ifndef PASSENGER_MANAGER_H
#define PASSENGER_MANAGER_H

#include "cocos2d.h"
#include <list>

class CarManager;
class PassengerModel;
class PassengerManager {
    
public:
    static const std::string EventType_RemovePassenger;
    static const std::string EventType_AddPassenger;
    static const std::string EventType_ShowResult;

public:
	PassengerManager();
	~PassengerManager();

public:
	PassengerModel* addPassenger();
	bool removePassenger(PassengerModel* model);
    
    void update(float dt);

public:
	void setPassengerCount(int count){ m_passengerCount = count; }
	int getPassengerCount(){ return m_passengerCount; }
    
    const std::list<PassengerModel*>& getModelList(){ return m_modelList; }

public:
    void setCarManager(CarManager* manager){ m_carManager = manager; }
    void setSceneEventDispatcher(cocos2d::EventDispatcher* dispatcher){ m_sceneEventDispatcher = dispatcher; }
    
private:
	void adjustPassengerEndX();
    
private:
    int m_passengerCount1;
    int m_passengerCount;
    float m_passengerCreateTime;
    //汽车乘客出现的秒数
    float m_passengerCreateSec;
    float m_totalTime;

private:
    CarManager* m_carManager;
	std::list<PassengerModel*> m_modelList;
    cocos2d::EventDispatcher* m_sceneEventDispatcher;
};

#endif
