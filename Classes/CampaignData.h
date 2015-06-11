/*
 * CampaignData.h
 *
 *  Created on: 2015年6月10日
 *      Author: kerlw
 */

#ifndef CAMPAIGNDATA_H_
#define CAMPAIGNDATA_H_

#include <string>
#include <vector>

typedef struct _uuid {
	unsigned int uuid1;
	unsigned int uuid2;
	unsigned int uuid3;
	unsigned int uuid4;
} uuid;

class StageData;

class CampaignData {
public:
	CampaignData();
	virtual ~CampaignData();

	StageData* getNextStageData();

	static CampaignData* loadData(const std::string& path);
	static CampaignData* loadFromFile(const std::string& path);

protected:
	uuid m_uuid;
	std::vector<StageData*> m_vctData;
	int m_iCurrentIndex;
};

//TODO lazy
class LazyLoadCampaignData : public CampaignData {
public:
	LazyLoadCampaignData();
	virtual ~LazyLoadCampaignData();
protected:
	FILE* m_pFile;
};

class DataFileHelper {
public:
	static DataFileHelper* getInstance();

	virtual ~DataFileHelper() {}

	void loadInternalData();
	bool installPackedData();

private:
	DataFileHelper();

private:
	std::string m_strDataPath;
	std::string m_strResPath;
	std::string m_strPackedDataPath;
};

class PackedCampaignProcessor {
public:
	virtual ~PackedCampaignProcessor();

	static PackedCampaignProcessor* createParser(const std::string& path);

	bool unpackTo(const std::string& path);

private:
	PackedCampaignProcessor(FILE* fp);
	FILE* m_pFile;
	unsigned int m_uVersion;
	uuid m_uuid;
};

#endif /* CAMPAIGNDATA_H_ */
