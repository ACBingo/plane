#include "EnemyBase.h"

EnemyBase::EnemyBase() : pEnemySprite(NULL)
{

}

EnemyBase::~EnemyBase()
{

}

void EnemyBase::setEnemyByType(std::string enType)
{
	std::string s ("enemy/");
	s += enType;
	s += ".png";
	//pEnemySprite = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("s"));//从缓存中读
	pEnemySprite = Sprite::create(s);
	auto t = pEnemySprite->getContentSize();
	//pEnemySprite->setScale(0.5);//图片太大了，缩放到0.5倍大小
	//pEnemySprite->setContentSize(pEnemySprite->getContentSize()*0.5);//同时，更新下属性大小，方便后面碰撞检测
	t = pEnemySprite->getContentSize();
	s = "ENEMY" + enType;
	nLife = enemyHP.at(s);//敌机生命值
	nScore = nLife * 10;

	this->addChild(pEnemySprite);
	Size winSize = Director::getInstance()->getWinSize();
	Size enemySize = pEnemySprite->getContentSize();
	int minX = enemySize.width / 2;
	int maxX = winSize.width - enemySize.width / 2;
	int rangeX = maxX - minX;
	int actualX = (rand() % rangeX) + minX;

	// 设置敌机Node方位 Node包含Sprite
	this->setPosition(Point(actualX, winSize.height - enemySize.height / 2));
}

bool EnemyBase::init()
{
	enemyHP["ENEMY1_1"]= ENEMY1_1;
	enemyHP["ENEMY1_1"]= ENEMY1_1;
	enemyHP["ENEMY1_2"]= ENEMY1_2;
	enemyHP["ENEMY1_3"]= ENEMY1_3;
	enemyHP["ENEMY2_1"]= ENEMY2_1;
	enemyHP["ENEMY2_2"]= ENEMY2_2;
	enemyHP["ENEMY2_3"]= ENEMY2_3;
	enemyHP["ENEMY3_1"]= ENEMY3_1;
	enemyHP["ENEMY3_2"]= ENEMY3_2;
	enemyHP["ENEMY3_3"]= ENEMY3_3;
	enemyHP["ENEMY4_1"]= ENEMY4_1;
	enemyHP["ENEMY4_2"]= ENEMY4_2;
	enemyHP["ENEMY4_3"]= ENEMY4_3;
	enemyHP["ENEMY5_1"]= ENEMY5_1;
	enemyHP["ENEMY5_2"]= ENEMY5_2;
	enemyHP["ENEMY5_3"]= ENEMY5_3;
	enemyHP["ENEMYBOSS1"]= ENEMYBOSS1;
	enemyHP["ENEMYBOSS2"]= ENEMYBOSS2;
	enemyHP["ENEMYBOSS3"]= ENEMYBOSS3;
	enemyHP["ENEMYBOSS4"]= ENEMYBOSS4;
	enemyHP["ENEMYBOSS5"]= ENEMYBOSS5;
	enemyHP["ENEMYBOSS6"]= ENEMYBOSS6;
	bool pRet = true;
	if (!Node::init())
	{
		pRet = false;
	}

	return pRet;
}
Sprite* EnemyBase::getSprite()
{
	return pEnemySprite;
}

int EnemyBase::getLife()
{
	return nLife;
}

bool EnemyBase::loseLife(int t)
{
	nLife-=t;
	if (nLife <= 0) return false;
	return true;
}
int EnemyBase::getScore()
{
	return nScore;
}
Rect EnemyBase::getBoundingBox()
{
	Rect rect = pEnemySprite->boundingBox();
	Point pos = this->convertToWorldSpace(rect.origin);
	Rect enemyRect(pos.x, pos.y, rect.size.width, rect.size.height);
	return enemyRect;
}
