//=============================================================================
//
// �����蔻�菈�� [HitCheck.h]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
bool hitBC(D3DXVECTOR3 centerPosA, D3DXVECTOR3 centerPosB, float r1, float r2);
bool hitBB(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 size1, D3DXVECTOR3 size2);

#endif