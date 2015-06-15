/*
 * GameController.h
 *
 *  Created on: 2015年5月26日
 *      Author: kerlw
 */

#ifndef GAMECONTROLLER_H_
#define GAMECONTROLLER_H_

#include "cocos2d.h"

USING_NS_CC;

class StageScene;
class CampaignData;

class GameController: public Ref {
public:
	enum SceneType {
		eMainScene,
		eCampaignScene,
		eStageScene,
	};
public:
	virtual ~GameController();

	static GameController* getInstance();

	void enterScene(const SceneType& eType);
	void leaveScene();

	StageScene* getStageScene();
	void nextStage();
	void setCampaignData(CampaignData* data) {
		//TODO release old data.
		m_pCampaign = data;
	}

private:
	GameController();
	bool init();

	CampaignData* m_pCampaign;
};

#endif /* GAMECONTROLLER_H_ */
