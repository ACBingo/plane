#include "MainScene.h"
#include "GameScene.h"
USING_NS_CC;

Scene* MainScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MainScene::create();
	scene->addChild(layer);
	return scene;
}

bool MainScene::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bksprite = Sprite::create("img_bg_logo.jpg");//背景图片
	bksprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bksprite);

	//add menu
	auto gameItem = MenuItemFont::create("StartGame", CC_CALLBACK_1(MainScene::menuCallback, this));
	auto helpItem = MenuItemFont::create("Help", CC_CALLBACK_1(MainScene::menuCallback, this));
	auto aboutItem = MenuItemFont::create("About", CC_CALLBACK_1(MainScene::menuCallback, this));
	gameItem->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	gameItem->setColor(Color3B::BLACK);
	helpItem->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 40));
	helpItem->setColor(Color3B::BLACK);
	aboutItem->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 80));
	aboutItem->setColor(Color3B::BLACK);
	gameItem->setTag(11);//游戏菜单
	helpItem->setTag(12);//帮助菜单
	aboutItem->setTag(13);//关于菜单
	auto menu = Menu::create(gameItem, helpItem, aboutItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	//menu end

	return true;
}
void MainScene::myupdate(float f) {
	auto sp = this->getChildren();
	for (auto a : sp)
	{
		a->setPosition(a->getPosition().x, a->getPosition().y - 2);
	}
}
void MainScene::menuCallback(Ref* pSender)//菜单回调函数
{
	MenuItem * nowItem = (MenuItem *)pSender;
	switch (nowItem->getTag())
	{
	case 10:
	{

		break;
	}
	case 11:
	{
		//通过修改Transition函数达到切换效果
		Director::getInstance()->replaceScene(TransitionFadeUp::create(1, GameScene::createScene()));
		break;
	}
	case 12:
	{
		break;
	}
	case 13:
	{
		break;
	}
	}
}