#pragma once
#ifndef __Bullet_H__
#define __Bullet_H__

#include "cocos2d.h"

#include "Plane.h"
#include "EnemyManager.h"
#include "SimpleAudioEngine.h"

USING_NS_CC; // �����ռ�

class Bullet : public Layer
{
public:
	Bullet();
	~Bullet();
	virtual bool init();
	CREATE_FUNC(Bullet);

public:
	void bindEnemyManager(EnemyManager* enemyManager);//��������
	void BeginBulletShoot(float dt = 0.0f);	// �����ӵ����
	void StopBulletShoot();					// ֹͣ�ӵ����
	void addBullet(float dt);				// ����ӵ�
	void removeBullet(Node* pNode);			// �Ƴ��ӵ�
	void addEnemyBullet(float dt);				// ��ӵл��ӵ�
	void removeEnemyBullet(Node* pNode);			// �Ƴ��л��ӵ�
	void setDamage(int t);
	void addDamage(int t);
	int getDamage();
	Sprite * setBulletType(); 
	void setBulletPosition(Sprite* bullet, int t);
	void addOneBullet(int ix);
	void setAutoBullet();
	void addKeyBullet();
	bool autoBullet;
public:
	Vector<Sprite*> vecBullet;
	Vector<Sprite*> vecEnemyBullet;
	EnemyManager* m_enemyManager;
	int damage;//������
	static Bullet* instanceBullet;	//�ӵ�ʵ��
};

#endif
