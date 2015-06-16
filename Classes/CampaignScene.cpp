/*
 * CampaignScene.cpp
 *
 *  Created on: 2015年6月15日
 *      Author: kerlw
 */

#include "CampaignScene.h"
#include "GameController.h"
#include "CampaignData.h"
#include "RealTableView.h"
#include "ui/UIImageView.h"

#include <string>

CampaignScene* CampaignScene::create(CampaignData* data) {
    auto *pRet = new (std::nothrow) CampaignScene(data);
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

CampaignScene::CampaignScene(CampaignData* data) 
		: m_pData(data) {
	m_pSource = new CampaignDataSource(data);
}

CampaignScene::~CampaignScene() {
	CC_SAFE_DELETE(m_pSource);
}

bool CampaignScene::init() {
	if (!Layer::init())
		return false;

	auto director = Director::getInstance();
	Size visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();

	//stage list
	m_pTableStages = RealTableView::create(m_pSource, Size(960, 600));
	m_pTableStages->ignoreAnchorPointForPosition(false);
	m_pTableStages->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pTableStages->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
	this->addChild(m_pTableStages);

	//back menu item
    auto backItem = MenuItemImage::create("back.png", "back.png",
                                           CC_CALLBACK_1(CampaignScene::menuBackCallback, this));

	backItem->setPosition(Vec2(origin.x + backItem->getContentSize().width/2 + 10,
                                origin.y + visibleSize.height - 10 - backItem->getContentSize().height/2));


    // create menu, it's an autorelease object
    auto menu = Menu::create(backItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    return true;
}

void CampaignScene::menuBackCallback(Ref* pSender) {
	GameController::getInstance()->leaveScene();
}

CampaignDataSource::CampaignDataSource(CampaignData* data)
				: m_pData(data) {
}

TableViewCell* CampaignDataSource::tableCellAtIndex(RealTableView *table, ssize_t idx) {
	auto cell = TableViewCell::create();
	cell->setIdx(idx);

	char buff[10] = {0};
	sprintf(buff, "%d", idx+1);
	std::string title = buff;

	auto btn = ui::Button::create("btn-stage-0.png", "btn-stage-1.png", "btn-stage-2.png");
	btn->setTitleText(title);
	btn->setTitleFontSize(40);
	btn->setContentSize(Size(120, 120));
	btn->setPosition(Vec2(60, 60));
	btn->addClickEventListener(CC_CALLBACK_1(CampaignDataSource::onBtnStageClicked, this));

	cell->addChild(btn);

	if (m_pData->isStageDone(idx)) {
		log("%d is done", idx);
		auto doneImg = ui::ImageView::create("finished.png");
		doneImg->setPosition(Vec2(120 - doneImg->getContentSize().width / 2, doneImg->getContentSize().height / 2));
		cell->addChild(doneImg, 1);
	}

	return cell;
}

ssize_t CampaignDataSource::numberOfCellsInTableView(RealTableView *table) {
	return m_pData->getStageCount();
}

void CampaignDataSource::onBtnStageClicked(Ref* pSender) {
	ui::Widget* widget = dynamic_cast<ui::Widget*>(pSender);
	if (!widget)
		return;

	TableViewCell* cell = dynamic_cast<TableViewCell*>(widget->getParent());
	if (!cell)
		return;

	long index = cell->getIdx();
	m_pData->setCurrentIndex(index);

	GameController::getInstance()->enterScene(GameController::eStageScene);
}

