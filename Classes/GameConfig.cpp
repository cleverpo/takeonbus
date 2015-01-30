//
//  GameConfig.cpp
//  takeonbus
//
//  Created by luozhipeng on 15/1/29.
//
//

#include "GameConfig.h"
#include "json/document.h"
#include "cocostudio/DictionaryHelper.h"
#include "util.h"

using namespace cocos2d;
using namespace cocostudio;

static GameConfig* m_gameConfigInstance = NULL;

GameConfig* GameConfig::getInstance(){
    if(!m_gameConfigInstance)
        m_gameConfigInstance = new GameConfig();
    
    return m_gameConfigInstance;
};

void GameConfig::setConfig(std::string filename, int difficult){
    std::string fullPath    = FileUtils::getInstance()->fullPathForFilename(filename);
    std::string contentStr  = FileUtils::getInstance()->getStringFromFile(fullPath);
    
    rapidjson::Document doc;
    doc.Parse<0>(contentStr.c_str());
    if (doc.HasParseError())
    {
        CCLOG("GetParseError %s\n", doc.GetParseError());
    }
    
    const rapidjson::Value& json = DictionaryHelper::getInstance()->getSubDictionary_json(doc, convertToString<int>(difficult).c_str());
    this->passengerLeaveSec = DictionaryHelper::getInstance()->getIntValue_json(json, "stayTime");
    this->carCreateRate = DictionaryHelper::getInstance()->getIntValue_json(json, "carCreaetRate");
    this->carSpeed = DictionaryHelper::getInstance()->getIntValue_json(json, "carSpeed");
    this->numberRange.y = DictionaryHelper::getInstance()->getIntValue_json(json, "maxNumber");
}

