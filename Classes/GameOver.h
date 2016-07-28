
#ifndef __GameOver_H__
#define __GameOver_H__

#include "cocos2d.h"
#include "MainScene.h"

USING_NS_CC;

class GameOver : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// �˳���Ϸ
	void menuCloseCallback(cocos2d::Ref* pSender);

	// ����������
	void menuMainCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(GameOver);

public:

};

#endif