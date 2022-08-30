#include "stdafx.h"
#include "TimeMgr.h"


CTimeMgr::CTimeMgr()
{
}


CTimeMgr::~CTimeMgr()
{
}

HRESULT CTimeMgr::Ready_Time_Manager()
{
	QueryPerformanceFrequency(&m_iCpuTick);
	QueryPerformanceCounter(&m_iBegin);
	QueryPerformanceCounter(&m_iEnd);
	return S_OK;
}

void CTimeMgr::Update_Time_Manager()
{
	QueryPerformanceFrequency(&m_iCpuTick);
	QueryPerformanceCounter(&m_iEnd);
	m_fDeltaTime = float(m_iEnd.QuadPart - m_iBegin.QuadPart) / m_iCpuTick.QuadPart;
	m_iBegin = m_iEnd;
}
