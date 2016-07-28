#pragma once
#ifndef __Bullet_H__
#define __Bullet_H__

#include "cocos2d.h"

#include "Plane.h"
#include "EnemyManager.h"
#include "SimpleAudioEngine.h"

USING_NS_CC; // 命名空间

class Bullet : public Layer
{
public:
	Bullet();
	~Bullet();
	virtual bool init();
	CREATE_FUNC(Bullet);

public:
	void bindEnemyManager(EnemyManager* enemyManager);//分数更新
	void BeginBulletShoot(float dt = 0.0f);	// 开启子弹射击
	void StopBulletShoot();					// 停止子弹射击
	void addBullet(float dt);				// 添加子弹
	void removeBullet(Node* pNode);			// 移除子弹
	void addEnemyBullet(float dt);				// 添加敌机子弹
	void removeEnemyBullet(Node* pNode);			// 移除敌机子弹
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
	int damage;//攻击力
	static Bullet* instanceBullet;	//子弹实体
};

#endif
