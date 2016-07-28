#include "EnemyManager.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
EnemyManager::EnemyManager() : vecEnemy(NULL)
{

}

EnemyManager::~EnemyManager()
{

}
bool EnemyManager::init()
{
	if (!Layer::init())
	{
		return false;
	}
	this->schedule(schedule_selector(EnemyManager::addEnemy), m_fCreatSpeed);
	return true;
}

//绑定控制器(更新分数)  
void EnemyManager::bindController(Controller* controller)
{
	this->m_controlLayer = controller;
	m_controlLayer->retain();
}

Sequence* EnemyManager::MovingTrack()
{
	Size size = Director::getInstance()->getVisibleSize();
	Point p1,p2,p3;
	int w = size.width,h= size.height;
	p1.x = rand() % w; p1.y = rand() % h;
	p2.x = rand() % w; p2.y = rand() % h;
	p3.x = rand() % w; p3.y = rand() % h;
	ccBezierConfig m_bezier;
	m_bezier.controlPoint_1 = ccp(p1.x,p1.y);
	m_bezier.controlPoint_2 = ccp(p2.x,p2.y);
	m_bezier.endPosition = ccp(p3.x,p3.y);
	int t = 0;
	t = rand() % 10;
	if (t < 2) t = 2;
	auto actionMove = BezierTo::create(t, m_bezier);

	auto actionDone = CallFuncN::create(CC_CALLBACK_1(EnemyManager::removeEnemy, this));
	Sequence* sequence = Sequence::create(actionMove, actionDone, NULL);
	//sequence->setTag(100);//为了stop
	return sequence;
}

void EnemyManager::addEnemy(float dt)
{
	int level = GameScene::instanceGameScene->getLevel();
	EnemyBase *pEnemySprite = EnemyBase::create();
	std::string s;
	int st = rand() % level + 1;
	s = (char)st + '0';
	s += '_';
	int t = rand() % 3+1;
	s += (char)t + '0';
	pEnemySprite->setEnemyByType(s);
	pEnemySprite->setName(s);
	this->addChild(pEnemySprite);
	vecEnemy.pushBack(pEnemySprite);

	pEnemySprite->runAction(MovingTrack());
}
void EnemyManager::removeEnemy(Node *pNode)
{
	EnemyBase* enemy = (EnemyBase*)pNode;
	if (enemy != NULL)
	{
		this->removeChild(enemy, true);
		vecEnemy.eraseObject(enemy);
	}
}
//爆炸动画
Animate* EnemyManager::boomAnimate()
{
	Vector<SpriteFrame*> boomFrame;
	for (int i = 0; i < 7; i++)
	{
		auto tempString = __String::createWithFormat("boom/%d.png", i + 1);
		//auto tempAnimate = SpriteFrameCache::getInstance()->getSpriteFrameByName(tempString->getCString());
		SpriteFrame * sf = SpriteFrame::create(tempString->getCString(),Rect(0,0,51,51));
		boomFrame.pushBack(sf);
	}

	auto animate = Animate::create(Animation::createWithSpriteFrames(boomFrame, 0.1f));
	return animate;
}
void EnemyManager::blowupEnemy(EnemyBase* pEnemySprite)
{
	auto sprite = Sprite::create();
	sprite->setPosition(pEnemySprite->getPosition());
	auto saveData = m_controlLayer->getSaveData();
	//爆炸音效todo
	saveData->setScore(saveData->getScore() + pEnemySprite->getScore());//加分
	//
	checkCanLevel(saveData->getScore());

	Animate *pAnimate = EnemyManager::boomAnimate();
	
	// 爆炸完，要移除敌机
	auto pActionDone = CallFuncN::create(CC_CALLBACK_0(EnemyManager::removeEnemy, this, pEnemySprite));//回调函数
	Sequence* pSequence = Sequence::create(pActionDone, NULL);
	pEnemySprite->getSprite()->runAction(pSequence);
	auto * act = Sequence::create(
		pAnimate,//动画
		CCCallFuncN::create(sprite, callfuncN_selector(EnemyManager::removeAct)),
		NULL);
	this->addChild(sprite);
	sprite->runAction(act);
}
void EnemyManager::removeAct(Node * pSender)
{
	pSender->removeFromParentAndCleanup(true);
}
void EnemyManager::checkCanLevel(int score)
{
	int level = GameScene::instanceGameScene->getLevel();
	int t = 0;
	switch (level)
	{
	case 1:
		t = 1000;
		break;
	case 2:
		t = 5000;
		break;
	case 3:
		t = 20000;
		break;
	case 4:
		t = 50000;
		break;
	case 5:
		t = 100000;
		break;
	default:
		break;
	}
	if (score > t) 
	{
		//level up
		GameScene::instanceGameScene->levelUp();
	}
}