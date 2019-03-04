//=============================================================================
//
// �A�N�V�������� [action.h]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#ifndef _ACTION_H_
#define _ACTION_H_

#include "battle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ���ꂼ��̃A�j���[�V�������ɂ��鏈��
void IdleAction(CHARA *Chara, int ControllerNum);
void FrontwalkAction(CHARA *Chara, int ControllerNum);
void BackwalkAction(CHARA *Chara, int ControllerNum);
void RightstepAction(CHARA *Chara);
void LeftstepAction(CHARA *Chara);
void GuardAction(CHARA *Chara, int ControllerNum);
void DamageAction(CHARA *Chara);
void DownAction(CHARA *Chara);
void DownposeAction(CHARA *Chara);
void GetupAction(CHARA *Chara);
void PunchiAction(CHARA *Chara, int ControllerNum);
void StraightAction(CHARA *Chara, int ControllerNum);
void UpperAction(CHARA *Chara);
void KickAction(CHARA *Chara);
void HadouAction(CHARA *Chara);
void ShoryuAction(CHARA *Chara);
void SPattackAction(CHARA *Chara);
void ThrowAction(CHARA *Chara);
void WinAction(CHARA *Chara);
void MissAction(CHARA *Chara);
void ThrowedposeAction(CHARA *Chara);

void HitAction(CHARA *AttackChara, CHARA *DefendChara);			// �U�������������Ƃ��̓���


#endif
