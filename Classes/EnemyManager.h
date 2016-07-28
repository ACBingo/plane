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
	//�󶨿�����(���·������л����ƶ�)
	void bindController(Controller* controller);

	//���ݷ���������ӵл��ٶ�
	//void addSpeed(float dt);

	// ��ӵл�
	void addEnemy(float dt);

	// �л���ը
	void blowupEnemy(EnemyBase* pEnemySprite);

	// �Ƴ��л�pNode
	void removeEnemy(Node *pNode);

	//��ը����
	Animate* boomAnimate();
	
	//�˶��켣������
	Sequence* EnemyManager::MovingTrack();

	void removeAct(Node * pSender);

	void checkCanLevel(int score);
public:
	//Vector<Sprite*> vecItem;//������Ʒ
	Vector<EnemyBase*> vecEnemy;// �л����������ڱ�����ײ����
	Controller* m_controlLayer;	//������
	float m_fCreatSpeed=1;	//��ӵл��ٶ�
	float m_fEnemy;
};



#endif