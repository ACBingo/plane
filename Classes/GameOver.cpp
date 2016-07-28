#include "GameOver.h"

Scene* GameOver::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameOver::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameOver::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	bool bRect = false;

	//背景音乐
	if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/game_over.mp3", true);
	}

	do
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		//添加背景图片
		//auto m_background = Sprite::createWithSpriteFrameName("backgroundGameOver.png");
		auto m_background = Sprite::create("img_bg_logo.jpg");
		m_background->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
		m_background->setAnchorPoint(Vec2(0.5, 0.5));
		CC_BREAK_IF(!m_background);

		this->addChild(m_background);

		//添加分数
		auto score_int = UserDefault::getInstance()->getIntegerForKey("currentScore");
		auto score_str = __String::createWithFormat("%d", score_int);
		auto score = Label::createWithTTF(score_str->getCString(), "fonts/DFPShaoNvW5-GB.ttf", 40);
		score->setPosition(Point(visibleSize.width / 2, visibleSize.height / 3 * 2));
		score->setColor(Color3B(255, 0, 0));
		CC_BREAK_IF(!score);

		this->addChild(score);

		//设定等级

		//设置标签 并 获取中文文本
		auto dictionary = Dictionary::createWithContentsOfFile("fonts/AboutMe.xml");
		String rank_str;

		switch (score_int / 1000)
		{
		case 0:
			rank_str = ((__String*)(dictionary->objectForKey("Eleven")))->getCString();
			break;
		case 1:
			rank_str = ((__String*)(dictionary->objectForKey("Ten")))->getCString();
			break;
		case 2:
			rank_str = ((__String*)(dictionary->objectForKey("Nine")))->getCString();
			break;
		case 3:
			rank_str = ((__String*)(dictionary->objectForKey("Eight")))->getCString();
			break;
		case 4:
			rank_str = ((__String*)(dictionary->objectForKey("Seven")))->getCString();
			break;
		case 5:
			rank_str = ((__String*)(dictionary->objectForKey("Six")))->getCString();
			break;
		case 6:
			rank_str = ((__String*)(dictionary->objectForKey("Five")))->getCString();
			break;
		case 7:
			rank_str = ((__String*)(dictionary->objectForKey("Four")))->getCString();
			break;
		case 8:
			rank_str = ((__String*)(dictionary->objectForKey("Three")))->getCString();
			break;
		case 9:
			rank_str = ((__String*)(dictionary->objectForKey("Two")))->getCString();
			break;
		case 10:
			rank_str = ((__String*)(dictionary->objectForKey("One")))->getCString();
			break;
		default:
			rank_str = ((__String*)(dictionary->objectForKey("Zere")))->getCString();
			break;
		};

		auto m_label1 = Label::createWithTTF(
			rank_str.getCString(),
			"fonts/DFPShaoNvW5-GB.ttf",
			65
			);
		m_label1->setColor(Color3B(255, 0, 0));
		m_label1->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - m_label1->getContentSize().height));

		this->addChild(m_label1);


		/////////////////////////////
		// 2. add a menu item with "X" image, which is clicked to quit the program
		//    you may modify it.

		//退出游戏 按钮
		//auto tempClose1 = Sprite::createWithSpriteFrameName("GameOver_nor.png");
		//auto tempClose2 = Sprite::createWithSpriteFrameName("GameOver_touched.png");
		auto tempClose1 = Sprite::create("others/back_peek3.png");
		auto tempClose2 = Sprite::create("others/back_peek3.png");

		auto closeItem = MenuItemSprite::create(
			tempClose1, tempClose2, CC_CALLBACK_1(GameOver::menuCloseCallback, this)
			);

		//返回主界面 按钮
		//auto tempBack1 = Sprite::createWithSpriteFrameName("ReturnGame_nor.png");
		//auto tempBack2 = Sprite::createWithSpriteFrameName("ReturnGame_touched.png");
		auto tempBack1 = Sprite::create("others/back_peek2.png");
		auto tempBack2 = Sprite::create("others/back_peek2.png");
		auto backItem = MenuItemSprite::create(
			tempBack1, tempBack2, CC_CALLBACK_1(GameOver::menuMainCallback, this)
			);

		// create menu, it's an autorelease object
		auto menu = Menu::create(closeItem, backItem, NULL);
		menu->alignItemsVerticallyWithPadding(closeItem->getContentSize().height / 2);
		menu->setPosition(Vec2(origin.x + visibleSize.width / 2, visibleSize.height / 4));
		CC_BREAK_IF(!menu);

		this->addChild(menu, 1);

		bRect = true;
	} while (0);

	/////////////////////////////
	// 3. add your codes below...


	return true;
}

// 退出游戏
void GameOver::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

// 返回主界面
void GameOver::menuMainCallback(cocos2d::Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	Director::getInstance()->replaceScene(TransitionProgressRadialCCW::create(0.8f, MainScene::createScene()));
}