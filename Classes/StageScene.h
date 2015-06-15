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
	StageScene();
	virtual ~StageScene();

    // a selector callback
    void menuBackCallback(Ref* pSender);
    void menuResetCallback(Ref* pSender);
    void menuUndoCallback(Ref* pSender);
    
    bool loadStageData(const StageData* pData);

    virtual bool onTouchBegan(Touch *touch, Event *unused_event) override;
	virtual void onTouchMoved(Touch *touch, Event *unused_event) override;
	virtual void onTouchEnded(Touch *touch, Event *unused_event) override;
	virtual void onTouchCancelled(Touch *touch, Event *unused_event) override;

    // implement the "static create()" method manually
    CREATE_FUNC(StageScene);

protected:
	/** show the victory scene */
	void showVictoryLayer();

protected:
    SudokuBox* m_pBox;
    NumberBar* m_pBar;
    Sprite*	m_pSelectedSprite;
    int m_iSelectedIndex;

    Lock m_lock;
};

#endif // __STAGE_SCENE_H__
