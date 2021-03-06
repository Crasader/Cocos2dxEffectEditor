//
//  AnimationConfig.hpp
//  EffectEditor
//
//  Created by Matrix on 16/8/8.
//
//

#ifndef AnimationConfig_hpp
#define AnimationConfig_hpp

#include <stdio.h>
#include <string>
#include <cocos2d.h>
#include "GlobalDefine.hpp"


NS_EE_BEGIN

class AnimationConfig
{
public:
    
    AnimationConfig();
    
    std::string id;
    std::string frameFile;
    std::string texture;
    std::string frameName;
    int frameFrom;
    int frameTo;
    cocos2d::Vec3 pos;
    float rotation;
    cocos2d::Vec2 scale;
    float interval;
    int repeat;
    float delay;
    std::string timeline;
    bool visible;
};

NS_EE_END

#endif /* AnimationConfig_hpp */
