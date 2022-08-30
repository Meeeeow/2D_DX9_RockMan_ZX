#pragma once
#include "Singleton.h"
class CInterfaceMgr :
	public CSingleton<CInterfaceMgr>
{
public:
	CInterfaceMgr();
	virtual ~CInterfaceMgr();
};

