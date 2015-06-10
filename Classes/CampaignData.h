/*
 * CampaignData.h
 *
 *  Created on: 2015年6月10日
 *      Author: kerlw
 */

#ifndef CAMPAIGNDATA_H_
#define CAMPAIGNDATA_H_

class CampaignData {
public:
	CampaignData();
	virtual ~CampaignData();
};

class PackedCampaignDataFileHelper {
public:
	static PackedCampaignDataFileHelper* getInstance();

	virtual ~PackedCampaignDataFileHelper();

private:
	PackedCampaignDataFileHelper();
};

#endif /* CAMPAIGNDATA_H_ */
