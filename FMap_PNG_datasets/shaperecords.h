#ifndef SHAPERECORDS_H
#define SHAPERECORDS_H

#include <string>
#include <vector>
//#include "E:\FIUWork\NewFreamwork\Mike\nfmap\shapefil.h"
#include "shapefil.h"
#include "TransInfoManager.h"
#include "ColorManager.h"



class ShapeRecords
{
	std::vector<std::pair<int, SHPObject*> > records;
	std::string shpBaseName;
	TransInfo transInfo;
	OverridedColor colorInfo;
	bool bTransInfo;
	bool bColorInfo;
	int shpType;

	bool HasHLCenter;
	
public:
	ShapeRecords(): bTransInfo(false), bColorInfo(false), HasHLCenter(false){}
	
	std::string getShpBaseName() {return shpBaseName;}
	void setShpBaseName(std::string name) { shpBaseName = name; }
	
	TransInfo getTransInfo() { return transInfo; }
	void setTransInfo(TransInfo info) { bTransInfo = true; transInfo = info; }
	
	OverridedColor getColorInfo() { return colorInfo; }
	void setColorInfo(OverridedColor info) { bColorInfo = true; colorInfo = info; }

	int getShpType() { return shpType; }
	void setShpType(int type) { shpType = type; }
	
	bool hasTransInfo() { return bTransInfo; }
	bool hasColorInfo() { return bColorInfo; }
	
	void push_record(int idx, SHPObject* obj)
	{
		records.push_back(std::make_pair(idx, obj));
	}
	std::pair<int, SHPObject*> getRecord(int idx)
		{
			return records[idx];
		}
		
	int size() { return (int) (records.size()); }

	int destroyRecords()
	{
		for(int i = 0; i < (int)(records.size()); i++)
			::SHPDestroyObject(records[i].second);

		return 0;
	}

	void SetHighlightCenterInfo( bool hl)
	{
		this->HasHLCenter = hl;
	}

	bool GetHighlightCenterInfo()
	{
		return this->HasHLCenter;
	}
};

#endif