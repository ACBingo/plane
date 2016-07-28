
#include "Controller.h"
#include "SimpleAudioEngine.h"

Controller::Controller() : pPauseItem(NULL), pMusicItem(NULL), m_score(NULL), score_label(NULL)
{

}

Controller::~Controller()
{
	m_saveData->release();
}

bool Controller::init()
{
	if (!Layer::init())
	{
		return false;
	}

	bool bRect = false;

	do
	{
		Size winSize = Director::getInstance()->getWinSize();

		//从xml文件中读取中文显示出来
		auto dictionary = Dictionary::createWithContentsOfFile("fonts/AboutMe.xml");
		score_label = Label::createWithTTF(
			((__String *)(dictionary->objectForKey("score")))->getCString(),
			"fonts/DFPShaoNvW5-GB.ttf",
			25);
		score_label->setPosition(score_label->getContentSize().width / 2,
			winSize.height - score_label->getContentSize().height * 2);
		CC_BREAK_IF(!score_label);

		this->addChild(score_label);

		//添加显示分数的标签
		m_saveData = SaveData::create();
		//这里一定要retain一下saveData，在析构函数中release一下
		m_saveData->retain();
		auto str = __String::createWithFormat("%d", m_saveData->getScore());
		m_score = Label::createWithTTF(str->getCString(), "fonts/DFPShaoNvW5-GB.ttf", 25);
		m_score->setPosition(Point(score_label->getContentSize().width + m_score->getContentSize().width / 2 + 30,
			winSize.height - score_label->getContentSize().height * 2));
		CC_BREAK_IF(!m_score);

		this->addChild(m_score);
		//更新分数的显示
		this->scheduleUpdate();

		//游戏声音控制按钮
		Sprite *normalMusic = Sprite::create("others/8.png");
		Sprite *pressedMusic = Sprite::create("others/5.png");
		pMusicItem = MenuItemSprite::create(
			normalMusic,
			normalMusic,
			NULL,
			CC_CALLBACK_1(Controller::menuMusicCallback, this));


		//游戏暂停按钮
		Sprite *normalPause = Sprite::create("others/6.png");
		Sprite *pressedPause = Sprite::create("others/4.png");
		pPauseItem = MenuItemSprite::create(
			normalPause,
			pressedPause,
			NULL,
			CC_CALLBACK_1(Controller::menuPauseCallback, this));

		Menu *menuPause = Menu::create(pMusicItem, pPauseItem, NULL);
		menuPause->alignItemsHorizontallyWithPadding(pPauseItem->getContentSize().width / 2);
		menuPause->setPosition(
			Point(winSize.width - pPauseItem->getContentSize().width * 2, winSize.height - normalPause->getContentSize().height));
		this->addChild(menuPause);

	} while (0);

	return true;
}

//游戏暂停
void Controller::menuPauseCallback(cocos2d::Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/button.mp3");
	if (!Director::getInstance()->isPaused())
	{
		// 图标状态设置
		pPauseItem->setNormalImage(Sprite::create("others/3.png"));
		pPauseItem->setSelectedImage(Sprite::create("others/3.png"));
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic(); // 停止背景音乐
		CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects(); // 停止所有的特效
		Director::getInstance()->pause(); // 停止所有的动作，敌机飞行，子弹前进等
	}
	else
	{
		pPauseItem->setNormalImage(Sprite::create("others/6.png"));
		pPauseItem->setSelectedImage(Sprite::create("others/6.png"));
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();// 恢复
		Director::getInstance()->resume(); // 恢复
	}
}

void Controller::menuMusicCallback(cocos2d::Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/button.mp3");
	if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		// 图标状态设置
		pMusicItem->setNormalImage(Sprite::create("others/8.png"));
		pMusicItem->setSelectedImage(Sprite::create("others/8.png"));
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(); // 停止背景音乐
//		CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects(); // 停止所有的特效
	}
	else
	{
		pMusicItem->setNormalImage(Sprite::create("others/7.png"));
		pMusicItem->setSelectedImage(Sprite::create("others/7.png"));
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/BackgroundMusic.mp3", true);// 恢复
	}
}


void Controller::update(float tm)
{
	auto str = __String::createWithFormat("%d", m_saveData->getScore());
	//更新分数和坐标
	m_score->setColor(Color3B(255, 0, 0));
	m_score->setString(str->getCString());
	m_score->setPositionX(score_label->getContentSize().width + m_score->getContentSize().width / 2 + 30);
}