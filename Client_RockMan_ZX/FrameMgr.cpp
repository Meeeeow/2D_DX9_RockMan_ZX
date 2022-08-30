#include "stdafx.h"
#include "FrameMgr.h"
#include "TimeMgr.h"

CFrameMgr::CFrameMgr()
{
}


CFrameMgr::~CFrameMgr()
{
}

void CFrameMgr::Ready_Frame_Manager(float fFPS)
{
	m_fSPF = 1.f / fFPS;
	m_fDeltaTime = 0.f;
	QueryPerformanceFrequency(&m_iCpuTick);
	QueryPerformanceCounter(&m_iBegin);
	QueryPerformanceCounter(&m_iEnd);

	m_fFPSTime = 0.f;
	m_iFPS = 0;
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
}

bool CFrameMgr::Frame_Lock()
{
	QueryPerformanceCounter(&m_iEnd);
	m_fDeltaTime += float(m_iEnd.QuadPart - m_iBegin.QuadPart) / m_iCpuTick.QuadPart;
	m_iBegin = m_iEnd;
	if (m_fDeltaTime >= m_fSPF)
	{
		++m_iFPS;
		QueryPerformanceFrequency(&m_iCpuTick);
		m_fDeltaTime = 0.f;
		return true;
	}
	return false;
}

void CFrameMgr::Render_Frame_Manager()
{
	m_fFPSTime += CTimeMgr::Get_Instance()->Get_DeltaTime();
	if (1 < m_fFPSTime)
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);

		m_iFPS = 0;
		m_fFPSTime = 0.f;
	}
	SetWindowText(g_hWND, m_szFPS);
}
