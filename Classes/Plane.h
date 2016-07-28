#ifndef __MyPlane_H__
#define __MyPlane_H__

#include "cocos2d.h"

USING_NS_CC;
#define SCENE_WIDTH   512 //��Ұ���
#define SCENE_HEIGHT   768  //��Ұ�߶�
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
	int AIRLEVEL = 1;//�ɻ��ȼ�
	int AIRBASEHP = 20;
	int AIRHP = 20;//�ɻ�����������
public:
	//�ɻ��ƶ�
	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);//�����ص�����ʼ
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);//�����ص����ƶ�
	void keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);//���̻ص�
	//�ɻ�����
	//Animate* planeFly();
	
	//�߽���
	//void borderCheck(float dt);
	//�ɻ���ը
	Animate* boomAnimate();
	void blowUp();

	//�Ƴ��ɻ�
	void removePlane();

	//��ȡ����ֵ
	int getHP();

	//��������ֵ
	void addHP(int t);

	//Ѫ������
	void setBloodbar();

	// ��������ֵ,return false���ɻ�����
	bool loseHP(int t);

	// ��ȡ�ȼ�
	int getAirLevel();

	//����
	void airLevelUp();

public:
	static MyPlane* instancePlane;	//�ɻ�ʵ��
	CCProgressTimer *pBloodProGress; //Ѫ��

private:
	int m_hp;
	Label* hpItem1;
	Label* hpItem2;

};

#endif