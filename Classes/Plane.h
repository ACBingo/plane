#ifndef __MyPlane_H__
#define __MyPlane_H__

#include "cocos2d.h"

USING_NS_CC;
#define SCENE_WIDTH   512 //视野宽度
#define SCENE_HEIGHT   768  //视野高度
enum
{
	AIRTAG = 1,
	AIRPLANE2,
	AIRPLANE3,
};

class MyPlane : public Layer
{
public:
	MyPlane();
	~MyPlane();

	static MyPlane* create();

	virtual bool init();
	int AIRLEVEL = 1;//飞机等级
	int AIRBASEHP = 20;
	int AIRHP = 20;//飞机的生命总数
public:
	//飞机移动
	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);//触摸回调，开始
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);//触摸回调，移动
	void keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);//键盘回调
	//飞机动画
	//Animate* planeFly();
	
	//边界检测
	//void borderCheck(float dt);
	//飞机爆炸
	Animate* boomAnimate();
	void blowUp();

	//移除飞机
	void removePlane();

	//获取生命值
	int getHP();

	//增加生命值
	void addHP(int t);

	//血条更新
	void setBloodbar();

	// 减少生命值,return false：飞机死亡
	bool loseHP(int t);

	// 获取等级
	int getAirLevel();

	//升级
	void airLevelUp();

public:
	static MyPlane* instancePlane;	//飞机实体
	CCProgressTimer *pBloodProGress; //血条

private:
	int m_hp;
	Label* hpItem1;
	Label* hpItem2;

};

#endif