#pragma once
#ifndef __Controller_H__
#define __Controller_H__

#include "cocos2d.h"
#include "SaveData.h"

USING_NS_CC;

class Controller : public Layer
{
public:
	Controller();
	~Controller();
	virtual bool init();
	CREATE_FUNC(Controller);

	//对玩家分数的操作
	CC_SYNTHESIZE_READONLY(SaveData *, m_saveData, SaveData);
	void update(float tm);

	//游戏暂停与恢复
	void menuPauseCallback(cocos2d::Ref* pSender);

	//声音控制
	void menuMusicCallback(cocos2d::Ref* pSender);

public:
	MenuItemSprite* pPauseItem;
	MenuItemSprite* pMusicItem;
	Label * m_score;
	Label * score_label;
};

#endif