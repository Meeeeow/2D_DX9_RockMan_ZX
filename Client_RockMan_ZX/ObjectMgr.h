#pragma once
#ifndef __OBJECT_MGR_H__
#define __OBJECT_MGR_H__


#include "Singleton.h"
class CObjectBase;
class CObjectMgr :
	public CSingleton<CObjectMgr>
{
public:
	enum ID { PLAYER, TRAININGROOM, AREA_E1, AREA_D2, MAGMA, AREA_D5, SCRAPER, BOSS, END};

public:
	void Load_ObjectData(ID _eID, wstring pFilePath);
	void Add_Object(CObjectBase* pObj, ID _eID) { m_ListObject[_eID].emplace_back(pObj); }

	template <typename T>
	void Add_Object_Template(CObjectBase* pObj, ID _eID);

	void Update_ObjectMgr();
	void Update_Type(ID _eID);
	void Late_Update_ObjectMgr();
	void Late_Update_Type(ID _eID);
	void Render_ObjectMgr();
	void Render_Player();
	void Render_Type(ID _eID);
	void Release_ObjectMgr();
	bool Using_SpecialAttack();
	bool Using_AquaField();
	ID	 Get_Current_ID();
	void Hit_Check(ID _eID,int iDmg, RECT rc);
	void Hurricane_Check(ID _eID, int iDmg, RECT rc,float fY);
	void FX_Attack(ID _eID, int iDmg, RECT rc,float fX);
	bool Get_Can_Play();
	UNITINFO Get_PlayerInfo();
	list<CObjectBase*> Get_Player() { return m_ListObject[PLAYER]; }
	list<CObjectBase*> Get_Type(ID _eID) { return m_ListObject[_eID]; }


private:
	list<CObjectBase*> m_ListObject[END];

public:
	CObjectMgr();
	virtual ~CObjectMgr();
};


#endif // !__OBJECT_MGR_H__

template<typename T>
inline void CObjectMgr::Add_Object_Template(CObjectBase * pObj, ID _eID)
{

}
