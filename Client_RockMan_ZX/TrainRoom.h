#pragma once
#ifndef __TRAIN_ROOM_H__
#define __TRAIN_ROOM_H__

#include "Scene.h"
class CTrainRoom :
	public CScene
{
public:
	static CScene* Create_Train_Room();
private:
	CTrainRoom();
public:
	virtual ~CTrainRoom();

	// CScene을(를) 통해 상속됨
	virtual HRESULT Init_Scene() override;
	virtual void Update_Scene() override;
	virtual void Late_Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
};


#endif // !__TRAIN_ROOM_H__
