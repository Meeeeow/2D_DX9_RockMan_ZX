#pragma once
#ifndef __SCRAPER_H__
#define __SCRAPER_H__

#include "Scene.h"
class CScraper :
	public CScene
{
public:
	static CScene* Create_Scraper();
private:
	CScraper();
public:
	virtual ~CScraper();

	// CScene을(를) 통해 상속됨
	virtual HRESULT Init_Scene() override;
	virtual void Update_Scene() override;
	virtual void Late_Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
};


#endif // !__SCRAPER_H__
