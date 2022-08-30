#pragma once
#ifndef __DEFINE_H__
#define __DEFINE_H__

#define SAFE_DELETE(p) if(p) {delete p; p = nullptr;}
#define SAFE_DELETE_ARR(p) if(p) {delete[] p; p = nullptr;}

#define WINCX 800
#define WINCY 600



// OBJ WAY
#define OBJ_LEFT -1
#define OBJ_RIGHT 1
#define ENEMY_LEFT   1
#define ENEMY_RIGHT -1

// OBJ EVENT
#define OBJ_DEAD 1
#define OBJ_NOEVENT 0

// TILE
#define TILECX 20
#define TILECY 20

// JUMP
#define GRAVITY 9.8f 
#ifdef _AFX
#define MSG_BOX(message) AfxMessageBox(message)
#else 
#define MSG_BOX(message) MessageBox(nullptr, message, L"System_Error", MB_OK)
#endif // _AFX

#endif // !__DEFINE_H__
