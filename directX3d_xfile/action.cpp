//=============================================================================
//
// �A�N�V�������� [action.cpp]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#include "main.h"
#include "action.h"
#include "battle.h"
#include "particle.h"
#include "game.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//=====================================================================================================
// �A�C�h�����̃A�N�V����
//=====================================================================================================
void IdleAction(CHARA *Chara, int ControllerNum)
{
	// �R�}���h����
	if (Chara->CommandInput == true)
	{
		// ���͂��ꂽ�R�}���h�ɉ����ăA�j���[�V������ύX������
		// �ړ�
		if (CheckInput(Chara->Input, CMD_Frontwalk) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Frontwalk, Data[Frontwalk].Spd);
		}
		else if (CheckInput(Chara->Input, CMD_Backwalk) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Backwalk, Data[Backwalk].Spd);
		}
		else if (CheckInput(Chara->Input, CMD_Rightstep) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Rightstep, Data[Rightstep].Spd);
		}
		else if (CheckInput(Chara->Input, CMD_Leftstep) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Leftstep, Data[Leftstep].Spd);
		}

		// �U��
		// SP�Z
		else if (CheckInput(Chara->Input, CMD_SPattack) == true && Chara->SP == FULL_SPGAUGE)
		{
			// SP�Q�[�W����
			PlaySound(SE_EFFECT0);
			AddSpGauge(Chara, -FULL_SPGAUGE);
			Chara->Animation->ChangeAnimation(Chara->Animation, SPattack, Data[SPattack].Spd);
		}
		// ����
		else if (CheckInput(Chara->Input, CMD_Throw) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Throw, Data[Throw].Spd);
		}
		// �g����
		else if ((CheckInput(Chara->Input, CMD_Hadou) == true || CheckInput(Chara->Input, CMD_Hadou1) == true)
			&& Chara->HadouBullet.use == false)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Hadou, Data[Hadou].Spd);
		}
		// ������
		else if (CheckInput(Chara->Input, CMD_Shoryu) == true || CheckInput(Chara->Input, CMD_Shoryu1) == true
			|| CheckInput(Chara->Input, CMD_Shoryu2) == true || CheckInput(Chara->Input, CMD_Shoryu3) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Shoryu, Data[Shoryu].Spd);
		}
		// �p���`
		else if (CheckInput(Chara->Input, CMD_Punchi) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Punchi, Data[Punchi].Spd);
		}
		// �L�b�N
		else if (CheckInput(Chara->Input, CMD_Kick) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Kick, Data[Kick].Spd);
		}
		// �K�[�h
		else if (CheckInput(Chara->Input, CMD_Guard) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Guard, Data[Guard].Spd);
		}

	}

	// �C�[�W�[����
	else
	{
		// �ړ�
		// �O
		if (GetKeyboardPress(ControllerNum == 0 ? DIK_RIGHT : DIK_A)
			|| IsButtonPressed(ControllerNum, ControllerNum == 0 ? BUTTON_RIGHT : BUTTON_LEFT)
			|| IsButtonPressed(ControllerNum, ControllerNum == 0 ? STICK_RIGHT : STICK_LEFT))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Frontwalk, Data[Frontwalk].Spd);
		}
		// ���
		else if (GetKeyboardPress(ControllerNum == 0 ? DIK_LEFT : DIK_D)
			|| IsButtonPressed(ControllerNum, ControllerNum == 0 ? BUTTON_LEFT : BUTTON_RIGHT)
			|| IsButtonPressed(ControllerNum, ControllerNum == 0 ? STICK_LEFT : STICK_RIGHT))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Backwalk, Data[Backwalk].Spd);
		}
		// ��O
		else if (GetKeyboardPress(ControllerNum == 0 ? DIK_DOWN : DIK_S)
			|| IsButtonPressed(ControllerNum, ControllerNum == 0 ? BUTTON_DOWN : BUTTON_UP)
			|| IsButtonPressed(ControllerNum, ControllerNum == 0 ? STICK_DOWN : STICK_UP))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Rightstep, Data[Rightstep].Spd);
		}
		// ��
		else if (GetKeyboardPress(ControllerNum == 0 ? DIK_UP : DIK_W)
			|| IsButtonPressed(ControllerNum, ControllerNum == 0 ? BUTTON_UP : BUTTON_DOWN)
			|| IsButtonPressed(ControllerNum, ControllerNum == 0 ? STICK_UP : STICK_DOWN))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Leftstep, Data[Leftstep].Spd);
		}

		// �U��
		// ����
		else if ((GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I) && GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K))
			|| (IsButtonTriggered(ControllerNum, BUTTON_A) && IsButtonTriggered(ControllerNum, BUTTON_B)))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Throw, Data[Throw].Spd);
		}
		// SP�Z
		else if (Chara->SP == FULL_SPGAUGE &&
			((GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O) && GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L))
				|| (IsButtonTriggered(ControllerNum, BUTTON_C) && IsButtonTriggered(ControllerNum, BUTTON_X))))
		{
			// SP�Q�[�W����
			PlaySound(SE_EFFECT0);
			AddSpGauge(Chara, -FULL_SPGAUGE);
			Chara->Animation->ChangeAnimation(Chara->Animation, SPattack, Data[SPattack].Spd);
		}
		// �p���`
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I)
			|| IsButtonTriggered(ControllerNum, BUTTON_A))
		{
			Chara->graceflag = true;
			Chara->gracetype = Punchi;
		}
		// �L�b�N
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K)
			|| IsButtonTriggered(ControllerNum, BUTTON_B))
		{
			Chara->graceflag = true;
			Chara->gracetype = Kick;
		}
		// �g����
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L)
			|| IsButtonTriggered(ControllerNum, BUTTON_X))
		{
			Chara->graceflag = true;
			Chara->gracetype = Hadou;
		}
		// ������
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O)
			|| IsButtonTriggered(ControllerNum, BUTTON_C))
		{
			Chara->graceflag = true;
			Chara->gracetype = Shoryu;
		}
		// �K�[�h
		else if (GetKeyboardPress(ControllerNum == 0 ? DIK_G : DIK_P)
			|| IsButtonPressed(ControllerNum, BUTTON_Y))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Guard, Data[Guard].Spd);
		}
	}
}

//=====================================================================================================
// �O�������̃A�N�V����
//=====================================================================================================
void FrontwalkAction(CHARA *Chara, int ControllerNum)
{
	// �R�}���h����
	if (Chara->CommandInput == true)
	{
		// ���͂��ꂽ�R�}���h�ɉ����ăA�j���[�V������ύX������
		// �U��
		// SP�Z
		if (CheckInput(Chara->Input, CMD_SPattack) == true && Chara->SP == FULL_SPGAUGE)
		{
			// SP�Q�[�W����
			PlaySound(SE_EFFECT0);
			AddSpGauge(Chara, -FULL_SPGAUGE);
			Chara->Animation->ChangeAnimation(Chara->Animation, SPattack, Data[SPattack].Spd);
		}
		// ����
		else if (CheckInput(Chara->Input, CMD_Throw) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Throw, Data[Throw].Spd);
		}
		// �g����
		else if ((CheckInput(Chara->Input, CMD_Hadou) == true || CheckInput(Chara->Input, CMD_Hadou1) == true)
			&& Chara->HadouBullet.use == false)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Hadou, Data[Hadou].Spd);
		}
		// ������
		else if (CheckInput(Chara->Input, CMD_Shoryu) == true || CheckInput(Chara->Input, CMD_Shoryu1) == true
			|| CheckInput(Chara->Input, CMD_Shoryu2) == true || CheckInput(Chara->Input, CMD_Shoryu3) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Shoryu, Data[Shoryu].Spd);
		}
		// �p���`
		else if (CheckInput(Chara->Input, CMD_Punchi) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Punchi, Data[Punchi].Spd);
		}
		// �L�b�N
		else if (CheckInput(Chara->Input, CMD_Kick) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Kick, Data[Kick].Spd);
		}
		// �K�[�h
		else if (CheckInput(Chara->Input, CMD_Guard) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Guard, Data[Guard].Spd);
		}

		// �ړ�
		else if (CheckInput(Chara->Input, CMD_Frontwalk) == true)
		{
		}
		else if (CheckInput(Chara->Input, CMD_Backwalk) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Backwalk, Data[Backwalk].Spd);
		}
		else if (CheckInput(Chara->Input, CMD_Rightstep) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Rightstep, Data[Rightstep].Spd);
		}
		else if (CheckInput(Chara->Input, CMD_Leftstep) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Leftstep, Data[Leftstep].Spd);
		}

		// �L�[�����[�X�őҋ@�ɖ߂�
		else
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		}
	}

	// �C�[�W�[����
	else
	{
		// �O
		if (GetKeyboardPress(ControllerNum == 0 ? DIK_RIGHT : DIK_A)
			|| IsButtonPressed(ControllerNum, ControllerNum == 0 ? BUTTON_RIGHT : BUTTON_LEFT)
			|| IsButtonPressed(ControllerNum, ControllerNum == 0 ? STICK_RIGHT : STICK_LEFT))
		{

		}
		// �{�^�������[�X�őҋ@�ɖ߂�
		else
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		}

		// �U��
		// ����
		if ((GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I) && GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K))
			|| (IsButtonTriggered(ControllerNum, BUTTON_A) && IsButtonTriggered(ControllerNum, BUTTON_B)))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Throw, Data[Throw].Spd);
		}
		// SP�Z
		else if (Chara->SP == FULL_SPGAUGE &&
			((GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O) && GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L))
				|| (IsButtonTriggered(ControllerNum, BUTTON_C) && IsButtonTriggered(ControllerNum, BUTTON_X))))
		{
			// SP�Q�[�W����
			PlaySound(SE_EFFECT0);
			AddSpGauge(Chara, -FULL_SPGAUGE);
			Chara->Animation->ChangeAnimation(Chara->Animation, SPattack, Data[SPattack].Spd);
		}
		// �p���`
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I)
			|| IsButtonTriggered(ControllerNum, BUTTON_A))
		{
			Chara->graceflag = true;
			Chara->gracetype = Punchi;
		}
		// �L�b�N
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K)
			|| IsButtonTriggered(ControllerNum, BUTTON_B))
		{
			Chara->graceflag = true;
			Chara->gracetype = Kick;
		}
		// �g����
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L)
			|| IsButtonTriggered(ControllerNum, BUTTON_X))
		{
			// ���˂���Ă��Ȃ��ꍇ�̂�
			if (Chara->HadouBullet.use == false)
			{
				Chara->graceflag = true;
				Chara->gracetype = Hadou;
			}
		}
		// ������
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O)
			|| IsButtonTriggered(ControllerNum, BUTTON_C))
		{
			Chara->graceflag = true;
			Chara->gracetype = Shoryu;
		}
		// �K�[�h
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_G : DIK_P)
			|| IsButtonTriggered(ControllerNum, BUTTON_Y))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Guard, Data[Guard].Spd);
		}
	}
}

//=====================================================================================================
// ���������̃A�N�V����
//=====================================================================================================
void BackwalkAction(CHARA *Chara, int ControllerNum)
{
	// �R�}���h����
	if (Chara->CommandInput == true)
	{
		// ���͂��ꂽ�R�}���h�ɉ����ăA�j���[�V������ύX������
		// �U��
		// SP�Z
		if (CheckInput(Chara->Input, CMD_SPattack) == true && Chara->SP == FULL_SPGAUGE)
		{
			// SP�Q�[�W����
			PlaySound(SE_EFFECT0);
			AddSpGauge(Chara, -FULL_SPGAUGE);
			Chara->Animation->ChangeAnimation(Chara->Animation, SPattack, Data[SPattack].Spd);
		}
		// ����
		else if (CheckInput(Chara->Input, CMD_Throw) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Throw, Data[Throw].Spd);
		}
		// �g����
		else if ((CheckInput(Chara->Input, CMD_Hadou) == true || CheckInput(Chara->Input, CMD_Hadou1) == true)
			&& Chara->HadouBullet.use == false)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Hadou, Data[Hadou].Spd);
		}
		// ������
		else if (CheckInput(Chara->Input, CMD_Shoryu) == true || CheckInput(Chara->Input, CMD_Shoryu1) == true
			|| CheckInput(Chara->Input, CMD_Shoryu2) == true || CheckInput(Chara->Input, CMD_Shoryu3) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Shoryu, Data[Shoryu].Spd);
		}
		// �p���`
		else if (CheckInput(Chara->Input, CMD_Punchi) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Punchi, Data[Punchi].Spd);
		}
		// �L�b�N
		else if (CheckInput(Chara->Input, CMD_Kick) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Kick, Data[Kick].Spd);
		}
		// �K�[�h
		else if (CheckInput(Chara->Input, CMD_Guard) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Guard, Data[Guard].Spd);
		}

		// �ړ�
		else if (CheckInput(Chara->Input, CMD_Frontwalk) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Frontwalk, Data[Frontwalk].Spd);
		}
		else if (CheckInput(Chara->Input, CMD_Backwalk) == true)
		{
		}
		else if (CheckInput(Chara->Input, CMD_Rightstep) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Rightstep, Data[Rightstep].Spd);
		}
		else if (CheckInput(Chara->Input, CMD_Leftstep) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Leftstep, Data[Leftstep].Spd);
		}

		// �L�[�����[�X�őҋ@�ɖ߂�
		else
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		}
	}

	// �C�[�W�[����
	else
	{
		// ���
		if (GetKeyboardPress(ControllerNum == 0 ? DIK_LEFT : DIK_D)
			|| IsButtonPressed(ControllerNum, ControllerNum == 0 ? BUTTON_LEFT : BUTTON_RIGHT)
			|| IsButtonPressed(ControllerNum, ControllerNum == 0 ? STICK_LEFT : STICK_RIGHT))
		{

		}
		// �{�^�������[�X�őҋ@�ɖ߂�
		else
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		}

		// �U��
		// ����
		if ((GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I) && GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K))
			|| (IsButtonTriggered(ControllerNum, BUTTON_A) && IsButtonTriggered(ControllerNum, BUTTON_B)))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Throw, Data[Throw].Spd);
		}
		// SP�Z
		else if (Chara->SP == FULL_SPGAUGE &&
			((GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O) && GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L))
				|| (IsButtonTriggered(ControllerNum, BUTTON_C) && IsButtonTriggered(ControllerNum, BUTTON_X))))
		{
			// SP�Q�[�W����
			PlaySound(SE_EFFECT0);
			AddSpGauge(Chara, -FULL_SPGAUGE);
			Chara->Animation->ChangeAnimation(Chara->Animation, SPattack, Data[SPattack].Spd);
		}
		// �p���`
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I)
			|| IsButtonTriggered(ControllerNum, BUTTON_A))
		{
			Chara->graceflag = true;
			Chara->gracetype = Punchi;
		}
		// �L�b�N
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K)
			|| IsButtonTriggered(ControllerNum, BUTTON_B))
		{
			Chara->graceflag = true;
			Chara->gracetype = Kick;
		}
		// �g����
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L)
			|| IsButtonTriggered(ControllerNum, BUTTON_X))
		{
			// ���˂���Ă��Ȃ��ꍇ�̂�
			if (Chara->HadouBullet.use == false)
			{
				Chara->graceflag = true;
				Chara->gracetype = Hadou;
			}
		}
		// ������
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O)
			|| IsButtonTriggered(ControllerNum, BUTTON_C))
		{
			Chara->graceflag = true;
			Chara->gracetype = Shoryu;
		}
		// �K�[�h
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_G : DIK_P)
			|| IsButtonTriggered(ControllerNum, BUTTON_Y))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Guard, Data[Guard].Spd);
		}
	}
}

//=====================================================================================================
// �E�X�e�b�v���̃A�N�V����
//=====================================================================================================
void RightstepAction(CHARA *Chara)
{
	// �A�j���[�V�����I���őҋ@�ɖ߂�
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		Chara->HitFrag = false;
	}
}

//=====================================================================================================
// ���X�e�I�b�v���̃A�N�V����
//=====================================================================================================
void LeftstepAction(CHARA *Chara)
{
	// �A�j���[�V�����I���őҋ@�ɖ߂�
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		Chara->HitFrag = false;
	}
}

//=====================================================================================================
// �K�[�h���̃A�N�V����
//=====================================================================================================
void GuardAction(CHARA *Chara, int ControllerNum)
{
	// �R�}���h����
	if (Chara->CommandInput == true)
	{
		// �U��
		// SP�Z
		if (CheckInput(Chara->Input, CMD_SPattack) == true && Chara->SP == FULL_SPGAUGE)
		{
			// SP�Q�[�W����
			PlaySound(SE_EFFECT0);
			AddSpGauge(Chara, -FULL_SPGAUGE);
			Chara->Animation->ChangeAnimation(Chara->Animation, SPattack, Data[SPattack].Spd);
		}
		// ����
		else if (CheckInput(Chara->Input, CMD_Throw) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Throw, Data[Throw].Spd);
		}
		// �g����
		else if ((CheckInput(Chara->Input, CMD_Hadou) == true || CheckInput(Chara->Input, CMD_Hadou1) == true)
			&& Chara->HadouBullet.use == false)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Hadou, Data[Hadou].Spd);
		}
		// ������
		else if (CheckInput(Chara->Input, CMD_Shoryu) == true || CheckInput(Chara->Input, CMD_Shoryu1) == true
			|| CheckInput(Chara->Input, CMD_Shoryu2) == true || CheckInput(Chara->Input, CMD_Shoryu3) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Shoryu, Data[Shoryu].Spd);
		}
		// �p���`
		else if (CheckInput(Chara->Input, CMD_Punchi) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Punchi, Data[Punchi].Spd);
		}
		// �L�b�N
		else if (CheckInput(Chara->Input, CMD_Kick) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Kick, Data[Kick].Spd);
		}
		else if (CheckInput(Chara->Input, CMD_Guard) == true)
		{
		}
		// �L�[�����[�X�őҋ@�ɖ߂�
		else
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		}
	}

	// �C�[�W�[����
	else
	{
		// �U��
		// ����
		if ((GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I) && GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K))
			|| (IsButtonTriggered(ControllerNum, BUTTON_A) && IsButtonTriggered(ControllerNum, BUTTON_B)))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Throw, Data[Throw].Spd);
		}
		// SP�Z
		else if (Chara->SP == FULL_SPGAUGE &&
			((GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O) && GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L))
				|| (IsButtonTriggered(ControllerNum, BUTTON_C) && IsButtonTriggered(ControllerNum, BUTTON_X))))
		{
			// SP�Q�[�W����
			PlaySound(SE_EFFECT0);
			AddSpGauge(Chara, -FULL_SPGAUGE);
			Chara->Animation->ChangeAnimation(Chara->Animation, SPattack, Data[SPattack].Spd);
		}
		// �p���`
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I)
			|| IsButtonTriggered(ControllerNum, BUTTON_A))
		{
			Chara->graceflag = true;
			Chara->gracetype = Punchi;
		}
		// �L�b�N
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K)
			|| IsButtonTriggered(ControllerNum, BUTTON_B))
		{
			Chara->graceflag = true;
			Chara->gracetype = Kick;
		}
		// �g����
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L)
			|| IsButtonTriggered(ControllerNum, BUTTON_X))
		{
			if (Chara->HadouBullet.use == false)
			{
				Chara->graceflag = true;
				Chara->gracetype = Hadou;
			}
		}
		// ������
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O)
			|| IsButtonTriggered(ControllerNum, BUTTON_C))
		{
			Chara->graceflag = true;
			Chara->gracetype = Shoryu;
		}

		else if (GetKeyboardPress(ControllerNum == 0 ? DIK_G : DIK_P)
			|| IsButtonPressed(ControllerNum, BUTTON_Y))
		{

		}
		// �{�^�������[�X�őҋ@�ɖ߂�
		else
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		}
	}

}

//=====================================================================================================
// ��_���[�W���̃A�N�V����
//=====================================================================================================
void DamageAction(CHARA *Chara)
{
	// �A�j���[�V�����I���őҋ@�ɖ߂�
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		Chara->HadouBullet.frame = 0;
		Chara->graceframe = 0;
	}
}

//=====================================================================================================
// �_�E�����̃A�N�V����
//=====================================================================================================
void DownAction(CHARA *Chara)
{
	// �A�j���[�V�����I���ŋN���オ��Ɉڍs
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Downpose, Data[Downpose].Spd);
		// ���[�V�������W�ɃL�����N�^�[���W�����킹��
		Chara->pos = D3DXVECTOR3(Chara->HitBall[Hips].pos.x, 0.0f, Chara->HitBall[Hips].pos.z);
	}
}

//=====================================================================================================
// �_�E���|�[�Y���̃A�N�V����
//=====================================================================================================
void DownposeAction(CHARA *Chara)
{
	// �A�j���[�V�����I���ŋN���オ��Ɉڍs
	if (Chara->Animation->MotionEnd == true && Chara->HPzan > 0)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Getup, Data[Getup].Spd);
	}
}

//=====================================================================================================
// �N���オ�莞�̃A�N�V����
//=====================================================================================================
void GetupAction(CHARA *Chara)
{
	// �A�j���[�V�����I���őҋ@�ɖ߂�
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		Chara->HadouBullet.frame = 0;
		Chara->graceframe = 0;
		Chara->HitFrag = false;
	}
}

//=====================================================================================================
// �p���`���̃A�N�V����
//=====================================================================================================
void PunchiAction(CHARA *Chara, int ControllerNum)
{
	SetAttackParticle(Chara->HitBall[LeftHand].pos);

	// �q�b�g���ǉ����͂ŃX�g���[�g�Ɉڍs
	if (Chara->HitFrag == true)
	{

		// �R�}���h����
		if (Chara->CommandInput == true)
		{
			if (CheckInput(Chara->Input, CMD_Straight))
			{
				Chara->Animation->ChangeAnimation(Chara->Animation, Straight, Data[Straight].Spd);
				D3DXMATRIX Mtx = GetBoneMatrix(Chara->Animation, CharaHitPos[Hips]);
				Chara->pos = D3DXVECTOR3(Mtx._41, 0.0f, Mtx._43);
				Chara->HitFrag = false;
				Chara->framecount = 0;
			}
		}

		// �C�[�W�[����
		else
		{
			if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(ControllerNum, BUTTON_A))
			{
				Chara->Animation->ChangeAnimation(Chara->Animation, Straight, Data[Straight].Spd);
				D3DXMATRIX Mtx = GetBoneMatrix(Chara->Animation, CharaHitPos[Hips]);
				Chara->pos = D3DXVECTOR3(Mtx._41, 0.0f, Mtx._43);
				Chara->HitFrag = false;
				Chara->framecount = 0;
			}
		}
	}

	// �A�j���[�V�����I���őҋ@�ɖ߂�
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		Chara->HitFrag = false;
	}

}

//=====================================================================================================
// �X�g���[�g�i�ǌ��p���`�P�j���̃A�N�V����
//=====================================================================================================
void StraightAction(CHARA *Chara, int ControllerNum)
{
	SetAttackParticle(Chara->HitBall[RightHand].pos);

	// �q�b�g���ǉ����͂ŃA�b�p�[�Ɉڍs
	if (Chara->HitFrag == true)
	{

		// �R�}���h����
		if (Chara->CommandInput == true)
		{
			if (CheckInput(Chara->Input, CMD_Upper))
			{
				Chara->Animation->ChangeAnimation(Chara->Animation, Upper, Data[Upper].Spd);
				Chara->HitFrag = false;
				Chara->framecount = 0;
			}
		}

		// �C�[�W�[����
		else
		{
			if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(ControllerNum, BUTTON_A))
			{
				Chara->Animation->ChangeAnimation(Chara->Animation, Upper, Data[Upper].Spd);
				Chara->HitFrag = false;
				Chara->framecount = 0;
			}
		}
	}

	// �A�j���[�V�����I���őҋ@�ɖ߂�
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		Chara->HitFrag = false;
	}
}

//=====================================================================================================
// �A�b�p�[�i�ǌ��p���`�Q�j���̃A�N�V����
//=====================================================================================================
void UpperAction(CHARA *Chara)
{
	SetAttackParticle(Chara->HitBall[LeftHand].pos);
	// �A�j���[�V�����I���őҋ@�ɖ߂�
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		Chara->HitFrag = false;
	}
}

//=====================================================================================================
// �L�b�N���̃A�N�V����
//=====================================================================================================
void KickAction(CHARA *Chara)
{
	SetAttackParticle(Chara->HitBall[RightFoot].pos);
	// �A�j���[�V�����I���őҋ@�ɖ߂�
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		Chara->HitFrag = false;
	}
}

//=====================================================================================================
// �g�������̃A�N�V����
//=====================================================================================================
void HadouAction(CHARA *Chara)
{
	// ���t���[���o�߂Œe���o��
	Chara->HadouBullet.frame++;
	if (Chara->HadouBullet.frame == FIRE_FRAME)
	{
		SetHadou(&Chara->HadouBullet, Chara->HitBall, Chara->rot);
	}
	// �A�j���[�V�����I���őҋ@�ɖ߂�
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		Chara->HadouBullet.frame = 0;
		Chara->HitFrag = false;
	}
}

//=====================================================================================================
// ���������̃A�N�V����
//=====================================================================================================
void ShoryuAction(CHARA *Chara)
{
	SetShoryuParticle(Chara->HitBall[RightFoot].pos);
	SetShoryuParticle(Chara->HitBall[LeftFoot].pos);
	// �A�j���[�V�����I���őҋ@�ɖ߂�
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		Chara->HitFrag = false;
	}

}

//=====================================================================================================
// SP�U�����̃A�N�V����
//=====================================================================================================
void SPattackAction(CHARA *Chara)
{
	SetSPattackParticle(Chara->HitBall[RightFoot].pos);
	SetSPattackParticle(Chara->HitBall[LeftFoot].pos);
	// �A�j���[�V�����I���őҋ@�ɖ߂�
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		// ���[�V�������W�ɃL�����N�^�[���W�����킹��
		Chara->pos = D3DXVECTOR3(Chara->HitBall[Hips].pos.x, 0.0f, Chara->HitBall[Hips].pos.z);
		Chara->HitFrag = false;
	}
}

//=====================================================================================================
// �����鎞�̃A�N�V����
//=====================================================================================================
void ThrowAction(CHARA *Chara)
{
	// �A�j���[�V�����I���őҋ@�ɖ߂�
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		Chara->HitFrag = false;
	}
}

//=====================================================================================================
// �������̃A�N�V����
//=====================================================================================================
void WinAction(CHARA *Chara)
{

}

//=====================================================================================================
// �������s���̃A�N�V����
//=====================================================================================================
void MissAction(CHARA *Chara)
{
	// �A�j���[�V�����I���őҋ@�ɖ߂�
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		Chara->HitFrag = false;
	}
}

//=====================================================================================================
// �������Ă��鎞�̃A�N�V����
//=====================================================================================================
void ThrowedposeAction(CHARA *Chara)
{
	if (Chara->Animation->PreventAnimID != ThrowedPose)
	{
		Chara->framecount++;
	}
	// ��莞�Ԍo�߂ő���̓����A�j���[�V�����ɍ��킹�ă_�E�����[�V�����Ɉڍs
	if (Chara->framecount == THROW_FRAME)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Down, Data[Down].Spd);
		Chara->framecount = 0;
	}
}

//=============================================================================
//�U�������������Ƃ��ɔ������铮��
//=============================================================================
void HitAction(CHARA *AttackChara, CHARA *DefendChara)
{
	switch (AttackChara->Animation->CurrentAnimID)
	{
	case Punchi:
		// �G�̏�Ԋm�F
		if (DefendChara->Animation->CurrentAnimID == Guard || DefendChara->Animation->CurrentAnimID == Down
			|| DefendChara->Animation->CurrentAnimID == Downpose || DefendChara->Animation->CurrentAnimID == Getup)
		{
			// �G�t�F�N�g
			SetGuardParticle(AttackChara->HitBall[LeftHand].pos);

			// �X�R�A
			AddScore(AttackChara, Data[Punchi].Damage);
			AddScore(DefendChara, Data[Punchi].Damage);
			// ��
			PlaySound(SE_DEFEND0);
		}
		else
		{
			// �G�̃��[�V�����ύX
			DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Damage, Data[Damage].Spd);
			// �_���[�W
			SubDamage(DefendChara, Data[Punchi].Damage, AttackChara->CommandInput);
			// SP�Q�[�W����
			AddSpGauge(AttackChara, Data[Punchi].Damage);
			AddSpGauge(DefendChara, Data[Punchi].Damage);
			// �G�t�F�N�g
			SetHitParticle(AttackChara->HitBall[LeftHand].pos);
			// �X�R�A
			AddScore(AttackChara, Data[Punchi].Damage);
			// ��
			PlaySound(SE_HIT0);
		}
		//�@�q�b�g�t���O�̕ύX
		AttackChara->HitFrag = true;
		break;
	case Straight:
		// �G�̏�Ԋm�F
		if (DefendChara->Animation->CurrentAnimID == Guard || DefendChara->Animation->CurrentAnimID == Down
			|| DefendChara->Animation->CurrentAnimID == Downpose || DefendChara->Animation->CurrentAnimID == Getup)
		{
			// �G�t�F�N�g
			SetGuardParticle(AttackChara->HitBall[RightHand].pos);

			// �X�R�A
			AddScore(AttackChara, Data[Straight].Damage);
			AddScore(DefendChara, Data[Straight].Damage);
			// ��
			PlaySound(SE_DEFEND0);
		}
		else
		{
			// �G�̃��[�V�����ύX
			DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Damage, Data[Damage].Spd);
			// �_���[�W
			SubDamage(DefendChara, Data[Straight].Damage, AttackChara->CommandInput);
			// SP�Q�[�W����
			AddSpGauge(AttackChara, Data[Straight].Damage);
			AddSpGauge(DefendChara, Data[Straight].Damage);
			// �G�t�F�N�g
			SetHitParticle(AttackChara->HitBall[RightHand].pos);
			// �X�R�A
			AddScore(AttackChara, Data[Straight].Damage);
			// ��
			PlaySound(SE_HIT0);
		}
		//�@�q�b�g�t���O�̕ύX
		AttackChara->HitFrag = true;
		break;
	case Upper:
		// �G�̏�Ԋm�F
		if (DefendChara->Animation->CurrentAnimID == Guard || DefendChara->Animation->CurrentAnimID == Down
			|| DefendChara->Animation->CurrentAnimID == Downpose || DefendChara->Animation->CurrentAnimID == Getup)
		{
			// �G�t�F�N�g
			SetGuardParticle(AttackChara->HitBall[LeftHand].pos);

			// �X�R�A
			AddScore(AttackChara, Data[Upper].Damage);
			AddScore(DefendChara, Data[Upper].Damage);
			// ��
			PlaySound(SE_DEFEND0);
		}
		else
		{
			// �G�̃��[�V�����ύX
			DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Down, Data[Down].Spd);
			// �_���[�W
			SubDamage(DefendChara, Data[Upper].Damage, AttackChara->CommandInput);
			// SP�Q�[�W����
			AddSpGauge(AttackChara, Data[Upper].Damage);
			AddSpGauge(DefendChara, Data[Upper].Damage);
			// �G�t�F�N�g
			SetHitParticle(AttackChara->HitBall[LeftHand].pos);
			// �X�R�A
			AddScore(AttackChara, Data[Upper].Damage);
			// ��
			PlaySound(SE_HIT1);
		}
		//�@�q�b�g�t���O�̕ύX
		AttackChara->HitFrag = true;
		break;
	case Kick:
		// �G�̏�Ԋm�F
		if (DefendChara->Animation->CurrentAnimID == Guard || DefendChara->Animation->CurrentAnimID == Down
			|| DefendChara->Animation->CurrentAnimID == Downpose || DefendChara->Animation->CurrentAnimID == Getup)
		{
			// �G�t�F�N�g
			SetGuardParticle(AttackChara->HitBall[RightFoot].pos);

			// �X�R�A
			AddScore(AttackChara, Data[Kick].Damage);
			AddScore(DefendChara, Data[Kick].Damage);
			// ��
			PlaySound(SE_DEFEND0);
		}
		else
		{
			// �G�̃��[�V�����ύX
			DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Damage, Data[Damage].Spd);
			// �_���[�W
			SubDamage(DefendChara, Data[Kick].Damage, AttackChara->CommandInput);
			// SP�Q�[�W����
			AddSpGauge(AttackChara, Data[Kick].Damage);
			AddSpGauge(DefendChara, Data[Kick].Damage);
			// �G�t�F�N�g
			SetHitParticle(AttackChara->HitBall[RightFoot].pos);
			// �X�R�A
			AddScore(AttackChara, Data[Kick].Damage);
			// ��
			PlaySound(SE_HIT0);
		}
		//�@�q�b�g�t���O�̕ύX
		AttackChara->HitFrag = true;
		break;
	case Shoryu:
		// �G�̏�Ԋm�F
		if (DefendChara->Animation->CurrentAnimID == Guard || DefendChara->Animation->CurrentAnimID == Down
			|| DefendChara->Animation->CurrentAnimID == Downpose || DefendChara->Animation->CurrentAnimID == Getup)
		{
			// �_���[�W
			SubDamage(DefendChara, (int)(Data[Shoryu].Damage * 0.1f), AttackChara->CommandInput);
			// �G�t�F�N�g
			SetGuardParticle(AttackChara->HitBall[RightFoot].pos);
			SetGuardParticle(AttackChara->HitBall[LeftFoot].pos);
			// �X�R�A
			AddScore(AttackChara, Data[Shoryu].Damage);
			AddScore(DefendChara, Data[Shoryu].Damage);
			// ��
			PlaySound(SE_DEFEND0);
		}
		else
		{
			// �G�̃��[�V�����ύX
			DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Down, Data[Down].Spd);
			// �_���[�W
			SubDamage(DefendChara, Data[Shoryu].Damage, AttackChara->CommandInput);
			// SP�Q�[�W����
			AddSpGauge(AttackChara, Data[Shoryu].Damage);
			AddSpGauge(DefendChara, Data[Shoryu].Damage);
			// �G�t�F�N�g
			SetHitParticle(AttackChara->HitBall[LeftFoot].pos);
			SetHitParticle(AttackChara->HitBall[RightFoot].pos);
			// �X�R�A
			AddScore(AttackChara, Data[Shoryu].Damage);
			// ��
			PlaySound(SE_HIT1);
		}
		//�@�q�b�g�t���O�̕ύX
		AttackChara->HitFrag = true;
		break;
	case SPattack:
		// �G�̃��[�V�����ύX
		DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Down, Data[Down].Spd);
		// �_���[�W
		SubDamage(DefendChara, Data[SPattack].Damage, AttackChara->CommandInput);
		// SP�Q�[�W����
		AddSpGauge(AttackChara, Data[SPattack].Damage);
		AddSpGauge(DefendChara, Data[SPattack].Damage);
		// �G�t�F�N�g
		SetHitParticle(AttackChara->HitBall[RightFoot].pos);
		SetHitParticle(AttackChara->HitBall[LeftFoot].pos);
		// �X�R�A
		AddScore(AttackChara, Data[SPattack].Damage);
		// ��
		PlaySound(SE_HIT1);
		//�@�q�b�g�t���O�̕ύX
		AttackChara->HitFrag = true;
		break;
	case Throw:
		// �_���[�W
		SubDamage(DefendChara, Data[Throw].Damage, AttackChara->CommandInput);
		// SP�Q�[�W����
		AddSpGauge(AttackChara, Data[Throw].Damage);
		AddSpGauge(DefendChara, Data[Throw].Damage);
		// �X�R�A
		AddScore(AttackChara, Data[Throw].Damage);
		// �q�b�g�t���O�̕ύX
		AttackChara->HitFrag = true;
		break;
	default:
		break;
	}

	// �q�b�g�X�g�b�v����������
	SetHitStop();
}
