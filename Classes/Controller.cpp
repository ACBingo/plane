
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

		//��xml�ļ��ж�ȡ������ʾ����
		auto dictionary = Dictionary::createWithContentsOfFile("fonts/AboutMe.xml");
		score_label = Label::createWithTTF(
			((__String *)(dictionary->objectForKey("score")))->getCString(),
			"fonts/DFPShaoNvW5-GB.ttf",
			25);
		score_label->setPosition(score_label->getContentSize().width / 2,
			winSize.height - score_label->getContentSize().height * 2);
		CC_BREAK_IF(!score_label);

		this->addChild(score_label);

		//�����ʾ�����ı�ǩ
		m_saveData = SaveData::create();
		//����һ��Ҫretainһ��saveData��������������releaseһ��
		m_saveData->retain();
		auto str = __String::createWithFormat("%d", m_saveData->getScore());
		m_score = Label::createWithTTF(str->getCString(), "fonts/DFPShaoNvW5-GB.ttf", 25);
		m_score->setPosition(Point(score_label->getContentSize().width + m_score->getContentSize().width / 2 + 30,
			winSize.height - score_label->getContentSize().height * 2));
		CC_BREAK_IF(!m_score);

		this->addChild(m_score);
		//���·�������ʾ
		this->scheduleUpdate();

		//��Ϸ�������ư�ť
		Sprite *normalMusic = Sprite::create("others/8.png");
		Sprite *pressedMusic = Sprite::create("others/5.png");
		pMusicItem = MenuItemSprite::create(
			normalMusic,
			normalMusic,
			NULL,
			CC_CALLBACK_1(Controller::menuMusicCallback, this));


		//��Ϸ��ͣ��ť
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

//��Ϸ��ͣ
void Controller::menuPauseCallback(cocos2d::Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/button.mp3");
	if (!Director::getInstance()->isPaused())
	{
		// ͼ��״̬����
		pPauseItem->setNormalImage(Sprite::create("others/3.png"));
		pPauseItem->setSelectedImage(Sprite::create("others/3.png"));
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic(); // ֹͣ��������
		CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects(); // ֹͣ���е���Ч
		Director::getInstance()->pause(); // ֹͣ���еĶ������л����У��ӵ�ǰ����
	}
	else
	{
		pPauseItem->setNormalImage(Sprite::create("others/6.png"));
		pPauseItem->setSelectedImage(Sprite::create("others/6.png"));
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();// �ָ�
		Director::getInstance()->resume(); // �ָ�
	}
}

void Controller::menuMusicCallback(cocos2d::Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/button.mp3");
	if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		// ͼ��״̬����
		pMusicItem->setNormalImage(Sprite::create("others/8.png"));
		pMusicItem->setSelectedImage(Sprite::create("others/8.png"));
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(); // ֹͣ��������
//		CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects(); // ֹͣ���е���Ч
	}
	else
	{
		pMusicItem->setNormalImage(Sprite::create("others/7.png"));
		pMusicItem->setSelectedImage(Sprite::create("others/7.png"));
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/BackgroundMusic.mp3", true);// �ָ�
	}
}


void Controller::update(float tm)
{
	auto str = __String::createWithFormat("%d", m_saveData->getScore());
	//���·���������
	m_score->setColor(Color3B(255, 0, 0));
	m_score->setString(str->getCString());
	m_score->setPositionX(score_label->getContentSize().width + m_score->getContentSize().width / 2 + 30);
}