#include "CarModel.h"
#include "CarManager.h"
#include "cocostudio/CocoStudio.h"
#include "ui/UIText.h"
#include "util.h"

#define CAR_POSITION Vec2(-50, 315)
#define CAR_SIZE Size(280, 180)

using namespace ui;
using namespace cocostudio;

CarModel::CarModel():
m_node(NULL)
{

}

CarModel::~CarModel(){
	CC_SAFE_RELEASE(m_node);
}

CarModel* CarModel::create(const CarConfig& config, std::function<void(CarModel*)> removeFunc){
	CarModel* ret = new CarModel();
	if (ret) {
		ret->init(config, removeFunc);

		return ret;
	}

	return NULL;
}

void CarModel::initNode(CarType type){
    Node* car = CSLoader::createNode("car/bus.csb");
    timeline::ActionTimeline* timeLine = CSLoader::createTimeline("car/bus.csb");
    
    std::string fileName = "";
    
    if(type == CarType::Yellow){
        fileName = "car/bus01.png";
    }else if(type == CarType::Red){
        fileName = "car/bus02.png";
    }else if(type == CarType::Blue){
        fileName = "car/bus03.png";
    }else{
        CCASSERT(false, "wrong car type");
    }
    
    Sprite* carBody = static_cast<Sprite*>(car->getChildByName("bus01"));
    carBody->setTexture(fileName);
    
    timeLine->gotoFrameAndPlay(0, 30, true);
    car->runAction(timeLine);
    car->setAnchorPoint(Vec2(0.5, 0));
    car->setPosition(CAR_POSITION);
    car->setContentSize(CAR_SIZE);
    
    Sprite* notice = static_cast<Sprite*>(car->getChildByName("notice"));
    Text* label = static_cast<Text*>(notice->getChildByName("label"));
    
    GLProgram* program = GLProgramCache::getInstance()->getGLProgram("colorChangeShader");
    carBody->setGLProgram(program);
    carBody->getGLProgramState()->setUniformVec3("u_originColor", Vec3(0.968, 0.713, 0.0));
    carBody->getGLProgramState()->setUniformVec3("u_targetColor", Vec3(0.0, 0.0, 0.0));
    
    label->setString(convertToString(this->m_number));
    
    this->m_node = car;
    this->m_node->retain();
}

void CarModel::init(const CarConfig& config, std::function<void(CarModel*)>& removeFunc){
    this->m_type     = config.carType;
    this->m_speedSec = config.speedSec;
    this->m_number   = config.number;
    
    this->initNode(config.carType);
    
    //speed
    if(config.carType == CarType::Blue){
        //blue 快1秒
        this->m_speedSec -= 1;
    }else if(config.carType == CarType::Red){
        //red 快2秒
        this->m_speedSec -= 2;
    }
    //action
    this->m_node->runAction(Sequence::createWithTwoActions(MoveBy::create(this->m_speedSec, Vec2(1400, 0)), CallFunc::create([=](){
        removeFunc(this);
    })));
}