#pragma once
#ifndef __EnemyManager_H__
#define __EnemyManager_H__

#include "cocos2d.h"
#include "EnemyBase.h"
#include "Controller.h"

USING_NS_CC;

class EnemyManager : public Layer
{
public:
	EnemyManager();
	~EnemyManager();
	virtual bool init();
	CREATE_FUNC(EnemyManager);
public:
	//绑定控制器(更新分数，敌机的移动)
	void bindController(Controller* controller);

	//根据分数决定添加敌机速度
	//void addSpeed(float dt);

	// 添加敌机
	void addEnemy(float dt);

	// 敌机爆炸
	void blowupEnemy(EnemyBase* pEnemySprite);

	// 移除敌机pNode
	void removeEnemy(Node *pNode);

	//爆炸动画
	Animate* boomAnimate();
	
	//运动轨迹产生器
	Sequence* EnemyManager::MovingTrack();

	void removeAct(Node * pSender);

	void checkCanLevel(int score);
public:
	//Vector<Sprite*> vecItem;//掉落物品
	Vector<EnemyBase*> vecEnemy;// 敌机容器，用于遍历碰撞问题
	Controller* m_controlLayer;	//控制器
	float m_fCreatSpeed=1;	//添加敌机速度
	float m_fEnemy;
};



#endif