#include "TransInfoManager.h"

CTransInfoManager::CTransInfoManager(void)
{
	m_ntrans = 0;
}

CTransInfoManager::~CTransInfoManager(void)
{

}

COLORREF CTransInfoManager::GetTransInfo()
{
	//ASSERT(m_color != NULL);
	return this->m_color;
}

void CTransInfoManager::SetTransInfo( int trans, COLORREF color )
{
	//ASSERT(color != NULL);

	m_ntrans = trans;
	m_color  = color;
	// put the transparency info into the last two bits of every pixel
	BYTE r = (GetRValue(m_color) & ~0x01) | m_ntrans>>4;
	BYTE g = (GetGValue(m_color) & ~0x03) | (m_ntrans & 0x0f)>>2;
	BYTE b = (GetBValue(m_color) & ~0x03) | m_ntrans & 0x03;
	//important for here, b,g,r
	m_color = RGB(b,g,r);
}