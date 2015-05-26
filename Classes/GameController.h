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

class GameController: public Ref {
public:
	virtual ~GameController();

	static GameController* getInstance();

	StageScene* getStageScene();

	void nextStage();
private:
	GameController();
	bool init();
};

#endif /* GAMECONTROLLER_H_ */
