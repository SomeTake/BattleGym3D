//=============================================================================
//
// �o�g�������i�v���C���[���G�l�~�[���Ăяo�������j [battle.cpp]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#include "main.h"
#include "battle.h"
#include "input.h"
#include "camera.h"
#include "sound.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=====================================================================================================
// �����蔻��
// AttackPos = �U�������S�ʒu, DefendPos = �h�䑤���S�ʒu, AttackRange = �U�������a, DefendPos = �h�䑤���a
//=====================================================================================================
bool HitBC(D3DXVECTOR3 AttackPos, D3DXVECTOR3 DefendPos, float AttackRange, float DefendRange)
{
	// �����蔻��̒��S�̋����𑪂�
	D3DXVECTOR3 unit = AttackPos - DefendPos;
	float dist = D3DXVec3Length(&unit);

	// �����蔻��͈̔͂𑫂����������o���i���Ȃ̂�XYZ�ǂ�Ƃ��Ă������j
	float hitrange = AttackRange + DefendRange;

	// �����蔻��̒��S�̋��������͈͂𑫂��������̕���������Γ�����
	if (dist <= hitrange)
	{
		return true;
	}
	// �O��
	else
	{

	}
	return false;
}


//=============================================================================
// �L�����N�^�[���m�̓����蔻��
//=============================================================================
bool HitCheckCToC(CHARA *AttackChara, CHARA *DefendChara)
{
	float PEdistance = GetPEdistance();

	switch (AttackChara->Animation->CurrentAnimID)
	{
	case Punchi:
		// ����Ƒ���̊e���ʂƂ̔���
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			if (HitBC(AttackChara->HitBall[LeftHand].pos, DefendChara->HitBall[i].pos,
				AttackChara->HitBall[LeftHand].scl.x, DefendChara->HitBall[i].scl.x) == true)
			{
				return true;
			}
		}
		break;
	case Kick:
		// �E���Ƒ���̊e���ʂƂ̔���
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			if (HitBC(AttackChara->HitBall[RightFoot].pos, DefendChara->HitBall[i].pos,
				AttackChara->HitBall[RightFoot].scl.x, DefendChara->HitBall[i].scl.x) == true)
			{
				return true;
			}
		}
		break;
	case Shoryu:
		// �����Ƒ���̊e���ʂƂ̔���
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			// �����Ƃ̔���
			if (HitBC(AttackChara->HitBall[LeftFoot].pos, DefendChara->HitBall[i].pos,
				AttackChara->HitBall[LeftFoot].scl.x, DefendChara->HitBall[i].scl.x) == true)
			{
				return true;
			}
			// �E���Ƃ̔���
			else if (HitBC(AttackChara->HitBall[RightFoot].pos, DefendChara->HitBall[i].pos,
				AttackChara->HitBall[RightFoot].scl.x, DefendChara->HitBall[i].scl.x) == true)
			{
				return true;
			}
		}
		break;
	case Throw:
		if (PEdistance <= THROW_VALUE)
		{
			if (DefendChara->Animation->PreventAnimID != ThrowedPose)
			{
				DefendChara->Animation->ChangeAnimation(DefendChara->Animation, ThrowedPose, Data[ThrowedPose].Spd);
			}
			return true;
		}
		else
		{
			AttackChara->Animation->ChangeAnimation(AttackChara->Animation, Miss, Data[Miss].Spd);
		}
		break;
	case SPattack:
		// �����Ƒ���̊e���ʂƂ̔���
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			// �����Ƃ̔���
			if (HitBC(AttackChara->HitBall[LeftFoot].pos, DefendChara->HitBall[i].pos,
				AttackChara->HitBall[LeftFoot].scl.x, DefendChara->HitBall[i].scl.x) == true)
			{
				return true;
			}
			// �E���Ƃ̔���
			else if (HitBC(AttackChara->HitBall[RightFoot].pos, DefendChara->HitBall[i].pos,
				AttackChara->HitBall[RightFoot].scl.x, DefendChara->HitBall[i].scl.x) == true)
			{
				return true;
			}
		}
		break;
	default:
		break;
	}

	return false;
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

			// �X�R�A
			AddScore(AttackChara, Data[Punchi].Damage);
			AddScore(DefendChara, Data[Punchi].Damage);
			// ��
			PlaySound(SE_DEFEND0, 0, 0);
		}
		else
		{
			// �G�̃��[�V�����ύX
			DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Damage, Data[Damage].Spd);
			// �_���[�W
			SubDamage(DefendChara, Data[Punchi].Damage);
			// SP�Q�[�W����
			AddSpGauge(AttackChara, Data[Punchi].Damage);
			AddSpGauge(DefendChara, Data[Punchi].Damage);
			// �G�t�F�N�g
			//SetEffect(&AttackChara->effect, AttackChara->HitBall[RightHand].pos, EFFECT_WIDTH, EFFECT_HEIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), hit00);
			// �X�R�A
			AddScore(AttackChara, Data[Punchi].Damage);
			// ��
			PlaySound(SE_HIT0, 0, 0);
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

			// �X�R�A
			AddScore(AttackChara, Data[Kick].Damage);
			AddScore(DefendChara, Data[Kick].Damage);
			// ��
			PlaySound(SE_DEFEND0, 0, 0);
		}
		else
		{
			// �G�̃��[�V�����ύX
			DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Damage, Data[Damage].Spd);
			// �_���[�W
			SubDamage(DefendChara, Data[Kick].Damage);
			// SP�Q�[�W����
			AddSpGauge(AttackChara, Data[Kick].Damage);
			AddSpGauge(DefendChara, Data[Kick].Damage);
			// �G�t�F�N�g

			// �X�R�A
			AddScore(AttackChara, Data[Kick].Damage);
			// ��
			PlaySound(SE_HIT0, 0, 0);
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
			SubDamage(DefendChara, (int)(Data[Shoryu].Damage * 0.1f));
			// �G�t�F�N�g

			// �X�R�A
			AddScore(AttackChara, Data[Shoryu].Damage);
			AddScore(DefendChara, Data[Shoryu].Damage);
			// ��
			PlaySound(SE_DEFEND0, 0, 0);
		}
		else
		{
			// �G�̃��[�V�����ύX
			DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Down, Data[Down].Spd);
			// �_���[�W
			SubDamage(DefendChara, Data[Shoryu].Damage);
			// SP�Q�[�W����
			AddSpGauge(AttackChara, Data[Shoryu].Damage);
			AddSpGauge(DefendChara, Data[Shoryu].Damage);
			// �G�t�F�N�g

			// �X�R�A
			AddScore(AttackChara, Data[Shoryu].Damage);
			// ��
			PlaySound(SE_HIT1, 0, 0);
		}
		//�@�q�b�g�t���O�̕ύX
		AttackChara->HitFrag = true;
		break;
	case SPattack:
		// �G�̃��[�V�����ύX
		DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Down, Data[Down].Spd);
		// �_���[�W
		SubDamage(DefendChara, Data[SPattack].Damage);
		// SP�Q�[�W����
		AddSpGauge(AttackChara, Data[SPattack].Damage);
		AddSpGauge(DefendChara, Data[SPattack].Damage);
		// �G�t�F�N�g

		// �X�R�A
		AddScore(AttackChara, Data[SPattack].Damage);
		// ��
		PlaySound(SE_HIT1, 0, 0);
		//�@�q�b�g�t���O�̕ύX
		AttackChara->HitFrag = true;
		break;
	default:
		break;
	}
}
//=============================================================================
// �_���[�W��^����
// ����:CHARA:�^����Ώۂ̃L�����N�^ sub :����������_���B�}�C�i�X���\�A�������ȂǂɁB
//=============================================================================
void SubDamage(CHARA *Chara, int sub)
{
	Chara->HPzan -= sub;

	//�J���X�g����
	if (Chara->HPzan < 0)
	{
		Chara->HPzan = 0;
	}
	else if (Chara->HPzan >= FULL_HP)
	{
		Chara->HPzan = FULL_HP;
	}

}


//=============================================================================
// SP�Q�[�W�̑���
// ����:CHARA:���₷�Ώۂ̃L�����N�^ add :�ǉ�����_���B�}�C�i�X���\�A�������ȂǂɁB
//=============================================================================
void AddSpGauge(CHARA *Chara, int add)
{
	Chara->SP += add;

	//�J���X�g����
	if (Chara->SP >= FULL_SPGAUGE)
	{
		Chara->SP = FULL_SPGAUGE;
	}

}

//=============================================================================
// �g�����̓����蔻��y�у_���[�W�Ȃǂ̏���
// ����:AttackChara:�U���� DefendChara:�h�䑤
//=============================================================================
void HitHadou(CHARA *AttackChara, CHARA *DefendChara)
{
	// �e���m�̓����蔻��
	if (DefendChara->HadouBullet.use == true)
	{
		if (HitBC(AttackChara->HadouBullet.pos, DefendChara->HadouBullet.pos, AttackChara->HadouBullet.scl.x, DefendChara->HadouBullet.scl.x) == true)
		{
			// �G�t�F�N�g
			// �g�p�t���O�̕ύX
			AttackChara->HadouBullet.use = false;
			DefendChara->HadouBullet.use = false;

			// �X�R�A
			AddScore(AttackChara, Data[Hadou].Damage);
			AddScore(DefendChara, Data[Hadou].Damage);

			// ��
			PlaySound(SE_HIT1, 0, 0);
		}
	}
	// �e�Ƒ���L�����Ƃ̓����蔻��
	for (int i = 0; i < HIT_CHECK_NUM; i++)
	{
		if (HitBC(AttackChara->HadouBullet.pos, DefendChara->HitBall[i].pos, AttackChara->HadouBullet.scl.x, DefendChara->HitBall[i].scl.x) == true)
		{
			// �G�̏�Ԋm�F
			if (DefendChara->Animation->CurrentAnimID == Guard || DefendChara->Animation->CurrentAnimID == Down
				|| DefendChara->Animation->CurrentAnimID == Downpose || DefendChara->Animation->CurrentAnimID == Getup)
			{
				// �_���[�W
				SubDamage(DefendChara, (int)(Data[Hadou].Damage * 0.1f));

				// �G�t�F�N�g

				// �X�R�A
				AddScore(AttackChara, Data[Hadou].Damage);
				AddScore(DefendChara, Data[Hadou].Damage);

				// ��
				PlaySound(SE_DEFEND0, 0, 0);
			}
			else
			{
				// �G�̃��[�V�����ύX
				DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Damage, Data[Damage].Spd);
				// �_���[�W
				SubDamage(DefendChara, Data[Hadou].Damage);

				// �G�t�F�N�g

				// �X�R�A
				AddScore(AttackChara, Data[Hadou].Damage);

				// ��
				PlaySound(SE_HIT0, 0, 0);
			}
			// SP�Q�[�W����
			AddSpGauge(DefendChara, Data[Hadou].Damage);
			AddSpGauge(AttackChara, Data[Hadou].Damage);

			//�@�g�p�t���O�̕ύX
			AttackChara->HadouBullet.use = false;
			break;
		}
	}
}

//=============================================================================
// �ȒP����	���� Chara:�L�����N�^�\���� ControllerNum:���삷��R���g���[���ԍ�
//=============================================================================
void EasyInput(CHARA *Chara, int ControllerNum)
{
	int *phase = GetPhase();

	// �|�[�Y��ʂ�
	if (*phase == PhaseTutorial)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(ControllerNum, BUTTON_M))
		{
			SetPhase(PhaseTitle);
			StopSound(BGM_TUTORIAL, 0);
			PlaySound(BGM_TITLE, 1, 1);
			ReInit();
		}
	}
	else if (*phase == PhaseGame)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(ControllerNum, BUTTON_M))
		{
			SetPhase(PhasePause);
			StopSound(BGM_BATTLE, 0);
		}
	}
	else if (*phase == PhaseTraining)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(ControllerNum, BUTTON_M))
		{
			SetPhase(PhaseTrainingPause);
			StopSound(BGM_TRAINING, 0);
		}
	}


	if (Chara->graceflag == true)
	{
		Chara->graceframe++;
		// ���͗P�\�͈͓��ɒǉ����͂��Ȃ��ꂽ�ꍇ
		if (Chara->graceframe < GRACE_VALUE)
		{
			switch (Chara->gracetype)
			{
			case Punchi:
				if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K)
					|| IsButtonTriggered(ControllerNum, BUTTON_B))
				{
					Chara->Animation->ChangeAnimation(Chara->Animation, Throw, Data[Throw].Spd);
					Chara->graceflag = false;
					Chara->graceframe = 0;
				}
				break;
			case Kick:
				if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I)
					|| IsButtonTriggered(ControllerNum, BUTTON_A))
				{
					Chara->Animation->ChangeAnimation(Chara->Animation, Throw, Data[Throw].Spd);
					Chara->graceflag = false;
					Chara->graceframe = 0;
				}
				break;
			case Hadou:
				if (Chara->SP == FULL_SPGAUGE)
				{
					if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O)
						|| IsButtonTriggered(ControllerNum, BUTTON_C))
					{
						Chara->Animation->ChangeAnimation(Chara->Animation, SPattack, Data[SPattack].Spd);
						Chara->graceflag = false;
						Chara->graceframe = 0;
					}
				}
				break;
			case Shoryu:
				if (Chara->SP == FULL_SPGAUGE)
				{
					if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L)
						|| IsButtonTriggered(ControllerNum, BUTTON_X))
					{
						Chara->Animation->ChangeAnimation(Chara->Animation, SPattack, Data[SPattack].Spd);
						Chara->graceflag = false;
						Chara->graceframe = 0;
					}
				}
				break;
			default:
				break;
			}
		}
		// �ǉ����͂�����Ȃ������ꍇ
		else if(Chara->graceframe >= GRACE_VALUE)
		{
			switch (Chara->gracetype)
			{
			case Punchi:
				Chara->Animation->ChangeAnimation(Chara->Animation, Punchi, Data[Punchi].Spd);
				Chara->graceflag = false;
				Chara->graceframe = 0;
				break;
			case Kick:
				Chara->Animation->ChangeAnimation(Chara->Animation, Kick, Data[Kick].Spd);
				Chara->graceflag = false;
				Chara->graceframe = 0;
				break;
			case Hadou:
				// ���˂���Ă��Ȃ��ꍇ
				if (Chara->HadouBullet.use == false)
				{
					Chara->Animation->ChangeAnimation(Chara->Animation, Hadou, Data[Hadou].Spd);
					Chara->graceflag = false;
					Chara->graceframe = 0;
				}
				break;
			case Shoryu:
				Chara->Animation->ChangeAnimation(Chara->Animation, Shoryu, Data[Shoryu].Spd);
				Chara->graceflag = false;
				Chara->graceframe = 0;
				break;
			default:
				break;
			}
		}
	}

	switch (Chara->Animation->CurrentAnimID)
	{
	case Idle:
		// �ړ�
		// �O
		if (GetKeyboardPress(ControllerNum == 0 ? DIK_RIGHT : DIK_A) 
			|| IsButtonPressed(ControllerNum, BUTTON_RIGHT) || IsButtonPressed(ControllerNum, STICK_RIGHT))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Frontwalk, Data[Frontwalk].Spd);
		}
		// ���
		else if (GetKeyboardPress(ControllerNum == 0 ? DIK_LEFT : DIK_D)
			|| IsButtonPressed(ControllerNum, BUTTON_LEFT) || IsButtonPressed(ControllerNum, STICK_LEFT))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Backwalk, Data[Backwalk].Spd);
		}
		// ��O
		else if (GetKeyboardPress(ControllerNum == 0 ? DIK_DOWN : DIK_S)
			|| IsButtonPressed(ControllerNum, BUTTON_DOWN) || IsButtonPressed(ControllerNum, STICK_DOWN))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Rightstep, Data[Rightstep].Spd);
		}
		// ��
		else if (GetKeyboardPress(ControllerNum == 0 ? DIK_UP : DIK_W)
			|| IsButtonPressed(ControllerNum, BUTTON_UP) || IsButtonPressed(ControllerNum, STICK_UP))
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
		else
		{
		}
		break;
	case Frontwalk:
		// �O
		if (GetKeyboardPress(ControllerNum == 0 ? DIK_RIGHT : DIK_A)
			|| IsButtonPressed(ControllerNum, BUTTON_RIGHT) || IsButtonPressed(ControllerNum, STICK_RIGHT))
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
		break;
	case Backwalk:
		// ���
		if (GetKeyboardPress(ControllerNum == 0 ? DIK_LEFT : DIK_D) 
			|| IsButtonPressed(ControllerNum, BUTTON_LEFT) || IsButtonPressed(ControllerNum, STICK_LEFT))
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
		break;
	case Rightstep:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		}
		break;
	case Leftstep:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		}
		break;
	case Guard:
		if (GetKeyboardPress(ControllerNum == 0 ? DIK_G : DIK_P)
			|| IsButtonPressed(ControllerNum, BUTTON_Y))
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
	case Damage:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
			Chara->HadouBullet.frame = 0;
		}
		break;
	case Down:
		// �A�j���[�V�����I���ŋN���オ��Ɉڍs
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Downpose, Data[Downpose].Spd);
			// ���[�V�������W�ɃL�����N�^�[���W�����킹��
			Chara->pos = D3DXVECTOR3(Chara->HitBall[Hips].pos.x, 0.0f, Chara->HitBall[Hips].pos.z);
		}
		break;
	case Downpose:
		// �A�j���[�V�����I���ŋN���オ��Ɉڍs
		if (Chara->Animation->MotionEnd == true && Chara->HPzan > 0)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Getup, Data[Getup].Spd);
		}
	case Getup:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
			Chara->HadouBullet.frame = 0;
		}
		break;
	case Punchi:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
			Chara->HitFrag = false;
		}
		break;
	case Kick:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
			Chara->HitFrag = false;
		}
		break;
	case Hadou:
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
		break;
	case Shoryu:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
			Chara->HitFrag = false;
		}
		break;
	case SPattack:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
			// SP�Q�[�W����
			AddSpGauge(Chara, -FULL_SPGAUGE);
			// ���[�V�������W�ɃL�����N�^�[���W�����킹��
			Chara->pos = D3DXVECTOR3(Chara->HitBall[Hips].pos.x, 0.0f, Chara->HitBall[Hips].pos.z);
			Chara->HitFrag = false;
		}
		break;
	case Throw:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
			Chara->HitFrag = false;
		}
		break;
	case Win:
		break;
	case Miss:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
			Chara->HitFrag = false;
		}
		break;
	case ThrowedPose:
		if (Chara->Animation->PreventAnimID != ThrowedPose)
		{
			Chara->framecount++;
		}
		// ��莞�Ԍo�߂ő���̓����A�j���[�V�����ɍ��킹�ă_�E�����[�V�����Ɉڍs
		if (Chara->framecount == THROW_FRAME)
		{
			SubDamage(Chara, Data[Throw].Damage);
			Chara->Animation->ChangeAnimation(Chara->Animation, Down, Data[Down].Spd);
			Chara->framecount = 0;
		}
		break;
	default:
		break;
	}
}

//=============================================================================
// �X�R�A�̒ǉ�
// ����:add :�ǉ�����_���B�}�C�i�X���\�A�������ȂǂɁB
//=============================================================================
void AddScore(CHARA *Chara, int add)
{
	Chara->score += add;

	//�J���X�g����
	if (Chara->score > FULL_SCORE)
	{
		Chara->score = FULL_SCORE;
	}
	else if (Chara->score < 0)
	{
		Chara->score = 0;
	}
}

//=============================================================================
// �o�g���pAI ����1:���� ����2:����̏�� �ɂ���ď󋵂𔻒f����
//=============================================================================
void BattleAI(CHARA *AIChara, CHARA *AnotherChara)
{
	float PEdist = GetPEdistance();
	bool AttackFlag = false;

	// ����̍U������
	if (AnotherChara->Animation->CurrentAnimID == Punchi
		|| AnotherChara->Animation->CurrentAnimID == Kick
		|| AnotherChara->Animation->CurrentAnimID == Shoryu
		|| AnotherChara->Animation->CurrentAnimID == SPattack
		|| AnotherChara->HadouBullet.use == true)
	{
		AttackFlag = true;
	}
	else
	{
		AttackFlag = false;
	}

	int ActRand = rand() % 300;

	switch (AIChara->Animation->CurrentAnimID)
	{
	case Idle:
		if (AttackFlag == true)
		{
			switch(ActRand)
			{
			case 0:
				AIChara->Animation->ChangeAnimation(AIChara->Animation, Guard, Data[Guard].Spd);
				break;
			case 1:
				AIChara->Animation->ChangeAnimation(AIChara->Animation, Rightstep, Data[Rightstep].Spd);
				break;
			case 2:
				AIChara->Animation->ChangeAnimation(AIChara->Animation, Leftstep, Data[Leftstep].Spd);
				break;
			default:
				break;
			}
		}
		else
		{
			// �ߋ���
			if (PEdist < MAX_DISTANCE * 0.2f)
			{
				switch (ActRand)
				{
				case 0:
					AIChara->Animation->ChangeAnimation(AIChara->Animation, Kick, Data[Kick].Spd);
					break;
				case 1:
					AIChara->Animation->ChangeAnimation(AIChara->Animation, Punchi, Data[Punchi].Spd);
					break;
				case 2:
					AIChara->Animation->ChangeAnimation(AIChara->Animation, Hadou, Data[Hadou].Spd);
					break;
				case 3:
					AIChara->Animation->ChangeAnimation(AIChara->Animation, Shoryu, Data[Shoryu].Spd);
					break;
				case 4:
					AIChara->Animation->ChangeAnimation(AIChara->Animation, Throw, Data[Throw].Spd);
					break;
				case 5:
					if (AIChara->SP == FULL_SPGAUGE)
					{
						AIChara->Animation->ChangeAnimation(AIChara->Animation, SPattack, Data[SPattack].Spd);
					}
					break;
				default:
					break;
				}
			}
			// ������
			else if (PEdist < MAX_DISTANCE * 0.5f || PEdist > MAX_DISTANCE * 0.2f)
			{
				if (ActRand == 0 && AIChara->HadouBullet.use == false)
				{
					AIChara->Animation->ChangeAnimation(AIChara->Animation, Hadou, Data[Hadou].Spd);
				}
				else if(ActRand == 1)
				{
					AIChara->Animation->ChangeAnimation(AIChara->Animation, Frontwalk, Data[Frontwalk].Spd);
				}
			}
			// ������
			else
			{
				AIChara->Animation->ChangeAnimation(AIChara->Animation, Frontwalk, Data[Frontwalk].Spd);
			}
		}
		break;
	case Frontwalk:
		if (PEdist < MAX_DISTANCE * 0.5f || PEdist > MAX_DISTANCE * 0.2f)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
		}
		break;
	case Backwalk:
		if (PEdist > MAX_DISTANCE * 0.5f)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
		}
		break;
	case Rightstep:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
		}
		break;
	case Leftstep:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
		}
		break;
	case Guard:
		if (AttackFlag == false)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
		}
		break;
	case Damage:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
			AIChara->HadouBullet.frame = 0;
		}
		break;
	case Down:
		// �A�j���[�V�����I���ŋN���オ��Ɉڍs
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Downpose, Data[Downpose].Spd);
			// ���[�V�������W�ɃL�����N�^�[���W�����킹��
			AIChara->pos = D3DXVECTOR3(AIChara->HitBall[Hips].pos.x, 0.0f, AIChara->HitBall[Hips].pos.z);
		}
		break;
	case Downpose:
		// �A�j���[�V�����I���ŋN���オ��Ɉڍs
		if (AIChara->Animation->MotionEnd == true && AIChara->HPzan > 0)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Getup, Data[Getup].Spd);
		}
	case Getup:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
			AIChara->HadouBullet.frame = 0;
		}
		break;
	case Punchi:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
			AIChara->HitFrag = false;
		}
		break;
	case Kick:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
			AIChara->HitFrag = false;
		}
		break;
	case Hadou:
		// ���t���[���o�߂Œe���o��
		AIChara->HadouBullet.frame++;
		if (AIChara->HadouBullet.frame == FIRE_FRAME)
		{
			SetHadou(&AIChara->HadouBullet, AIChara->HitBall, AIChara->rot);
		}
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
			AIChara->HadouBullet.frame = 0;
			AIChara->HitFrag = false;
		}
		break;
	case Shoryu:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
			AIChara->HitFrag = false;
		}
		break;
	case SPattack:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
			// SP�Q�[�W����
			AddSpGauge(AIChara, -FULL_SPGAUGE);
			// ���[�V�������W�ɃL�����N�^�[���W�����킹��
			AIChara->pos = D3DXVECTOR3(AIChara->HitBall[Hips].pos.x, 0.0f, AIChara->HitBall[Hips].pos.z);
			AIChara->HitFrag = false;
		}
		break;
	case Throw:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
			AIChara->HitFrag = false;
		}
		break;
	case Win:
		break;
	case Miss:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
			AIChara->HitFrag = false;
		}
		break;
	case ThrowedPose:
		if (AIChara->Animation->PreventAnimID != ThrowedPose)
		{
			AIChara->framecount++;
		}
		// ��莞�Ԍo�߂ő���̓����A�j���[�V�����ɍ��킹�ă_�E�����[�V�����Ɉڍs
		if (AIChara->framecount == THROW_FRAME)
		{
			SubDamage(AIChara, Data[Throw].Damage);
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Down, Data[Down].Spd);
			AIChara->framecount = 0;
		}
		break;
	default:
		AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Idle);
		break;
	}

}

//=============================================================================
// �`���[�g���A�����[�h
//=============================================================================
void SetupTutorial(CHARA *Chara, REDGAUGE *Gauge)
{
	if (Chara->HPzan <= 0)
	{
		Chara->HPzan = 0;
	}

	if (Chara->HPzan == Gauge->value)
	{
		Chara->damagecount++;
		if (Chara->damagecount >= 180)
		{
			if (Chara->HP > Chara->HPzan)
			{
				Chara->HPzan += HPHEAL;
				Gauge->value += HPHEAL;
			}
		}
	}
	else
	{
		Chara->damagecount = 0;
	}

	Chara->SP = FULL_SPGAUGE;

}