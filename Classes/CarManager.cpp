#include "CarManager.h"
#include "CarModel.h"
#include "PassengerManager.h"
#include "PassengerModel.h"
#include "GameConfig.h"
#include <math.h>

const std::string CarManager::EventType_RemoveCar = "RemoveCar";

CarManager::CarManager():
m_passengerManager(NULL),
m_sceneEventDispatcher(NULL)
{

}

CarManager::~CarManager(){
    for (std::map<int, CarModel*>::const_iterator it = m_carMap.begin(); it != m_carMap.end(); it++){
        delete (*it).second;
    }
    m_carMap.clear();
}

int CarManager::generateNumber(){
    float matchThinkRate = GameConfig::getInstance()->carMatchRate;
    float randomNum = random<float>(0, 1.0);
    
    if (randomNum <= matchThinkRate){
        //match passenger think
        const std::list<PassengerModel*>& passengerList = m_passengerManager->getModelList();
        for(std::list<PassengerModel*>::const_iterator it = passengerList.begin(); it != passengerList.end(); it++){
            int thinkNum = (*it)->getThinkNum();
            if(m_carMap.find(thinkNum) == m_carMap.end()){
                //找不到现有的车在走
                return thinkNum;
            }
        }
    }
    
    return random(GameConfig::getInstance()->numberRange.x, GameConfig::getInstance()->numberRange.y);
}

CarModel* CarManager::addCar(){
    int num = this->generateNumber();

    CarConfig config;
    config.carType  = CarType(random<int>(0, 2));
    config.number   = num;
    config.speedSec = GameConfig::getInstance()->carSpeed;
    
	CarModel* car = CarModel::create(config, m_sceneEventDispatcher);
	m_carMap.insert(std::pair<int, CarModel*>(num, car));

	return car;
}

bool CarManager::removeCar(CarModel* car){
	for (std::map<int, CarModel*>::const_iterator it = m_carMap.begin(); it != m_carMap.end(); it++){
		if ((*it).second == car){
			m_carMap.erase(it);
			CC_SAFE_DELETE(car);
			return true;
		}
	}

	return false;
}