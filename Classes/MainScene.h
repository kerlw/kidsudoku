/*
 * MainScene.h
 *
 *  Created on: 2015年6月15日
 *      Author: kerlw
 */

#ifndef MAINSCENE_H_
#define MAINSCENE_H_

#include "cocos2d.h"

USING_NS_CC;

class MainScene: public Layer {
public:
	MainScene();
	virtual ~MainScene();

	CREATE_FUNC(MainScene);
	virtual bool init() override;

	void onMenuCampaignClicked(Ref* pSender);
	void onMenuExitClicked(Ref* pSender);
protected:
};

#endif /* MAINSCENE_H_ */
