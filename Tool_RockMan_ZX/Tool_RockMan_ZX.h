
// Tool_RockMan_ZX.h : Tool_RockMan_ZX ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CTool_RockMan_ZXApp:
// �� Ŭ������ ������ ���ؼ��� Tool_RockMan_ZX.cpp�� �����Ͻʽÿ�.
//

class CTool_RockMan_ZXApp : public CWinAppEx
{
public:
	CTool_RockMan_ZXApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTool_RockMan_ZXApp theApp;
