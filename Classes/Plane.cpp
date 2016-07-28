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

MyPlane* MyPlane::instancePlane = NULL;//飞机实例

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

	//添加飞机
	auto spPlane = Sprite::create("plane/0.png");
	spPlane->setPosition(Point(visibleSize.width / 2, origin.y + visibleSize.height / 10));
	spPlane->setTag(AIRTAG);
	//m_planeSprite->runAction(this->planeFly());//飞机动画
	//设置物理世界实体
	//这里只有我机、敌机、子弹，子弹和敌机可以碰撞，敌机和我机可以碰撞，我机和子弹不可以碰撞
	//auto planeBody = PhysicsBody::createBox(spPlane->getContentSize());
	//planeBody->setContactTestBitmask(0x0003);            //碰撞测试掩码
	//planeBody->setCategoryBitmask(0x0001);                //类别掩码
	//planeBody->setCollisionBitmask(0x0007);                 //碰撞掩码
	//planeBody->setGravityEnable(false);                          //设置重力无效，飞机是在天空中的，别让他掉下来。
	//spPlane->setPhysicsBody(planeBody);
	this->addChild(spPlane);

	//血量条
	Point p(visibleSize.width / 2, 20);
	CCSprite *pBloodKongSp = CCSprite::create("others/83.png");//空血条
	this->addChild(pBloodKongSp);
	pBloodKongSp->setPosition(p);
	CCSprite *pBloodManSp = CCSprite::create("others/84.png");//满血条
	pBloodProGress = CCProgressTimer::create(pBloodManSp);
	pBloodProGress->setType(kCCProgressTimerTypeBar);
	pBloodProGress->setBarChangeRate(Vec2(1, 0));
	pBloodProGress->setMidpoint(Vec2(0, 0));
	pBloodProGress->setPosition(p);
	pBloodProGress->setPercentage(100);
	this->addChild(pBloodProGress);
	
	//pBloodProGress->setPercentage(90);

	//触摸控制飞机移动
	EventListenerTouchOneByOne * event = EventListenerTouchOneByOne::create();
	event->setSwallowTouches(true);
	if (event->onTouchBegan = CC_CALLBACK_2(MyPlane::onTouchBegan, this))
	{
		event->onTouchMoved = CC_CALLBACK_2(MyPlane::onTouchMoved, this);
	}
	_eventDispatcher->addEventListenerWithSceneGraphPriority(event, this);

	//键盘控制飞机移动
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
	Point touch = pTouch->getLocation();//返回点击的位置
	auto spPlayer = this->getChildByTag(AIRTAG);
	if (spPlayer == NULL) return false;
	Rect rectPlayer = spPlayer->getBoundingBox();//飞机所占矩形区域的大小  
	if (rectPlayer.containsPoint(touch)) {//如果点击的点在这个矩形区域内就可以对飞机进行拖动  
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
	Rect rectScene = Rect(0, 0, SCENE_WIDTH - t.x, SCENE_HEIGHT - t.y);//边缘碰撞检测，避免飞机飞到屏幕外面去
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
	Rect rectScene = Rect(0, 0, SCENE_WIDTH - t.x, SCENE_HEIGHT - t.y);//边缘碰撞检测，避免飞机飞到屏幕外面去
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

//爆炸动画
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
	this->unscheduleAllSelectors(); // 停止飞机的所有行动
	Animate *pAnimate = MyPlane::boomAnimate();
	//爆炸音效todo

	//auto pActionDone = CallFuncN::create(CC_CALLBACK_0(MyPlane::removePlane, this, this->getChildByTag(AIRTAG)));
	CallFunc* m_removePlane = CallFunc::create(this, callfunc_selector(MyPlane::removePlane));
	Sequence* pSequence = Sequence::create(pAnimate, m_removePlane, NULL);
	this->getChildByTag(AIRTAG)->runAction(pSequence);
}
void MyPlane::removePlane()
{
	// 移除飞机精灵 true子节点上的所有运行行为和回调将清理
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
	//升级动画todo
	setBloodbar();
}

void MyPlane::setBloodbar()
{
	float t = (float)AIRHP / (float)AIRBASEHP * 100;
	if (AIRHP <= 0) t = 0;
	pBloodProGress->setPercentage(t);	
}