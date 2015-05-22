#include "StageScene.h"
#include "StageData.h"
#include "NumberBar.h"
#include "Const.h"

USING_NS_CC;

Scene* StageScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = StageScene::create();

    //TODO stage data should load from file
    StageData data;
    data.plt_file = "animals.plist";
    data.res_file = "animals.png";
    data.numbers = 3;

    layer->loadStageData(&data);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
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
    
    auto director = Director::getInstance();
	auto pView = director->getOpenGLView();
	pView->setDesignResolutionSize(1280, 720, kResolutionShowAll);
	director->setOpenGLView(pView);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(StageScene::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    return true;
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

	Size visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();

	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile(pData->plt_file, pData->res_file);

	auto bar = NumberBar::create();
	bar->attachSprites(pData->numbers, cache);
	bar->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(bar, 0);

	return true;
}
