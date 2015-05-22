/*
 * NumberBar.h
 *
 *  Created on: 2015年5月22日
 *      Author: kerlw
 */

#ifndef NUMBERBAR_H_
#define NUMBERBAR_H_

#include "cocos2d.h"
#include "ui/UIHBox.h"

class NumberBar : public cocos2d::ui::HBox {
public:
	NumberBar();
	virtual ~NumberBar();
    /**
     * Create a empty HBox instance.
     * @return A HBox instance pointer.
     */
	static NumberBar* create();

	void attachSprites(int number, cocos2d::SpriteFrameCache* cache);

CC_CONSTRUCTOR_ACCESS:
//	 virtual bool init() override;

private:


};

#endif /* NUMBERBAR_H_ */
