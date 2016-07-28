#include "Bullet.h"

Bullet* Bullet::instanceBullet = NULL;//�ӵ�ʵ��
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
	BeginBulletShoot();//��ʱ��
	return true;
}

void Bullet::bindEnemyManager(EnemyManager* enemyManager)
{
	this->m_enemyManager = enemyManager;
	m_enemyManager->retain();
}

void Bullet::BeginBulletShoot(float dt)
{
	this->schedule(schedule_selector(Bullet::addBullet), 0.2f, kRepeatForever, dt);//���������ٶ�todo
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
	bullet->setTag(getDamage());//�����˺�
	return bullet;
}
void Bullet::setBulletPosition(Sprite* bullet,int t)
{
	// ��÷ɻ���λ��
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

	// ���г��� ���о��ǳ�������
	float flyLen = Director::getInstance()->getWinSize().height + bullet->getContentSize().height / 2 - bulletPos.y;
	float flyVelocity = 320 / 1; //�����ٶ�
	float realFlyDuration = flyLen / flyVelocity; // ����ʱ��
	auto actionMove = MoveTo::create(realFlyDuration, Point(bulletPos.x, Director::getInstance()->getWinSize().height + bullet->getContentSize().height / 2));
	auto actionDone = CallFuncN::create(CC_CALLBACK_1(Bullet::removeBullet, this));

	Sequence* sequence = Sequence::create(actionMove, actionDone, NULL);
	bullet->runAction(sequence);
}
void Bullet::addOneBullet(int i)
{
	auto bullet = setBulletType();
	setBulletPosition(bullet, i);
	this->addChild(bullet);		// �ӵ�Layer��ȥ
	vecBullet.pushBack(bullet); // �ӵ�������ȥ�������Ժ����ײ����
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
	if (!autoBullet) return;//�Ƿ��Զ������ӵ����

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

// ��ӵл��ӵ�
void Bullet::addEnemyBullet(float dt)
{

	for (auto& eEnemy : m_enemyManager->vecEnemy)
	{
		EnemyBase* pEnemySprite = (EnemyBase*)eEnemy;

		// �ӵ�
		auto bullet = Sprite::create("bullet2.png");
		if (NULL == bullet)
		{
			return;
		}

		this->addChild(bullet);		// �ӵ�Layer��ȥ
		vecEnemyBullet.pushBack(bullet); // �ӵ�������ȥ�������Ժ����ײ����

										 // ��õз��ɻ���λ��
		Point enemyPos = pEnemySprite->getPosition();

		Point bulletPos = Point(enemyPos.x, enemyPos.y - pEnemySprite->getContentSize().height / 2);
		bullet->setPosition(bulletPos);

		// ���г��� ���о��ǳ�������
		float flyLen = bulletPos.y - bullet->getContentSize().height / 2;
		float flyVelocity = 320 / 1; //�����ٶ�
		float realFlyDuration = flyLen / flyVelocity; // ����ʱ��
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

// �Ƴ��л��ӵ�
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