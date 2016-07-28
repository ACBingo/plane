#include "GameScene.h"
#include "GameOver.h"
//#include "CCControlSlider.h"

#define SCENE_WIDTH   512 //视野宽度
#define SCENE_HEIGHT   768  //视野高度
/*Tag
0：score
1,2:background
9:gamescene;
10:player_plane
30:enemy
50:bullet
60:boom
*/
GameScene::GameScene() : planeLayer(NULL), bulletLayer(NULL), enemyLayer(NULL), controlLayer(NULL), itemLayer(NULL), boomNum(3)
{

}
GameScene::~GameScene()
{
	_eventDispatcher->removeEventListenersForTarget(this);
}
bool GameScene::init() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	level = 1;
	status = 1;
	score = 0;

	// 启动触摸机制
	this->setTouchEnabled(true);

	//背景滚动
	this->setBackGround();
	int backgroundSpeed = 0.005;//背景移动速度
	this->schedule(schedule_selector(GameScene::moveBackground), backgroundSpeed);//背景移动计时器
				
	//碰撞检测
	this->schedule(schedule_selector(GameScene::gameUpdate));
	
	// 加入控制层
	controlLayer = Controller::create();
	this->addChild(controlLayer);

	// 加入飞机
	planeLayer = MyPlane::create();
	this->addChild(planeLayer, 1);

	// 加入敌机和分数显示
	enemyLayer = EnemyManager::create();
	enemyLayer->bindController(controlLayer);
	this->addChild(enemyLayer, 1);

	// 开启子弹
	bulletLayer = Bullet::create();
	bulletLayer->bindEnemyManager(enemyLayer);
	Bullet::instanceBullet = bulletLayer;
	this->addChild(bulletLayer, 1);
	
	// 开启物品掉落系统
	itemLayer = ItemDrop::create();
	this->addChild(itemLayer, 1);

	//清屏导弹
	creatClearBoom();

	return true;
}
GameScene * GameScene::instanceGameScene = NULL;//初始化该实例
Scene * GameScene::createScene() {
	//auto scene = Scene::createWithPhysics();         //创建物理世界的场景
	auto scene = Scene::create();//不知道为啥，改不回普通世界了，先这样吧= =
	auto layer = GameScene::create();
	instanceGameScene = layer;
	scene->addChild(layer);
	return scene;
}
void GameScene::moveBackground(float t) {
	auto spbk = this->getChildByTag(1);
	auto spbk2 = this->getChildByTag(2);
	spbk->setPositionY(spbk->getPositionY() - 0.5);
	if (spbk->getPositionY() < -SCENE_HEIGHT) {
		spbk->setPositionY(0);
	}
	spbk2->setPositionY(spbk->getPositionY() + SCENE_HEIGHT);//始终让spbk02在背景1的上方
}
void GameScene::levelUp() 
{
	if (level == 5) return;
	level++;//关卡加一
	Sprite * tbksprite = (Sprite *) this->getChildByTag(1);
	Sprite * tbksprite2 = (Sprite *) this->getChildByTag(2);

	std::string s = "img_bg_level_";//更换背景图片
	s += (char)(level + 48);
	s += ".jpg";
	tbksprite->setTexture(s);
	tbksprite2->setTexture(s);
	
	//todo
}
void GameScene::setBackGround() 
{
	std::string s = "img_bg_level_";
	s += (char)(level+48);
	s += ".jpg";
	
	auto bksprite = Sprite::create(s);//背景图片
	bksprite->setPosition(Point::ZERO);
	bksprite->setAnchorPoint(Point::ZERO);
	bksprite->setTag(1);
	this->addChild(bksprite);
	auto bksprite2 = Sprite::create(s);//背景图片
	bksprite2->setPosition(Point::ZERO);
	bksprite2->setAnchorPoint(Point::ZERO);
	bksprite2->setTag(2);
	this->addChild(bksprite2);
}

int GameScene::getLevel()
{
	return level;
}

void GameScene::gameUpdate(float dt)//检测
{
	bool bMoveButt = false;

	//子弹和敌机碰撞检测
	for (auto& eButtle : bulletLayer->vecBullet)
	{
		Sprite* pBullet = (Sprite*)eButtle; // 获取子弹精灵
		bMoveButt = bulletCollisionEnemy(pBullet);//
		if (bMoveButt)
		{
			// 子弹删除了，无需再遍历了
			return;
		}
	}
	


	// 敌机与我机碰撞检测以及敌机子弹与我机子弹碰撞检测
	enemyCollisionPlane();

	//Boss出战检测


	//关卡升级检测
	//物品拾取检测
	bool bItem = false;
	for (auto& eItem : itemLayer->vecItem)
	{
		Sprite* pItem = (Sprite*)eItem;
		bItem = ItemCollisionPlane(pItem);
		//物品被吃了后，无需再遍历了
		return;
	}

}


bool GameScene::bulletCollisionEnemy(Sprite* pBullet)
{
	
	for (auto& eEnemy : enemyLayer->vecEnemy)
	{
		EnemyBase* pEnemySprite = (EnemyBase*)eEnemy;

		// 是否发生碰撞
		if (pBullet->boundingBox().intersectsRect(pEnemySprite->getBoundingBox()))
		{
			if (pEnemySprite->loseLife(pBullet->getTag()) == false)
			{
				itemLayer->createItem(pEnemySprite->getPosition());
				enemyLayer->blowupEnemy(pEnemySprite);
			}

			//删除子弹
			bulletLayer->removeBullet(pBullet);
			return true;
		}
	}

	return false;
}
void GameScene::enemyCollisionPlane()
{
	Sprite* pPlane = (Sprite*)planeLayer->getChildByTag(AIRTAG);
	for (auto& eEnemy : enemyLayer->vecEnemy)
	{
		EnemyBase* pEnemySprite = (EnemyBase*)eEnemy;

		// 是否发生碰撞
		if (pPlane->boundingBox().intersectsRect(pEnemySprite->getBoundingBox()) && pEnemySprite->getLife() > 0)
		{
			if (planeLayer->loseHP(pEnemySprite->getLife()) == false)
			{
				controlLayer->getSaveData()->save();
				this->unscheduleAllSelectors();
				this->bulletLayer->StopBulletShoot();
				this->planeLayer->blowUp();
				this->scheduleOnce(schedule_selector(GameScene::toGameOver), 1.5f);//爆炸效果执行完后再切换场景
			}
			enemyLayer->blowupEnemy(pEnemySprite);
		}
	}

	for (auto& eEnemyBullet : bulletLayer->vecEnemyBullet)
	{
		Sprite* pEnemyBullet = (Sprite*)eEnemyBullet; // 获取子弹精灵

													  // 是否发生碰撞
		if (pPlane->boundingBox().intersectsRect(pEnemyBullet->getBoundingBox()))
		{
			if (1 == planeLayer->getHP())
			{
				planeLayer->loseHP(1);
				controlLayer->getSaveData()->save();
				this->unscheduleAllSelectors();
				this->bulletLayer->StopBulletShoot();
				this->planeLayer->blowUp();
				this->scheduleOnce(schedule_selector(GameScene::toGameOver), 1.5f);//爆炸效果执行完后再切换场景
			}
			else
				planeLayer->loseHP(1);

			bulletLayer->removeEnemyBullet(pEnemyBullet);

			return;
		}
	}
}
bool GameScene::ItemCollisionPlane(Sprite* pItem)
{
	Sprite* pPlane = (Sprite*)planeLayer->getChildByTag(AIRTAG);
	int c = pItem->getTag();
	if (pPlane->boundingBox().intersectsRect(pItem->getBoundingBox()))
	{
		switch (c)
		{
		case 1://加血
			planeLayer->addHP(planeLayer->AIRBASEHP/10);
			break;
		case 2://子弹加伤害
			bulletLayer->addDamage(1);
			break;
		case 5:
			addBoomNum(1);
			break;
		case 6:
			addBoomNum(3);
			break;
		case 3:
			planeLayer->airLevelUp();
			break;
		}
		itemLayer->removeItem(pItem);
	}
	
	return true;
}

void GameScene::creatClearBoom()
{
	Size winSize = Director::getInstance()->getWinSize();
	//boomNum = Label::createWithTTF("导弹：5", "fonts/DFPShaoNvW5-GB.ttf", 25);
	Sprite * boomNum = Sprite::create("others/35.png");
	boomNum->setPosition(boomNum->getContentSize().width / 2,
		winSize.height - boomNum->getContentSize().height * 2-25);
	this->addChild(boomNum);
	setBoomNum();
}
void GameScene::setBoomNum()
{
	Size winSize = Director::getInstance()->getWinSize();
	std::string s = "nums/";
	s += boomNum+'0';
	s += ".png";
	Sprite * boomNum = Sprite::create(s);
	boomNum->setPosition(boomNum->getContentSize().width / 2+50,
		winSize.height - boomNum->getContentSize().height * 2 - 10);
	boomNum->setScale(0.8);
	boomNum->setTag(60);
	this->addChild(boomNum);
}
void GameScene::setBoomPic()
{
	Sprite* spBoom = (Sprite*) this->getChildByTag(60);
	std::string s = "nums/";
	s += boomNum + '0';
	s += ".png";
	spBoom->setTexture(s);
}
void GameScene::addBoomNum(int t)
{
	boomNum += t;
	if (boomNum > 9) boomNum = 9;
	setBoomPic();
}
void GameScene::boomAction()
{
	if (boomNum == 0) return;
	boomNum -= 1;
	setBoomPic();
	Size winSize = Director::getInstance()->getWinSize();
	Sprite * spBoom = Sprite::create("others/12.png");
	spBoom->setPosition(winSize.width/2, -spBoom->getContentSize().height / 2);
	ActionInterval *moveto = MoveTo::create(2, Vec2(winSize.width/2, winSize.height/3*2));
	auto moveAction = EaseExponentialOut::create(moveto);
	
	
	Vector<SpriteFrame*> boomFrame;
	for (int i = 0; i < 6; i++)
	{
		auto tempString = __String::createWithFormat("boom/2%d.png", i + 1);
		//auto tempAnimate = SpriteFrameCache::getInstance()->getSpriteFrameByName(tempString->getCString());
		SpriteFrame * sf = SpriteFrame::create(tempString->getCString(), Rect(0, 0, 250, 250));
		boomFrame.pushBack(sf);
	}
	Animate *bAnimate=Animate::create(Animation::createWithSpriteFrames(boomFrame, 0.05f));
	auto callBack = CallFunc::create(CC_CALLBACK_0(GameScene::clearAllEnemy, this));
	auto callBack2 = CallFunc::create(CC_CALLBACK_0(GameScene::particleAct, this));
	auto * act = Sequence::create(
		moveAction,//移动动画
		bAnimate,//爆炸动画
		CCCallFuncN::create(spBoom, callfuncN_selector(GameScene::removeBoom)),//移除boom
		callBack,//allclear
		NULL);
	
	this->addChild(spBoom);
	spBoom->runAction(act);
}
void GameScene::removeBoom(Node * pSender)
{
	pSender->removeFromParentAndCleanup(true);
}
void GameScene::particleAct()
{
	Size winSize = Director::getInstance()->getWinSize();
	//粒子系统
	auto ps = ParticleSystemQuad::create("t.plist");
	ps->setPosition(Vec2(winSize.width / 2, winSize.height / 3 * 2));
	this->addChild(ps,10);
}
void GameScene::clearAllEnemy()
{
	

	for (auto& eEnemyBullet : bulletLayer->vecEnemyBullet)
	{
		Sprite* pEnemyBullet = (Sprite*)eEnemyBullet; 
		bulletLayer->removeEnemyBullet(pEnemyBullet);
	}
	for (auto& eEnemy : enemyLayer->vecEnemy)
	{
		EnemyBase* pEnemySprite = (EnemyBase*)eEnemy;
		enemyLayer->blowupEnemy(pEnemySprite);//清除敌人，todo降低100滴血
	}

}
void GameScene::toGameOver(float dt)
{
	Director::getInstance()->replaceScene(
		TransitionMoveInT::create(0.8f, GameOver::createScene())); // 替换场景
}