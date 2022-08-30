#pragma once
#ifndef __FRAME_MGR_H__
#define __FRAME_MGR_H__

#include "Singleton.h"
class CFrameMgr :
	public CSingleton<CFrameMgr>
{
public:
	CFrameMgr();
	virtual ~CFrameMgr();

public:
	void Ready_Frame_Manager(float fFPS);
	bool Frame_Lock();
	void Render_Frame_Manager();
private:
	LARGE_INTEGER m_iBegin;
	LARGE_INTEGER m_iEnd;
	LARGE_INTEGER m_iCpuTick;
	float m_fDeltaTime;
	float m_fSPF;

	float m_fFPSTime;
	int m_iFPS;
	TCHAR m_szFPS[32];
};


#endif // !__FRAME_MGR_H__
