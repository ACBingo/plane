#pragma once
#ifndef __EnemyBase_H__
#define __EnemyBase_H__

#define HardCode 1

#include "cocos2d.h"

USING_NS_CC;


// �л�����ֵ
const int ENEMY1_1 = 1 * HardCode;
const int ENEMY1_2 = 2 * HardCode;
const int ENEMY1_3 = 2 * HardCode;
const int ENEMY2_1 = 8 * HardCode;
const int ENEMY2_2 = 10 * HardCode;
const int ENEMY2_3 = 12 * HardCode;
const int ENEMY3_1 = 8*3 * HardCode;
const int ENEMY3_2 = 9*3 * HardCode;
const int ENEMY3_3 = 10*3 * HardCode;
const int ENEMY4_1 = 16*4 * HardCode;
const int ENEMY4_2 = 17*4 * HardCode;
const int ENEMY4_3 = 18*4 * HardCode;
const int ENEMY5_1 = 32*5 * HardCode;
const int ENEMY5_2 = 35*5 * HardCode;
const int ENEMY5_3 = 40*5 * HardCode;
const int ENEMYBOSS1 = 25 * HardCode;
const int ENEMYBOSS2 = 50 * HardCode;
const int ENEMYBOSS3 = 100 * HardCode;
const int ENEMYBOSS4 = 200 * HardCode;
const int ENEMYBOSS5 = 400 * HardCode;
const int ENEMYBOSS6 = 800 * HardCode;

class EnemyBase : public Node
{
public:
	EnemyBase();
	~EnemyBase();
	virtual bool init();
	CREATE_FUNC(EnemyBase);

public:
	std::map<std::string, int> enemyHP;//�ɻ���HP(������ͷ�ļ��ж���ȫ�ֱ����������������ض���)
	void setEnemyByType(std::string enType);//�ɻ�����
	Sprite* getSprite();//��ø�ʵ��

	int getScore();
	int getLife();
	bool loseLife(int t);
	Rect getBoundingBox();

private:
	Sprite *pEnemySprite;//ʵ��
	int nLife;
	int nScore;
};
#endif