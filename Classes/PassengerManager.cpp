#include "PassengerManager.h"
#include "PassengerModel.h"
#include "GameConfig.h"

#define PASSENGER_X_MAX 350
#define PASSENGER_MODEL_SIZE Size(100, 120)
#define PASSENGER_X_GAP 30

const std::string PassengerManager::EventType_RemovePassenger = "removePassenger";

PassengerManager::PassengerManager():
m_modelList(NULL),
m_sceneEventDispatcher(NULL)
{
	m_passengerCount = 0;
}

PassengerManager::~PassengerManager(){
    for (std::list<PassengerModel*>::iterator it = m_modelList.begin(); it != m_modelList.end(); it++){
        delete *it;
    }
    m_modelList.clear();
}

void PassengerManager::adjustPassengerEndX(){
	int x = PASSENGER_X_MAX;

	for (std::list<PassengerModel*>::iterator it = m_modelList.begin(); it != m_modelList.end(); it++){
		PassengerModel* model = *it;
		model->setEndX(x);
		x -= model->getSize().width / 3 + PASSENGER_X_GAP;
	}
}

PassengerModel* PassengerManager::addPassenger(){
    PassengerConfig config;
    config.thinkNumber = random(GameConfig::getInstance()->numberRange.x, GameConfig::getInstance()->numberRange.y);
    config.thingSec = GameConfig::getInstance()->passengerThingSec;
    config.leaveSec = GameConfig::getInstance()->passengerLeaveSec;
    
    PassengerModel* passenger = PassengerModel::create(config, m_sceneEventDispatcher);
    
	m_modelList.push_back(passenger);
	m_passengerCount++;

	this->adjustPassengerEndX();

	return passenger;
}

bool PassengerManager::removePassenger(PassengerModel* model){
	m_modelList.remove(model);
	CC_SAFE_DELETE(model);
	m_passengerCount--;

	this->adjustPassengerEndX();

	return true;
}