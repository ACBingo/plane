#pragma once
#ifndef __ItemDrop_H__
#define __ItemDrop_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC; 

class ItemDrop : public Layer 
{
public:
	ItemDrop();
	~ItemDrop();
	virtual bool init();
	CREATE_FUNC(ItemDrop);

	//�������һ����Ʒ
	void createItem(Point p);

	//
	void removeItem(Node *pNode);
public:
	Vector<Sprite*> vecItem;
};

#endif