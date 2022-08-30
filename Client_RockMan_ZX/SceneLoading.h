#pragma once
#include "Scene.h"
class CSceneLoading :
	public CScene
{
public:
	static CScene* Create_Scene();
	static unsigned __stdcall ThreadFunc(LPVOID pVoid);
private:
	CSceneLoading();
	virtual ~CSceneLoading();

private:
	HANDLE	m_hThread;
	CRITICAL_SECTION cri;
	HWND	m_hVideo;
public:
	virtual HRESULT Init_Scene() override;
	virtual void Update_Scene() override;
	virtual void Late_Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;


};

