#pragma once
#ifndef __SCENE_H__
#define __SCENE_H__

class CScene abstract
{
public:
	CScene();
	virtual ~CScene();

	virtual HRESULT Init_Scene() PURE;
	virtual void Update_Scene() PURE;
	virtual void Late_Update_Scene() PURE;
	virtual void Render_Scene() PURE;
	virtual void Release_Scene() PURE;

	void Render_Scene_Key(const wstring& _FilePath, float fSclaex, float fSclaey);
	void Render_Scene_Key_No_Quake(const wstring& _FilePath);
	void Render_Scene_No_Scroll(const wstring& _FilePath);
	void Get_TileArray(const wstring& _FileKey);
	_vec2 Get_Vector() { return m_vecTileArray; }
protected:
	_vec2	m_vecTileArray;
};


#endif // !__SCENE_H__
