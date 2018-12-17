#pragma once
//#include "StdAfx.h"
#include "afxwin.h"
#include <string>


struct TransInfo
{
	typedef enum {area, line, point} Type;
	Type type;
	int values[3];

	TransInfo() 
	{
	}

	TransInfo(Type t, int a=0, int b=0, int c=0)
	: type(t)
	{
		values[0] = a;
		values[1] = b;
		values[2] = c;
	}
	
	static int char2int(char c)
	{
		if(isdigit(c))
		{
			return c-'0';
		}
		else if(c == '_')
		{
			return 0;
		}
		else if(c == 'l' || c == 'L' || c == 'n' || c == 'N' || c == 'i' || c == 'I' ||
			c == 'o' || c == 'O' || c == 'a' || c == 'A' || c == 'w' || c == 'W')
		{
			return c;
		}
		else 
		{
			return c-'a'+10;
		}
	}
};


class CTransInfoManager
{
public:
	COLORREF m_color;
	//TransInfo m_transinfo;
	int m_ntrans;

public:
	CTransInfoManager(void);
public:
	~CTransInfoManager(void);

public:
	COLORREF GetTransInfo();
	void SetTransInfo( int trans, COLORREF color );

};
