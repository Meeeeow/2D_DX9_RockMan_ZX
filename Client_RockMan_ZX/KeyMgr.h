#pragma once
#ifndef __KEY_MGR_H__
#define __KEY_MGR_H__

#define VK_MAX 0xFF

#include "Singleton.h"
class CKeyMgr :
	public CSingleton<CKeyMgr>
{
public:
	CKeyMgr();
	virtual ~CKeyMgr();
public:
	bool Key_Pressing(int _key);
	bool Key_Down(int _key);
	bool Key_Double_Down(int _key);
	bool Key_Up(int _key);

private:
	bool			m_bKeyState[VK_MAX];
};


#endif // !__KEY_MGR_H__
