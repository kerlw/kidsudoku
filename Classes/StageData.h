/*
 * StageData.h
 *
 *  Created on: 2015年5月21日
 *      Author: kerlw
 */

#ifndef STAGEDATA_H_
#define STAGEDATA_H_

#include <string>

class StageData {
public:
	std::string plt_file;
	std::string res_file;
	/** count of number */
	int numbers;
	/** count of cell*/
	int cells;
};

#endif /* STAGEDATA_H_ */
