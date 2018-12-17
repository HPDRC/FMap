#include "LayerPriority.h"
#include <vector>


CLayerPriority::CLayerPriority(void)
{

	/*std::vector<CString> layerList;
	layerList.resize(20);
	*/

	this->layerTable.SetAt("WM_WORLD_NATIONS", 1);
	this->layerTable.SetAt("WM_WORLD_ADMIN", 2);
	this->layerTable.SetAt("WM_GREAT_LAKES", 3);
	this->layerTable.SetAt("WM_WATER_POLY", 4);
	this->layerTable.SetAt("WM_GLACIER", 5);
	this->layerTable.SetAt("WM_MAJOR_ROUTES", 6);
	this->layerTable.SetAt("WM_MINOR_ROUTES", 7);
    this->layerTable.SetAt("WM_Railroad_TRACK", 8);
	this->layerTable.SetAt("WM_UTILITIES", 9);
	this->layerTable.SetAt("WM_PHYSIOGRAPHY", 10);
	this->layerTable.SetAt("WM_MOUNTAINS", 11);
	this->layerTable.SetAt("WM_AIRPORTS", 12);
	this->layerTable.SetAt("WM_RAILWAY_STATIONS", 13);
	this->layerTable.SetAt("WM_MARINE_PORTS", 14);
	this->layerTable.SetAt("WM_CITIES_75K_TO_900K", 15);
	this->layerTable.SetAt("WM_CITIES_Greater_900K", 16);
	this->layerTable.SetAt("WM_CITIES_Unknownpop", 17);
	this->layerTable.SetAt("WM_CAPITALS", 18);
	this->layerTable.SetAt("WM_SEAS_BAYS", 19);
	this->layerTable.SetAt("WM_GREAT_LAKES_NAME", 101);
	this->layerTable.SetAt("WM_WORLD_AMIN_NAME", 102);
	this->layerTable.SetAt("NATIONS_NAME", 103);
	//this->layerTable.SetAt("wm_Capitals", 23);

}


bool CLayerPriority::ReorderLayer(Overlays& layers, CString& newLayers)
{
	//make sure that number of layers > 1


	CString strLayer = layers.getNextOverlay().c_str();	
	//CString name;
	int priority;

	while(strLayer!="")
	{


		//add to list
		//to check priority
		CString name = this->req.GetTableName(strLayer);
		if(this->layerTable.Lookup(name.MakeUpper(),priority))
		{
			InsertToPriorityList(strLayer,priority);
		}
		else
		{
			//100 means no Prority
			PrioLayer nonPrioLayer(strLayer, NONPRIORITY);
			list.push_back(nonPrioLayer);
		}

		strLayer = layers.getNextOverlay().c_str();	
	}

	CString plus = "+";
	this->cur=list.begin();
	newLayers = cur->first;
	cur++;
	while(this->cur!=list.end())
	{
		newLayers = newLayers + plus + cur->first;
		cur++;
	}

	return true;
}


bool CLayerPriority::InsertToPriorityList(CString& str, int priority)
{

	if(str=="" || priority<0)
	{
		return false;
	}

	PrioLayer layer(str, priority);

	if(list.size()==0)
	{
		this->list.push_back(layer);
		return true;
	}

	//for(int i=0;i<size-1;++i)
	for(this->cur=this->next=list.begin(),tail=list.end(),tail--;next!=list.end();++cur)
	{
		if(list.size()==1)
		{
			if(cur->second<layer.second)
				list.push_back(layer);
			else
				list.push_front(layer);

			return true;
		}
		else
		{
			//move to next
			next++;
			if(cur->second<=layer.second && next->second>layer.second)
			{
				//insert to the pos behind the cur
				list.insert(next,layer);
				//cur to this one
				//cur++;
				return true;
			}
			
			if(tail->second<=layer.second)
			{
				list.push_back(layer);
				return true;
			}

		}

	}


	


	return false;


}

CLayerPriority::~CLayerPriority(void)
{
	this->layerTable.RemoveAll();
	this->list.clear();
}
