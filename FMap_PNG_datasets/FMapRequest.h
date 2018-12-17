#ifndef FMAPREQUEST_H
#define FMAPREQUEST_H

#include <vector>
#include <list>
#include <string>


class Pair
{
	std::pair<std::string, std::string> requestPair;
public:
	Pair(){}
	/*Pair(const Pair& p)
	{
		requestPair = makepair(p.get);
	}*/
	Pair(std::string);
	std::string getValue();
	std::string getName();
};

class RequestString 
{
	std::vector<Pair> request;
	static bool binPred(char a, char b)
	{
		return toupper(a) == toupper(b);
	}
public:
	RequestString(){}
	RequestString(const char* requestString);
	std::string getValue(std::string);
	
};

class Overlays
{
	std::list<std::string> overlays;
	std::list<std::string>::iterator cur;
public:
	Overlays(){}
	Overlays(std::string);
	std::string getNextOverlay();
	void remove(std::string aOverlay) { overlays.remove(aOverlay); }
};


class Rule
{
	std::string overlay;
	std::pair<double, double> range;
public:
	Rule(){}
	Rule(std::string aOverlay, std::pair<double,double> aRange):overlay(aOverlay), range(aRange) {}

	friend bool operator==(const Rule& r1, const Rule& r2);

	virtual void apply(Overlays& overlays, double res);
	virtual ~Rule(){};
};

class CompositeRule : public Rule
{
	std::list<Rule> rules;
public:
	CompositeRule(){};
	void add(Rule& rule){ rules.push_back(rule); }
	void remove(Rule& rule)
	{
		rules.remove(rule);
	}

	void apply(Overlays& overlays, double res);
};

class Overlay2Qix
{
	std::string overlay;
	double longitude1, longitude2;	// rectangle bounds that define the area
	double latitude1, latitude2;
	double resolution;

public:

	///////////////////////////////////////////////////////////////////////
	////define road type
	bool hasStreets, hasSechwys, hasMajHwys, hasDetailroads, hasMainroads;
	////int streets, sechwys, majHwys, detailroads, mainroads;
	///////////////////////////////////////////////////////////////////////

	std::pair<double, double> getDirName(double,double,double);
	int checkAndAddFile(std::vector<std::string>&, char*);
public:
	Overlay2Qix(std::string aOverlay, double longi1, double longi2,
		double lati1, double lati2, double res): overlay(aOverlay), longitude1(longi1),
		longitude2(longi2), latitude1(lati1), latitude2(lati2), resolution(res) {}
	std::vector<std::string> getQixFile(std::string levelOneDir, std::string levelTwoDir);
};

#endif