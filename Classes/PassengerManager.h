#ifndef PASSENGER_MANAGER_H
#define PASSENGER_MANAGER_H

#include <list>

class PassengerModel;
class PassengerManager {
    
public:
    static const std::string EventType_RemovePassenger;

public:
	PassengerManager();
	~PassengerManager();

public:
	PassengerModel* addPassenger();
	bool removePassenger(PassengerModel* model);

public:
	void setPassengerCount(int count){ m_passengerCount = count; }
	int getPassengerCount(){ return m_passengerCount; }
    
    const std::list<PassengerModel*>& getModelList(){ return m_modelList; }

public:
    void setSceneEventDispatcher(cocos2d::EventDispatcher* dispatcher){ m_sceneEventDispatcher = dispatcher; }
    
private:
	void adjustPassengerEndX();

private:
	int m_passengerCount;
	std::list<PassengerModel*> m_modelList;
    cocos2d::EventDispatcher* m_sceneEventDispatcher;
};

#endif