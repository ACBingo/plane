#include "ItemDrop.h"
#include "GameScene.h"

ItemDrop::ItemDrop()
{

}
ItemDrop::~ItemDrop()
{

}
bool ItemDrop::init()
{
	if (!Layer::init())
	{
		return false;
	}
	return true;
}
void ItemDrop::createItem(Point p)
{
	int t,c;
	std::string s("itemdrop/");
	t = rand() % 100;
	if (t > 20) return;
	switch (t)
	{
	case 0: 
	//case 1:
		c = 3;//飞机升级
		break;
	case 2:
	case 3:
	//case 4:
	//case 5:
		c = 6;//获得导弹三枚
		break;
	case 4:
	case 5:
		c = 7;//子弹加速
		break;
	case 6:
	case 7:
	case 8:
	case 9:
		c = 5;//获得导弹一枚
		break;
	case 10:
	case 11:
	case 12:
	case 13:
		c = 2;//子弹升级
		break;
	default:
		c = 1;//加血
		break;
	}

	s += c + '0';
	s += ".png";
	auto spItem = Sprite::create(s);
	spItem->setTag(c);
	spItem->setPosition(p);

	this->addChild(spItem);
	vecItem.pushBack(spItem);

	
	float flyLen = p.y;// 飞行长度 
	float flyVelocity = 150 / 1; //飞行速度
	float realFlyDuration = flyLen / flyVelocity; // 飞行时间
	auto actionMove = MoveTo::create(realFlyDuration, Point(p.x, -50));
	auto actionDone = CallFuncN::create(CC_CALLBACK_1(ItemDrop::removeItem, this));

	Sequence* sequence = Sequence::create(actionMove, actionDone, NULL);
	spItem->runAction(sequence);

	/*if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/bullet.wav");
	}*/
}
void ItemDrop::removeItem(Node *pNode)
{
	if (NULL == pNode)
	{
		return;
	}
	Sprite* spItem = (Sprite*)pNode;
	this->removeChild(spItem, true);
	vecItem.eraseObject(spItem);
}

