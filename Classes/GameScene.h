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
	int status;         //��Ϸ״̬  1Ϊ������2Ϊ��ͣ��3Ϊ����
	int score;
public:
	GameScene();
	~GameScene();

	bool init();
	CREATE_FUNC(GameScene);

	static Scene * createScene();
	static GameScene * instanceGameScene;//gamesceneʵ��
	void moveBackground(float t);//�����ƶ�

	void setBackGround();
	void levelUp();//�ؿ�����
	int getLevel();

	void gameUpdate(float dt);     // ��ײ���
	bool bulletCollisionEnemy(Sprite* pBullet);   // �ӵ��͵л���ײ
	void enemyCollisionPlane();    // �һ��͵л����л��ӵ���ײ
	bool ItemCollisionPlane(Sprite* pItem);//������ײ
	void toGameOver(float dt);
	

	MyPlane *planeLayer;
	Bullet *bulletLayer;
	EnemyManager *enemyLayer;
	Controller *controlLayer;
	ItemDrop *itemLayer;

	//int clearBoom;//��������
	void clearAllEnemy();//������е��˺��ӵ�
	void removeBoom(Node * pSender);
	void creatClearBoom();//
	void setBoomNum();//����
	void addBoomNum(int t);
	void setBoomPic();
	void particleAct();
	int boomNum;//
	void boomAction();
};
#endif