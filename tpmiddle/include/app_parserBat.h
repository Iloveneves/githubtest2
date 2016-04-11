#ifndef _APP_PARSERBAT_H_
#define _APP_PARSERBAT_H_

struct batProgInfo_s
{
	BU08 isUsed;
	BU32 serviceId;
	BS08 audioOffset;
	BU08 audioTrack;
	BU32 progLCN;
	struct batProgInfo_s *next;
};

struct batInfo_s
{
	BU08 isUsed;
	BU32 bqId;
	BU08 bqName[APP_CHN_NAME_LEN];
	struct batProgInfo_s *pFirstBatProgInfo;
	struct batInfo_s *next;
};


#endif








