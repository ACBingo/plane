#ifndef __MainScene_SCENE_H__
#define __MainScene_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
class MainScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	void myupdate(float f);
	virtual bool init();
	CREATE_FUNC(MainScene);

	void menuCallback(cocos2d::Ref* pSender);//菜单回调函数
};

#endif 