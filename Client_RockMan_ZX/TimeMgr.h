#pragma once
#ifndef __TIME_MGR_H__
#define __TIME_MGR_H__

#include "Singleton.h"
class CTimeMgr :
	public CSingleton<CTimeMgr>
{
public:
	CTimeMgr();
	virtual ~CTimeMgr();

public:
	float Get_DeltaTime() { return m_fDeltaTime; }
	HRESULT Ready_Time_Manager();
	void Update_Time_Manager();

private:
	LARGE_INTEGER m_iBegin;
	LARGE_INTEGER m_iEnd;
	LARGE_INTEGER m_iCpuTick;
	float m_fDeltaTime;
};


#endif // !__TIME_MGR_H__
