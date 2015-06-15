/*
 * CampaignData.cpp
 *
 *  Created on: 2015年6月10日
 *      Author: kerlw
 */

#include "CampaignData.h"
#include "cocos2d.h"
#include "Const.h"
#include "StageData.h"
#include "GameController.h"

USING_NS_CC;

static DataFileHelper* s_pDataFileHelper = nullptr;
const uuid CampaignData::INTERNAL_CAMPAIGN_UUID = {0,0,0,0};

CampaignData* CampaignData::loadData(const std::string& path) {
	auto campaign = new (std::nothrow) CampaignData();
	/**
	 * For android, the 3rd parameter of FileUtils::getFileData(...) could be nullptr,
	 * but for linux, it could not be nullptr, suck!
	 */
	ssize_t size;
	unsigned char* data = FileUtils::getInstance()->getFileData(path, "rb", &size);
	if (!data)
		return campaign;

	int pos = 0;
	pos = 4 + 4;	// TODO version, resource
	int count = *((int*)(data + pos));
	pos += 4;
//	log("there are %d stages in this file.", count);
	for (int i = 0; i < count; i++) {
			auto stage = new (std::nothrow) StageData();
			stage->rows_per_grid = data[pos++];
			stage->cols_per_grid = data[pos++];
			stage->grids_in_row = data[pos++];
			stage->grids_in_col = data[pos++];
			stage->numbers = stage->cols_per_grid * stage->rows_per_grid;

			int cells = stage->numbers * stage->grids_in_col * stage->grids_in_row;
			stage->cell_data = new int[cells];
			memset(stage->cell_data, 0, sizeof(int) * cells);

			cells = (data[pos++] | data[pos++] << 8);
//			log(" has %d known cells", cells);
			for (int j = 0; j < cells; j++) {
				stage->cell_data[data[pos++]] = data[pos++];
			}

			campaign->m_vctData.push_back(stage);
		}
		campaign->m_iCurrentIndex = 0;

		return campaign;
}

CampaignData* CampaignData::loadFromFile(const std::string& path) {
	auto campaign = new (std::nothrow) CampaignData();
	FILE* fp = fopen(path.c_str(), "rb");
	if (!fp)
		return campaign;

	int tmp = 0;
	fread(&tmp, 4, 1, fp);	//version
	fread(&tmp, 4, 1, fp);	//specify resource

	fread(&tmp, 4, 1, fp);
	int count = tmp;
	log("there are %d stages in this file.", count);
	for (int i = 0; i < count; i++) {
		auto stage = new (std::nothrow) StageData();
		fread(&tmp, 4, 1, fp);

		stage->rows_per_grid = (tmp & 0x0ff);
		stage->cols_per_grid = ((tmp >> 8) & 0x0ff);
		stage->grids_in_row = ((tmp >> 16) & 0x0ff);
		stage->grids_in_col = ((tmp >> 24) & 0x0ff);
		stage->numbers = stage->cols_per_grid * stage->rows_per_grid;

		int cells = stage->numbers * stage->grids_in_col * stage->grids_in_row;
		stage->cell_data = new int[cells];
		memset(stage->cell_data, 0, sizeof(int) * cells);

		tmp = 0;
		fread(&tmp, 2, 1, fp);
		cells = tmp;
		log(" has %d known cells", cells);
		for (int j = 0; j < cells; j++) {
			tmp = 0;
			fread(&tmp, 2, 1, fp);
			stage->cell_data[(tmp & 0xff)] = ((tmp >> 8) & 0xff);
		}

		campaign->m_vctData.push_back(stage);
	}
	campaign->m_iCurrentIndex = 0;

	fclose(fp);
	return campaign;
}

CampaignData::CampaignData()
	: m_iCurrentIndex(-1) {
}

CampaignData::~CampaignData() {
	auto it = m_vctData.begin();
	while (it != m_vctData.end()) {
		delete *it;
	}
	m_vctData.clear();
}

StageData* CampaignData::currentStageData() {
	if (m_iCurrentIndex < 0)
		m_iCurrentIndex = 0;
	if (m_iCurrentIndex >= m_vctData.size())
		return nullptr;

	return m_vctData[m_iCurrentIndex];
}

StageData* CampaignData::getNextStageData() {
	if (m_iCurrentIndex < 0)
		m_iCurrentIndex = 0;
	if (m_iCurrentIndex >= m_vctData.size())
		return nullptr;

	return m_vctData[m_iCurrentIndex++];
}


DataFileHelper* DataFileHelper::getInstance() {
	if (!s_pDataFileHelper) {
		s_pDataFileHelper = new (std::nothrow) DataFileHelper();
	}
	return s_pDataFileHelper;
}

DataFileHelper::DataFileHelper() {
    FileUtils* futils = FileUtils::getInstance();

    std::string path = UserDefault::getInstance()->getStringForKey(KEYNAME_EXTERNAL_DATA_PATH);
	if (path.length() == 0) {
		path = futils->getWritablePath();
	}
    log("external data path is %s", m_strDataPath.c_str());

    m_strDataPath = path + "/" + PATH_DATA;
    m_strResPath = path + "/" + PATH_RESOURCE;
    m_strPackedDataPath = path + "/" + PATH_PACKEDDATA;

    if (!futils->isDirectoryExist(m_strPackedDataPath))
    	futils->createDirectory(m_strDataPath);
    if (!futils->isDirectoryExist(m_strResPath))
    	futils->createDirectory(m_strResPath);
    if (!futils->isDirectoryExist(m_strPackedDataPath))
    	futils->createDirectory(m_strPackedDataPath);
}

void DataFileHelper::loadInternalData() {
	FileUtils* futils = FileUtils::getInstance();
	const std::string path = futils->fullPathForFilename("data/campaign.dat");
	log("data path is %s", path.c_str());
	if (!futils->isFileExist(path))
		return;

	auto campaign = CampaignData::loadData(path);
	GameController::getInstance()->setCampaignData(campaign);
}

bool DataFileHelper::installPackedData() {
	bool ret = false;

	FileUtils* futils = FileUtils::getInstance();
	std::string path = m_strPackedDataPath + "campaign.pcd";
	std::string outPath = m_strDataPath + "/campaign.dat";	//TODO use uuid as file name

	//TODO move these into a installer class, notify progress to UI
	auto parser = PackedCampaignProcessor::createParser(path);
	if (!parser)
		return false;

	parser->unpackTo(outPath);
	return true;
}

PackedCampaignProcessor* PackedCampaignProcessor::createParser(const std::string& path) {
	FILE* fp = fopen(path.c_str(), "rb");
	if (fp == nullptr)
		return nullptr;

	int tmp = 0;
	uuid id;
	fread(&tmp, 4, 1, fp);		//length
	fread(&tmp, 4, 1, fp);		//version
	//XXX construct different parser according to the version
	auto ret = new PackedCampaignProcessor(fp);
	ret->m_uVersion = tmp;
	fread(&ret->m_uuid, sizeof(uuid), 1, fp);	//uuid
	return ret;
}

PackedCampaignProcessor::PackedCampaignProcessor(FILE* fp)
	: m_pFile(fp) {
}

PackedCampaignProcessor::~PackedCampaignProcessor() {
	if (m_pFile)
		fclose(m_pFile);
}

bool PackedCampaignProcessor::unpackTo(const std::string& path) {
	FILE* fp = fopen(path.c_str(), "wb");
	if (!fp)
		return false;
	fwrite(&m_uVersion, 4, 1, fp);

	unsigned int tmp;
	fseek(m_pFile, 24, SEEK_SET);	//XXX 24 = 4+4+16 should not hard code here.

	fread(&tmp, 4, 1, m_pFile);		//specify resource
	if (tmp > 0)
		fseek(m_pFile, tmp, SEEK_CUR);
	tmp = 0;
	fwrite(&tmp, 4, 1, fp);

	//stage count
	fread(&tmp, 4, 1, m_pFile);
	fwrite(&tmp, 4, 1, fp);

	//deal with stage data
	int count = tmp;
	fseek(m_pFile, count * 4, SEEK_CUR);	//skip offset array
	//TODO add offset array into data file

	char solution[MAX_CELLS] = {0};
	char known[MAX_CELLS] = {0};
	for (int i = 0; i < count; i++) {
		fseek(m_pFile, 4+4+4, SEEK_CUR);	//TODO: length, id, specify resource

		//sudoku grid size & box size
		fread(&tmp, 4, 1, m_pFile);
		fwrite(&tmp, 4, 1, fp);

		int cells = (tmp & 0x0FF) * ((tmp >> 8) & 0x0FF) * ((tmp >> 16) & 0x0FF) * ((tmp >> 24) & 0x0FF);
		tmp = 0;
		fread(&tmp, 2, 1, m_pFile);
		//TODO check if the calculated cells count equals the read out count.
		cells = tmp;
		if (cells > MAX_CELLS) {
			fclose(fp);
			return false;
		}
		fread(solution, 1, cells, m_pFile);

		tmp = 0;
		fread(&tmp, 2, 1, m_pFile);
		cells = tmp;
		fread(known, 1, cells, m_pFile);

		fwrite(&cells, 2, 1, fp);
		for (int j = 0; j < cells; j++) {
			fwrite(known + j, 1, 1, fp);
			fwrite(solution + known[j], 1, 1, fp);
		}
	}

	fclose(fp);
	return true;
}

bool operator==(const uuid& src, const uuid& dest) {
	return (src.uuid1 == dest.uuid1) && (src.uuid2 == dest.uuid2)
			&& (src.uuid3 == dest.uuid3) && (src.uuid4 == dest.uuid4);
}
