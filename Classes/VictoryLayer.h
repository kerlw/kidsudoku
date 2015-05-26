/*
 * VictoryLayer.h
 *
 *  Created on: 2015年5月26日
 *      Author: kerlw
 */

#ifndef VICTORYLAYER_H_
#define VICTORYLAYER_H_

#include "cocos2d.h"

USING_NS_CC;

class VictoryLayer: public LayerColor {
public:
	VictoryLayer();
	virtual ~VictoryLayer();

	virtual bool init() override;

	void onContinueClicked(Ref* ref);

	CREATE_FUNC(VictoryLayer);

};

#endif /* VICTORYLAYER_H_ */
