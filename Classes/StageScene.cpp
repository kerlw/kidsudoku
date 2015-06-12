#include "StageScene.h"
#include "StageData.h"
#include "NumberBar.h"
#include "SudokuBox.h"
#include "Const.h"

#include "VictoryLayer.h"

USING_NS_CC;

//Scene* StageScene::createScene()
//{
//    // 'scene' is an autorelease object
//    auto scene = Scene::create();
//
//    // 'layer' is an autorelease object
//    auto layer = StageScene::create();
//
//    //TODO stage data should load from file
//
//    auto data = StageDataManager::getInstance()->getNextStageData();
//    layer->loadStageData(data);
//
//    // add layer as a child to scene
//    scene->addChild(layer);
//
//    // return the scene
//    return scene;
//}

StageScene::StageScene() :
		m_pBar(nullptr),
		m_pSelectedSprite(nullptr),
		m_iSelectedIndex(-1)
{
//	this->setTouchEnabled(true);
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(StageScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(StageScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(StageScene::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(StageScene::onTouchCancelled, this);

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

StageScene::~StageScene() {
}

// on "init" you need to initialize your instance
bool StageScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    return true;
}


void StageScene::menuResetCallback(Ref* pSender) {
	if (m_pBox)
		m_pBox->reset();
}

void StageScene::menuUndoCallback(Ref* pSender) {
	if (m_pBox)
		m_pBox->undo();
}

void StageScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

bool StageScene::loadStageData(const StageData* pData) {
	if (pData == NULL)
		return false;

	auto director = Director::getInstance();
	float ratio = StageDataHelper::calcScaleRatio(*pData);
	auto pView = director->getOpenGLView();
	pView->setDesignResolutionSize(1280*ratio, 720*ratio, ResolutionPolicy::SHOW_ALL);
	director->setOpenGLView(pView);

	Size visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();
	log("visibleSize is %f, %f, origin is %f, %f", visibleSize.width, visibleSize.height, origin.x, origin.y);

	auto cache = SpriteFrameCache::getInstance();
	if (pData->plt_file.length() > 0) {
		cache->removeSpriteFrames();
		cache->addSpriteFramesWithFile(pData->plt_file, pData->res_file);
		cache->addSpriteFramesWithFile("fruits.plist");	//TODO hard code fruits as default res
	}

	//add Number Bar to scene
	m_pBar = NumberBar::create();
	m_pBar->attachSprites(pData->numbers, cache);
	m_pBar->setPosition(Vec2(visibleSize.width/2 + origin.x, origin.y));
	this->addChild(m_pBar, 0);

	//add Sudoku Box to scene
	m_pBox = SudokuBox::create();
	m_pBox->initWithStageData(*pData);
	m_pBox->setPosition(Vec2(visibleSize.width/2 + origin.x, (visibleSize.height + CELL_SIZE)/2 + origin.y));
	this->addChild(m_pBox);

	//reset menu item
	auto resetItem = MenuItemImage::create("reset.png", "reset.png",
											CC_CALLBACK_1(StageScene::menuResetCallback, this));
	resetItem->setPosition(Vec2(origin.x + resetItem->getContentSize().width/2,
							 origin.y + visibleSize.height - resetItem->getContentSize().height/2));

	//undo menu item
	auto undoItem = MenuItemImage::create("undo.png", "undo.png",
											CC_CALLBACK_1(StageScene::menuUndoCallback, this));
	undoItem->setPosition(Vec2(origin.x + undoItem->getContentSize().width/2,
							 origin.y + visibleSize.height - CELL_SIZE - undoItem->getContentSize().height/2));

	//close menu item
    auto closeItem = MenuItemImage::create("close.png", "close.png",
                                           CC_CALLBACK_1(StageScene::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, resetItem, undoItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	return true;
}

bool StageScene::onTouchBegan(Touch *touch, Event *unused_event) {
	AutoLock lock(m_lock);
	//if has a sprite selected, ignore this touch event
	if (m_pSelectedSprite) {
		return false;
	}

	Vec2 location = touch->getLocation();
	if (m_pBar) {
		int index = m_pBar->findSpriteIndexAtLocation(m_pBar->convertToNodeSpace(location));
		if (index < 0) {
			return false;
		}

		//one sprite selected, record the index, and add a dragable sprite to scene
		m_iSelectedIndex = index;
		m_pSelectedSprite = Sprite::createWithSpriteFrame(
				SpriteFrameCache::getInstance()->getSpriteFrameByName(FRAME_NAME[index])
				);
		m_pSelectedSprite->setPosition(location);

		this->addChild(m_pSelectedSprite, 0);
	}

	return true;
}

void StageScene::onTouchMoved(Touch *touch, Event *unused_event) {
	AutoLock lock(m_lock);
	//touch moved, refresh the position of selected sprite.
	if (m_pSelectedSprite)
		m_pSelectedSprite->setPosition(touch->getLocation());
}

void StageScene::onTouchEnded(Touch *touch, Event *unused_event) {
	AutoLock lock(m_lock);
	if (m_pSelectedSprite && m_pBox) {
		//touch up, insert a item in SudokuBox in need.
		m_pBox->onItemDragedAtPoint(m_pBox->convertToNodeSpace(touch->getLocation()), m_iSelectedIndex);

		//check if this stage finished
		if (m_pBox->checkResult()) {
			this->showVictoryLayer();
		} else {
			m_pBox->refreshErrorTipsLayer();
		}

		this->removeChild(m_pSelectedSprite);
		m_pSelectedSprite = nullptr;
		m_iSelectedIndex = -1;
	}
}

void StageScene::onTouchCancelled(Touch *touch, Event *unused_event) {
	AutoLock lock(m_lock);
	if (m_pSelectedSprite) {
		//remove dragable sprite and clear the selected sprite index.
		this->removeChild(m_pSelectedSprite);
		m_pSelectedSprite = nullptr;
		m_iSelectedIndex = -1;
	}
}

void StageScene::showVictoryLayer() {
	auto director = Director::getInstance();
	Size visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();

    auto layer = VictoryLayer::create();
    layer->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(layer, 2);
}
