#pragma once
#ifndef __MAIN_APLLICATION_H__
#define __MAIN_APPLICATION_H__

class CMainApplication
{
public:
	static CMainApplication* Create_Game();
public:
	HRESULT Init_Game();
	void	Update_Game();
	void	Late_Update_Game();
	void	Render_Game();
	void	Release_Game();


private:
	CMainApplication();
public:
	~CMainApplication();
};


#endif // !__MAIN_APLLICATION_H__
