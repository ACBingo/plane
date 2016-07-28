#include "Plane.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
#include "Bullet.h"

MyPlane::MyPlane() :m_hp(AIRHP)
{

}
MyPlane::~MyPlane()
{

}

MyPlane* MyPlane::instancePlane = NULL;//�ɻ�ʵ��

MyPlane* MyPlane::create()
{
	MyPlane* m_plane = NULL;
	do
	{
		m_plane = new MyPlane();
		CC_BREAK_IF(!m_plane);

		if (m_plane && m_plane->init())
		{
			m_plane->autorelease();
			instancePlane = m_plane;
		}
		else
			CC_SAFE_DELETE(m_plane);
	} while (0);

	return m_plane;
}


bool MyPlane::init()
{
	if (!Layer::init())
		return false;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//��ӷɻ�
	auto spPlane = Sprite::create("plane/0.png");
	spPlane->setPosition(Point(visibleSize.width / 2, origin.y + visibleSize.height / 10));
	spPlane->setTag(AIRTAG);
	//m_planeSprite->runAction(this->planeFly());//�ɻ�����
	//������������ʵ��
	//����ֻ���һ����л����ӵ����ӵ��͵л�������ײ���л����һ�������ײ���һ����ӵ���������ײ
	//auto planeBody = PhysicsBody::createBox(spPlane->getContentSize());
	//planeBody->setContactTestBitmask(0x0003);            //��ײ��������
	//planeBody->setCategoryBitmask(0x0001);                //�������
	//planeBody->setCollisionBitmask(0x0007);                 //��ײ����
	//planeBody->setGravityEnable(false);                          //����������Ч���ɻ���������еģ���������������
	//spPlane->setPhysicsBody(planeBody);
	this->addChild(spPlane);

	//Ѫ����
	Point p(visibleSize.width / 2, 20);
	CCSprite *pBloodKongSp = CCSprite::create("others/83.png");//��Ѫ��
	this->addChild(pBloodKongSp);
	pBloodKongSp->setPosition(p);
	CCSprite *pBloodManSp = CCSprite::create("others/84.png");//��Ѫ��
	pBloodProGress = CCProgressTimer::create(pBloodManSp);
	pBloodProGress->setType(kCCProgressTimerTypeBar);
	pBloodProGress->setBarChangeRate(Vec2(1, 0));
	pBloodProGress->setMidpoint(Vec2(0, 0));
	pBloodProGress->setPosition(p);
	pBloodProGress->setPercentage(100);
	this->addChild(pBloodProGress);
	
	//pBloodProGress->setPercentage(90);

	//�������Ʒɻ��ƶ�
	EventListenerTouchOneByOne * event = EventListenerTouchOneByOne::create();
	event->setSwallowTouches(true);
	if (event->onTouchBegan = CC_CALLBACK_2(MyPlane::onTouchBegan, this))
	{
		event->onTouchMoved = CC_CALLBACK_2(MyPlane::onTouchMoved, this);
	}
	_eventDispatcher->addEventListenerWithSceneGraphPriority(event, this);

	//���̿��Ʒɻ��ƶ�
	auto keyboardListener = EventListenerKeyboard::create();
	/*keyboardListener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event) {
	GameScene::keyPressed(keyCode, event);
	};
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);*/
	keyboardListener->onKeyPressed = CC_CALLBACK_2(MyPlane::keyPressed, this);
	//keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::keyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	return true;
}

bool MyPlane::onTouchBegan(Touch *pTouch, Event *pEvent) {
	Point touch = pTouch->getLocation();//���ص����λ��
	auto spPlayer = this->getChildByTag(AIRTAG);
	if (spPlayer == NULL) return false;
	Rect rectPlayer = spPlayer->getBoundingBox();//�ɻ���ռ��������Ĵ�С  
	if (rectPlayer.containsPoint(touch)) {//�������ĵ���������������ھͿ��ԶԷɻ������϶�  
		return true;
	}
	return false;
}
void MyPlane::onTouchMoved(Touch *pTouch, Event *pEvent) {
	auto spPlayer = this->getChildByTag(AIRTAG);
	if (spPlayer == NULL) return;
	Point temp = pTouch->getDelta();
	Point p = spPlayer->getPosition() + temp;
	Point t = spPlayer->getContentSize();
	Rect rectScene = Rect(0, 0, SCENE_WIDTH - t.x, SCENE_HEIGHT - t.y);//��Ե��ײ��⣬����ɻ��ɵ���Ļ����ȥ
	if (rectScene.containsPoint(p))
	{
		spPlayer->setPosition(p);
	}
	else
	{
		if (p.x < 0) p.x = 0;
		if (p.y < 0) p.y = 0;
		if (p.x > 512 ) p.x = SCENE_WIDTH ;
		if (p.y > 768 ) p.y = SCENE_HEIGHT ;
		spPlayer->setPosition(p);
	}
}


void MyPlane::keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	auto spPlayer = this->getChildByTag(AIRTAG);
	Point p = spPlayer->getPosition();
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_W:
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		p += Point(0, 50); break;
	case EventKeyboard::KeyCode::KEY_S:
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		p += Point(0, -50); break;
	case EventKeyboard::KeyCode::KEY_A:
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		p += Point(-50, 0); break;
	case EventKeyboard::KeyCode::KEY_D:
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		p += Point(50, 0); break;
	case EventKeyboard::KeyCode::KEY_SPACE:
		GameScene::instanceGameScene->boomAction(); break;
	case EventKeyboard::KeyCode::KEY_P:
		Bullet::instanceBullet->setAutoBullet(); break;
	case EventKeyboard::KeyCode::KEY_J:
		Bullet::instanceBullet->addKeyBullet(); break;
	}
	Point t = spPlayer->getContentSize();
	Rect rectScene = Rect(0, 0, SCENE_WIDTH - t.x, SCENE_HEIGHT - t.y);//��Ե��ײ��⣬����ɻ��ɵ���Ļ����ȥ
	if (!rectScene.containsPoint(p))
	{
		if (p.x < 0) p.x = 0;
		if (p.y < 0) p.y = 0;
		if (p.x > 512 ) p.x = SCENE_WIDTH ;
		if (p.y > 768 ) p.y = SCENE_HEIGHT ;
	}
	auto moveto = MoveTo::create(0.5f, p);
	spPlayer->runAction(moveto);
}

//��ը����
Animate* MyPlane::boomAnimate()
{
	Vector<SpriteFrame*> boomFrame;
	for (int i = 0; i < 7; i++)
	{
		auto tempString = __String::createWithFormat("boom/%d.png", i + 1);
		//auto tempAnimate = SpriteFrameCache::getInstance()->getSpriteFrameByName(tempString->getCString());
		SpriteFrame * sf = SpriteFrame::create(tempString->getCString(), Rect(0, 0, 51, 51));
		boomFrame.pushBack(sf);
	}

	auto animate = Animate::create(Animation::createWithSpriteFrames(boomFrame, 0.2f));
	return animate;
}

void MyPlane::blowUp()
{
	this->unscheduleAllSelectors(); // ֹͣ�ɻ��������ж�
	Animate *pAnimate = MyPlane::boomAnimate();
	//��ը��Чtodo

	//auto pActionDone = CallFuncN::create(CC_CALLBACK_0(MyPlane::removePlane, this, this->getChildByTag(AIRTAG)));
	CallFunc* m_removePlane = CallFunc::create(this, callfunc_selector(MyPlane::removePlane));
	Sequence* pSequence = Sequence::create(pAnimate, m_removePlane, NULL);
	this->getChildByTag(AIRTAG)->runAction(pSequence);
}
void MyPlane::removePlane()
{
	// �Ƴ��ɻ����� true�ӽڵ��ϵ�����������Ϊ�ͻص�������
	this->removeChildByTag(AIRTAG, true);
}
int MyPlane::getHP()
{
	return AIRHP;
}
void MyPlane::addHP(int t)
{
	AIRHP += t;
	if (AIRHP > AIRBASEHP) AIRHP = AIRBASEHP;
	setBloodbar();
}
bool MyPlane::loseHP(int t)
{
	AIRHP -= t;
	setBloodbar();
	if (AIRHP > 0) return true;
	return false;
}
int MyPlane::getAirLevel()
{
	return AIRLEVEL;
}

void MyPlane::airLevelUp()
{
	Sprite * spPlane = (Sprite *) this->getChildByTag(AIRTAG);
	AIRLEVEL++;
	if (AIRLEVEL > 4) AIRLEVEL = 4;
	AIRBASEHP *= 2;
	AIRHP = AIRBASEHP;
	std::string s = "plane/";
	s += (char)(AIRLEVEL + '0' - 1);
	s += ".png";
	spPlane->setTexture(s);
	//��������todo
	setBloodbar();
}

void MyPlane::setBloodbar()
{
	float t = (float)AIRHP / (float)AIRBASEHP * 100;
	if (AIRHP <= 0) t = 0;
	pBloodProGress->setPercentage(t);	
}