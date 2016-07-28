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

	//����ҷ����Ĳ���
	CC_SYNTHESIZE_READONLY(SaveData *, m_saveData, SaveData);
	void update(float tm);

	//��Ϸ��ͣ��ָ�
	void menuPauseCallback(cocos2d::Ref* pSender);

	//��������
	void menuMusicCallback(cocos2d::Ref* pSender);

public:
	MenuItemSprite* pPauseItem;
	MenuItemSprite* pMusicItem;
	Label * m_score;
	Label * score_label;
};

#endif