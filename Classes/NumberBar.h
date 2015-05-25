/*
 * NumberBar.h
 *
 *  Created on: 2015年5月22日
 *      Author: kerlw
 */

#ifndef NUMBERBAR_H_
#define NUMBERBAR_H_

#include "cocos2d.h"
#include <vector>
#include "ui/UILayout.h"

USING_NS_CC;

class NumberBar : public ui::Layout {
public:
	NumberBar();
	virtual ~NumberBar();
    /**
     * Create a empty HBox instance.
     * @return A HBox instance pointer.
     */
	static NumberBar* create();

	void attachSprites(int number, SpriteFrameCache* cache);
	int findSpriteIndexAtLocation(Vec2 location);

CC_CONSTRUCTOR_ACCESS:
	 virtual bool init() override;

	std::vector<Rect> m_rectSprites;

private:
	static const float SPRITE_GAP;
};

#endif /* NUMBERBAR_H_ */
