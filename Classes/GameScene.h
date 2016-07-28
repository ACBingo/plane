#pragma once
#ifndef GAME_SCENE
#define GAME_SCENE
#include "cocos2d.h"
#include "MainScene.h"
#include "Plane.h"
#include "EnemyManager.h"
#include "Bullet.h"
#include "ItemDrop.h"
#include "Controller.h"

using namespace cocos2d;
class GameScene :Layer {
private:
	int level;
	int status;         //游戏状态  1为正常、2为暂停、3为结束
	int score;
public:
	GameScene();
	~GameScene();

	bool init();
	CREATE_FUNC(GameScene);

	static Scene * createScene();
	static GameScene * instanceGameScene;//gamescene实例
	void moveBackground(float t);//背景移动

	void setBackGround();
	void levelUp();//关卡升级
	int getLevel();

	void gameUpdate(float dt);     // 碰撞检测
	bool bulletCollisionEnemy(Sprite* pBullet);   // 子弹和敌机碰撞
	void enemyCollisionPlane();    // 我机和敌机、敌机子弹碰撞
	bool ItemCollisionPlane(Sprite* pItem);//物体碰撞
	void toGameOver(float dt);
	

	MyPlane *planeLayer;
	Bullet *bulletLayer;
	EnemyManager *enemyLayer;
	Controller *controlLayer;
	ItemDrop *itemLayer;

	//int clearBoom;//清屏导弹
	void clearAllEnemy();//清除所有敌人和子弹
	void removeBoom(Node * pSender);
	void creatClearBoom();//
	void setBoomNum();//导弹
	void addBoomNum(int t);
	void setBoomPic();
	void particleAct();
	int boomNum;//
	void boomAction();
};
#endif