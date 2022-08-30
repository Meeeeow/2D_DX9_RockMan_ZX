#include "stdafx.h"
#include "KeyMgr.h"


CKeyMgr::CKeyMgr()
{
}


CKeyMgr::~CKeyMgr()
{
}

bool CKeyMgr::Key_Pressing(int _key)
{
	if (GetAsyncKeyState(_key) & 0x8000)
		return true;
	return false;
}

bool CKeyMgr::Key_Down(int _key)
{
	if (!m_bKeyState[_key] && (GetAsyncKeyState(_key) & 0x8000))
	{
		m_bKeyState[_key] = true;
		return true;
	}

	if (m_bKeyState[_key] && !(GetAsyncKeyState(_key) & 0x8000))
		m_bKeyState[_key] = false;

	return false;
}

bool CKeyMgr::Key_Double_Down(int _key)
{
	if (m_bKeyState[_key] && (GetAsyncKeyState(_key) & 0x8000))
	{
		m_bKeyState[_key] = false;
		return true;
	}
}


bool CKeyMgr::Key_Up(int _key)
{
	if (m_bKeyState[_key] && !(GetAsyncKeyState(_key) & 0x8000))
	{
		m_bKeyState[_key] = false;
		return true;
	}

	if (!m_bKeyState[_key] && (GetAsyncKeyState(_key) & 0x8000))
		m_bKeyState[_key] = !m_bKeyState[_key];

	return false;
}
