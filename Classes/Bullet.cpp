#include "Bullet.h"

Bullet* Bullet::instanceBullet = NULL;//子弹实例
Bullet::Bullet() :damage(1), autoBullet(true)
{

}

Bullet::~Bullet()
{

}

bool Bullet::init()
{
	if (!Layer::init())
	{
		return false;
	}
	BeginBulletShoot();//计时器
	return true;
}

void Bullet::bindEnemyManager(EnemyManager* enemyManager)
{
	this->m_enemyManager = enemyManager;
	m_enemyManager->retain();
}

void Bullet::BeginBulletShoot(float dt)
{
	this->schedule(schedule_selector(Bullet::addBullet), 0.2f, kRepeatForever, dt);//两个产生速度todo
	this->schedule(schedule_selector(Bullet::addEnemyBullet), 1.0f, kRepeatForever, dt);//
}

void Bullet::StopBulletShoot()
{
	this->unschedule(schedule_selector(Bullet::addBullet));
	this->unschedule(schedule_selector(Bullet::addEnemyBullet));
}
Sprite * Bullet::setBulletType() 
{
	std::string s = "bullet/";
	s += getDamage()+'0';
	s += ".png";
	auto bullet = Sprite::create(s);
	bullet->setTag(getDamage());//设置伤害
	return bullet;
}
void Bullet::setBulletPosition(Sprite* bullet,int t)
{
	// 获得飞机的位置
	Point planePos = MyPlane::instancePlane->getChildByTag(AIRTAG)->getPosition();
	int airLevel = MyPlane::instancePlane->getAirLevel();
	Point bulletPos1 = Point(planePos.x, planePos.y + MyPlane::instancePlane->getChildByTag(AIRTAG)->getContentSize().height / 2);
	Point bulletPos2 = Point(planePos.x - 20, planePos.y + MyPlane::instancePlane->getChildByTag(AIRTAG)->getContentSize().height / 2 - 5);
	Point bulletPos3 = Point(planePos.x + 20, planePos.y + MyPlane::instancePlane->getChildByTag(AIRTAG)->getContentSize().height / 2 - 5);
	Point bulletPos4 = Point(planePos.x - 40, planePos.y + MyPlane::instancePlane->getChildByTag(AIRTAG)->getContentSize().height / 2 - 10);
	Point bulletPos5 = Point(planePos.x + 40, planePos.y + MyPlane::instancePlane->getChildByTag(AIRTAG)->getContentSize().height / 2 - 10);
	Point bulletPos;
	switch (airLevel)
	{
	case 1 :
		bulletPos = bulletPos1;
		break;
	case 2 :
		if (t == 1) bulletPos = bulletPos2;
		if (t == 2) bulletPos = bulletPos3;
		break;
	case 3 :
		if (t == 1) bulletPos = bulletPos2;
		if (t == 2) bulletPos = bulletPos3;
		if (t == 3) bulletPos = bulletPos4;
		if (t == 4) bulletPos = bulletPos5;
		break;
	case 4 :
		if (t == 1) bulletPos = bulletPos1;
		if (t == 2) bulletPos = bulletPos2;
		if (t == 3) bulletPos = bulletPos3;
		if (t == 4) bulletPos = bulletPos4;
		if (t == 5) bulletPos = bulletPos5;
	default:
		break;
	}
	bullet->setPosition(bulletPos);

	// 飞行长度 飞行就是超出窗体
	float flyLen = Director::getInstance()->getWinSize().height + bullet->getContentSize().height / 2 - bulletPos.y;
	float flyVelocity = 320 / 1; //飞行速度
	float realFlyDuration = flyLen / flyVelocity; // 飞行时间
	auto actionMove = MoveTo::create(realFlyDuration, Point(bulletPos.x, Director::getInstance()->getWinSize().height + bullet->getContentSize().height / 2));
	auto actionDone = CallFuncN::create(CC_CALLBACK_1(Bullet::removeBullet, this));

	Sequence* sequence = Sequence::create(actionMove, actionDone, NULL);
	bullet->runAction(sequence);
}
void Bullet::addOneBullet(int i)
{
	auto bullet = setBulletType();
	setBulletPosition(bullet, i);
	this->addChild(bullet);		// 加到Layer中去
	vecBullet.pushBack(bullet); // 加到容器中去，用于以后的碰撞检测等
}
void Bullet::addKeyBullet()
{
	int airLevel = MyPlane::instancePlane->getAirLevel();
	if (airLevel >= 3) airLevel++;
	for (int i = 1; i <= airLevel; i++)
	{
		addOneBullet(i);
	}
}
void Bullet::addBullet(float dt)
{
	if (!autoBullet) return;//是否自动发射子弹检测

	int airLevel = MyPlane::instancePlane->getAirLevel();
	if (airLevel >= 3) airLevel++;
	for (int i = 1; i <= airLevel; i++)
	{
		addOneBullet(i);
	}
	

	//if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	//{
	//	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/bullet.wav");
	//}

}

void Bullet::removeBullet(Node* pNode)
{
	if (NULL == pNode)
	{
		return;
	}

	Sprite* bullet = (Sprite*)pNode;
	this->removeChild(bullet, true);
	vecBullet.eraseObject(bullet);
}

// 添加敌机子弹
void Bullet::addEnemyBullet(float dt)
{

	for (auto& eEnemy : m_enemyManager->vecEnemy)
	{
		EnemyBase* pEnemySprite = (EnemyBase*)eEnemy;

		// 子弹
		auto bullet = Sprite::create("bullet2.png");
		if (NULL == bullet)
		{
			return;
		}

		this->addChild(bullet);		// 加到Layer中去
		vecEnemyBullet.pushBack(bullet); // 加到容器中去，用于以后的碰撞检测等

										 // 获得敌方飞机的位置
		Point enemyPos = pEnemySprite->getPosition();

		Point bulletPos = Point(enemyPos.x, enemyPos.y - pEnemySprite->getContentSize().height / 2);
		bullet->setPosition(bulletPos);

		// 飞行长度 飞行就是超出窗体
		float flyLen = bulletPos.y - bullet->getContentSize().height / 2;
		float flyVelocity = 320 / 1; //飞行速度
		float realFlyDuration = flyLen / flyVelocity; // 飞行时间
		auto actionMove = MoveTo::create(realFlyDuration, Point(bulletPos.x, Director::getInstance()->getVisibleOrigin().y - bullet->getContentSize().height / 2));
		auto actionDone = CallFuncN::create(CC_CALLBACK_1(Bullet::removeEnemyBullet, this));

		Sequence* sequence = Sequence::create(actionMove, actionDone, NULL);
		bullet->runAction(sequence);

		if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/enemyBullet.wav");
		}

	}
}

// 移除敌机子弹
void Bullet::removeEnemyBullet(Node* pNode)
{
	if (NULL == pNode)
	{
		return;
	}

	Sprite* bullet = (Sprite*)pNode;
	this->removeChild(bullet, true);
	vecEnemyBullet.eraseObject(bullet);
}

void Bullet::setDamage(int t)
{
	damage = t;
}

void Bullet::addDamage(int t)
{
	damage += t;
	if (damage > 5) damage = 5;
}

int Bullet::getDamage()
{
	return damage;
}

void Bullet::setAutoBullet()
{
	autoBullet = !autoBullet;
}