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
		c = 3;//�ɻ�����
		break;
	case 2:
	case 3:
	//case 4:
	//case 5:
		c = 6;//��õ�����ö
		break;
	case 4:
	case 5:
		c = 7;//�ӵ�����
		break;
	case 6:
	case 7:
	case 8:
	case 9:
		c = 5;//��õ���һö
		break;
	case 10:
	case 11:
	case 12:
	case 13:
		c = 2;//�ӵ�����
		break;
	default:
		c = 1;//��Ѫ
		break;
	}

	s += c + '0';
	s += ".png";
	auto spItem = Sprite::create(s);
	spItem->setTag(c);
	spItem->setPosition(p);

	this->addChild(spItem);
	vecItem.pushBack(spItem);

	
	float flyLen = p.y;// ���г��� 
	float flyVelocity = 150 / 1; //�����ٶ�
	float realFlyDuration = flyLen / flyVelocity; // ����ʱ��
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

