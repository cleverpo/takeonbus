#ifndef CAR_MANAGER_H
#define CAR_MANAGER_H

#include <map>

class PassengerManager;
class CarModel;
class CarManager{
public:
    static const std::string EventType_RemoveCar;
    
public:
	CarManager();
	~CarManager();

public:
    int generateNumber();
    
    CarModel* addCar();
	bool removeCar(CarModel* car);
    
    const std::multimap<int, CarModel*>& getCarMap(){ return m_carMap; }

public:
    void setPassengerManager(PassengerManager* manager){ m_passengerManager = manager; }
    void setSceneEventDispatcher(cocos2d::EventDispatcher* dispatcher){ m_sceneEventDispatcher = dispatcher; }
    
private:
    PassengerManager* m_passengerManager;
	std::multimap<int, CarModel*> m_carMap;
    cocos2d::EventDispatcher* m_sceneEventDispatcher;
};

#endif