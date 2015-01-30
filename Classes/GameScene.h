#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "ui/UIText.h"

class PassengerModel;
class PassengerManager;
class CarModel;
class CarManager;
class ResultLayer;

class GameLayer : public cocos2d::Layer
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* scene();

	// implement the "static node()" method manually
	CREATE_FUNC(GameLayer);

protected:
	GameLayer();
	~GameLayer();
    
    void initSchedule();
    void initEventDispatcher();
    
    void initRoad();
    void initHUDLayer();
    
protected:
    void passengerLeave(PassengerModel* model);
	void removePassengerFromQueue(PassengerModel* model, bool isRemove);
	bool addPassengerToQueue(PassengerModel* passenger);

protected:
	bool addCar(CarModel* car);
	void removeCar(CarModel* car);

protected:
	void gameTimer(float dt);
    void stopTimer();
    void startTimer();

protected:
    void changeScore(int score, bool add);
    void changeMissCount(int changeNum);
    
private:
    //Layer
    cocos2d::Layer* m_carLayer;
    cocos2d::Layer* m_hudLayer;
    ResultLayer* m_resultLayer;
    
private:
    //HUD node
    cocos2d::ui::Text* m_scoreLabel;
    cocos2d::ui::Text* m_missLabel;
    
private:
	PassengerManager* m_passengerManager;
	CarManager* m_carManager;
    
    bool* m_carSymbol;
    
private:
    //走掉的乘客数量
    int m_missCount;
    //结果
    int m_score;
    
private:
    //总时间秒数
    float m_totolTime;
    
private:
    //事件接收器
    cocos2d::EventDispatcher* m_customEventDispatcher;
    
};

class ResultLayer : public cocos2d::Layer{
    
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static node()" method manually
    CREATE_FUNC(ResultLayer);
    
protected:
    ResultLayer();
    ~ResultLayer();
    
public:
    void showString(std::string text);
    void showScore(int score, bool isIncrease);
    void showResult();
};

#endif // __HELLOWORLD_SCENE_H__
