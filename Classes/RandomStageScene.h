/*
 * RandomStageScene.h
 *
 *  Created on: 2015年6月23日
 *      Author: kerlw
 */

#ifndef RANDOMSTAGESCENE_H_
#define RANDOMSTAGESCENE_H_

#include "cocos2d.h"
#include <vector>
#include <string>

USING_NS_CC;

struct BoxSizeData {
	std::string desc;
	short rows_per_grid;
	short cols_per_grid;
	short grids_in_row;
	short grids_in_col;
};

class RandomStageScene : public Layer {
public:
	RandomStageScene();
	virtual ~RandomStageScene();

	virtual bool init() override;

	CREATE_FUNC(RandomStageScene);

	void menuDecreaseSizeCallback(Ref* pSender);
	void menuIncreaseSizeCallback(Ref* pSender);
	void menuDecreaseDifficultyCallback(Ref* pSender);
	void menuIncreaseDifficultyCallback(Ref* pSender);
	void menuPrevResourceCallback(Ref* pSender);
	void menuNextResourceCallback(Ref* pSender);
	void menuBackCallback(Ref* pSender);
	void onBtnGoClicked(Ref* pSender);

protected:
	int m_iIndexSize;
	int m_iIndexDifficulty;
	int m_iIndexResource;
	std::vector<std::string> m_vctSize;
	std::vector<std::string> m_vctDifficulty;
	std::vector<std::string> m_vctResource;

	Label* m_pLabelSize;
	Label* m_pLabelDifficulty;
	Label* m_pLabelResource;
};

#endif /* RANDOMSTAGESCENE_H_ */
