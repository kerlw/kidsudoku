#ifndef __STAGE_SCENE_H__
#define __STAGE_SCENE_H__

#include "cocos2d.h"
#include "Lock.h"

USING_NS_CC;

class StageData;
class NumberBar;
class SudokuBox;

class StageScene : public Layer
{
public:
	StageScene(StageData* data);
	virtual ~StageScene();

    // a selector callback
    void menuBackCallback(Ref* pSender);
    void menuResetCallback(Ref* pSender);
    void menuUndoCallback(Ref* pSender);
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event) override;
	virtual void onTouchMoved(Touch *touch, Event *unused_event) override;
	virtual void onTouchEnded(Touch *touch, Event *unused_event) override;
	virtual void onTouchCancelled(Touch *touch, Event *unused_event) override;

	virtual bool init() override;

    // implement the "static create()" method manually
    static StageScene* create(StageData* data);

protected:
	/** show the victory scene */
	void showVictoryLayer();

protected:
	StageData* m_pData;

    SudokuBox* m_pBox;
    NumberBar* m_pBar;
    Sprite*	m_pSelectedSprite;
    int m_iSelectedIndex;
    float m_fScaleRatio;

    Lock m_lock;
};

#endif // __STAGE_SCENE_H__
