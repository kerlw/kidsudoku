/*
 * CampaignScene.h
 *
 *  Created on: 2015年6月15日
 *      Author: kerlw
 */

#ifndef CAMPAIGNSCENE_H_
#define CAMPAIGNSCENE_H_

#include "cocos2d.h"
#include "RealTableView.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CampaignData;
class CampaignDataSource;

class CampaignScene : public Layer {
public:
	CampaignScene(CampaignData* data);
	virtual ~CampaignScene();

	virtual bool init() override;

	static CampaignScene* create(CampaignData* data);

	void menuBackCallback(Ref* pSender);

	virtual void onEnter() override {
		Layer::onEnter();
		m_pTableStages->reloadData();
	}

	//virtual void tableCellTouched(RealTableView* table, TableViewCell* cell) override;
private:
	CampaignData* m_pData;
	CampaignDataSource* m_pSource;
	RealTableView* m_pTableStages;
};

class CampaignDataSource : public RealTableViewDataSource {
public:
	CampaignDataSource(CampaignData* data);

	virtual Size cellSizeForTable(RealTableView *table) override {
		return Size(120, 120);
	};

	virtual TableViewCell* tableCellAtIndex(RealTableView *table, ssize_t idx) override;
	virtual ssize_t numberOfCellsInTableView(RealTableView *table) override;

	void onBtnStageClicked(Ref* pSender);

private:
	CampaignData* m_pData;
};

#endif /* CAMPAIGNSCENE_H_ */
