//=============================================================================
//
// �o�g�������i�v���C���[���G�l�~�[���Ăяo�������j [battle.cpp]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "battle.h"
#include "camera.h"
#include "sound.h"
#include "particle.h"
#include "game.h"
#include "action.h"

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
bool HitSphere(D3DXVECTOR3 AttackPos, D3DXVECTOR3 DefendPos, float AttackRange, float DefendRange)
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
			if (HitSphere(AttackChara->HitBall[LeftHand].pos, DefendChara->HitBall[i].pos,
				AttackChara->HitBall[LeftHand].scl.x, DefendChara->HitBall[i].scl.x) == true)
			{
				return true;
			}
		}
		break;
	case Straight:
		// �E��Ƒ���̊e���ʂƂ̔���
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			if (HitSphere(AttackChara->HitBall[RightHand].pos, DefendChara->HitBall[i].pos,
				AttackChara->HitBall[RightHand].scl.x, DefendChara->HitBall[i].scl.x) == true)
			{
				return true;
			}
		}
		break;
	case Upper:
		// ����Ƒ���̊e���ʂƂ̔���
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			if (HitSphere(AttackChara->HitBall[LeftHand].pos, DefendChara->HitBall[i].pos,
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
			if (HitSphere(AttackChara->HitBall[RightFoot].pos, DefendChara->HitBall[i].pos,
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
			if (HitSphere(AttackChara->HitBall[LeftFoot].pos, DefendChara->HitBall[i].pos,
				AttackChara->HitBall[LeftFoot].scl.x, DefendChara->HitBall[i].scl.x) == true)
			{
				return true;
			}
			// �E���Ƃ̔���
			else if (HitSphere(AttackChara->HitBall[RightFoot].pos, DefendChara->HitBall[i].pos,
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
			if (HitSphere(AttackChara->HitBall[LeftFoot].pos, DefendChara->HitBall[i].pos,
				AttackChara->HitBall[LeftFoot].scl.x, DefendChara->HitBall[i].scl.x) == true)
			{
				return true;
			}
			// �E���Ƃ̔���
			else if (HitSphere(AttackChara->HitBall[RightFoot].pos, DefendChara->HitBall[i].pos,
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
// �g�����̓����蔻��y�у_���[�W�Ȃǂ̏���
// ����:AttackChara:�U���� DefendChara:�h�䑤
//=============================================================================
void HitHadou(CHARA *AttackChara, CHARA *DefendChara)
{
	// �e���m�̓����蔻��
	if (DefendChara->HadouBullet.use == true)
	{
		if (HitSphere(AttackChara->HadouBullet.pos, DefendChara->HadouBullet.pos, AttackChara->HadouBullet.scl.x, DefendChara->HadouBullet.scl.x) == true)
		{
			// �G�t�F�N�g
			SetHitParticle(AttackChara->HadouBullet.pos);
			SetHitParticle(DefendChara->HadouBullet.pos);
			// �g�p�t���O�̕ύX
			AttackChara->HadouBullet.use = false;
			DefendChara->HadouBullet.use = false;

			// �X�R�A
			AddScore(AttackChara, Data[Hadou].Damage);
			AddScore(DefendChara, Data[Hadou].Damage);

			// ��
			PlaySound(SE_HIT1);
		}
	}
	// �e�Ƒ���L�����Ƃ̓����蔻��
	for (int i = 0; i < HIT_CHECK_NUM; i++)
	{
		if (HitSphere(AttackChara->HadouBullet.pos, DefendChara->HitBall[i].pos, AttackChara->HadouBullet.scl.x, DefendChara->HitBall[i].scl.x) == true)
		{
			// �G�̏�Ԋm�F
			if (DefendChara->Animation->CurrentAnimID == Guard || DefendChara->Animation->CurrentAnimID == Down
				|| DefendChara->Animation->CurrentAnimID == Downpose || DefendChara->Animation->CurrentAnimID == Getup)
			{
				// �_���[�W
				SubDamage(DefendChara, (int)(Data[Hadou].Damage * 0.1f), AttackChara->CommandInput);

				// �G�t�F�N�g
				SetGuardParticle(DefendChara->HitBall[i].pos);

				// �X�R�A
				AddScore(AttackChara, Data[Hadou].Damage);
				AddScore(DefendChara, Data[Hadou].Damage);

				// ��
				PlaySound(SE_DEFEND0);
			}
			else
			{
				// �G�̃��[�V�����ύX
				DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Damage, Data[Damage].Spd);
				// �_���[�W
				SubDamage(DefendChara, Data[Hadou].Damage, AttackChara->CommandInput);

				// �G�t�F�N�g
				SetHitParticle(AttackChara->HadouBullet.pos);

				// �X�R�A
				AddScore(AttackChara, Data[Hadou].Damage);

				// ��
				PlaySound(SE_HIT0);
			}
			// SP�Q�[�W����
			AddSpGauge(DefendChara, Data[Hadou].Damage);
			AddSpGauge(AttackChara, Data[Hadou].Damage);

			//�@�g�p�t���O�̕ύX
			AttackChara->HadouBullet.use = false;

			// �q�b�g�X�g�b�v����������
			SetHitStop();
			break;
		}
	}
}

//=============================================================================
// �U�����[�V�����ɍ��킹�������蔻�蔭���t���[���̍X�V
//=============================================================================
void UpdateAttackCollision(CHARA *Chara)
{
	if (Chara->Animation->CurrentAnimID == Punchi || Chara->Animation->CurrentAnimID == Straight
		|| Chara->Animation->CurrentAnimID == Upper || Chara->Animation->CurrentAnimID == Kick
		|| Chara->Animation->CurrentAnimID == Hadou || Chara->Animation->CurrentAnimID == Shoryu
		|| Chara->Animation->CurrentAnimID == SPattack)
	{
		Chara->framecount++;
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->framecount = 0;
		}
	}
	else if(Chara->Animation->CurrentAnimID == Damage || Chara->Animation->CurrentAnimID == Down)
	{
		Chara->framecount = 0;
	}
}

//=============================================================================
// �_���[�W��^����i�R�}���h���͂̏ꍇ�_���[�W��1.1�{�ɂ���j
// ����:CHARA:�^����Ώۂ̃L�����N�^ sub :����������_���B�}�C�i�X���\�A�������ȂǂɁB AtkCharaInput:�U�����̑��샂�[�h
//=============================================================================
void SubDamage(CHARA *Chara, int sub, bool AtkCharaInput)
{
	int *Phase = GetPhase();

	if (*Phase != PhaseReplay)
	{
		if (AtkCharaInput == true)
		{
			Chara->HPzan -= (int)(sub * 1.1f);
		}
		else
		{
			Chara->HPzan -= sub;
		}

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

	// �U�����̓����蔻�蔭������
	UpdateAttackCollision(AIChara);

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

	int ActRand = rand() % 100;

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
						// SP�Q�[�W����
						AddSpGauge(AIChara, -FULL_SPGAUGE);
						PlaySound(SE_EFFECT0);
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
		RightstepAction(AIChara);
		break;
	case Leftstep:
		LeftstepAction(AIChara);
		break;
	case Guard:
		if (AttackFlag == false)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
		}
		break;
	case Damage:
		DamageAction(AIChara);
		break;
	case Down:
		DownAction(AIChara);
		break;
	case Downpose:
		DownposeAction(AIChara);
		break;
	case Getup:
		GetupAction(AIChara);
		break;
	case Punchi:
		SetAttackParticle(AIChara->HitBall[LeftHand].pos);
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
			AIChara->HitFrag = false;
		}
		break;
	case Straight:
		SetAttackParticle(AIChara->HitBall[RightHand].pos);
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
			AIChara->HitFrag = false;
		}
		break;
	case Upper:
		UpperAction(AIChara);
		break;
	case Kick:
		KickAction(AIChara);
		break;
	case Hadou:
		HadouAction(AIChara);
		break;
	case Shoryu:
		ShoryuAction(AIChara);
		break;
	case SPattack:
		SPattackAction(AIChara);
		break;
	case Throw:
		ThrowAction(AIChara);
		break;
	case Win:
		WinAction(AIChara);
		break;
	case Miss:
		MissAction(AIChara);
		break;
	case ThrowedPose:
		ThrowedposeAction(AIChara);
		break;
	default:
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
				SubDamage(Chara, -HPHEAL, Chara->CommandInput);
				Gauge->value += HPHEAL;
				if (Gauge->value >= FULL_HP)
				{
					Gauge->value = FULL_HP;
				}
			}
		}
	}
	else
	{
		Chara->damagecount = 0;
	}

	Chara->SP = FULL_SPGAUGE;

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
			StopSound(BGM_TUTORIAL);
			PlaySound(BGM_TITLE);
			ReInit();
		}
	}
	else if (*phase == PhaseGame)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(ControllerNum, BUTTON_M))
		{
			SetPhase(PhasePause);
			StopSound(BGM_BATTLE);
		}
	}
	else if (*phase == PhaseTraining)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(ControllerNum, BUTTON_M))
		{
			SetPhase(PhaseTrainingPause);
			StopSound(BGM_TRAINING);
		}
	}

	// �U�����̓����蔻�蔭������
	UpdateAttackCollision(Chara);

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
						// SP�Q�[�W����
						AddSpGauge(Chara, -FULL_SPGAUGE);
						PlaySound(SE_EFFECT0);
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
						// SP�Q�[�W����
						AddSpGauge(Chara, -FULL_SPGAUGE);
						PlaySound(SE_EFFECT0);
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
		else if (Chara->graceframe >= GRACE_VALUE)
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
		IdleAction(Chara, ControllerNum);
		break;
	case Frontwalk:
		FrontwalkAction(Chara, ControllerNum);
		break;
	case Backwalk:
		BackwalkAction(Chara, ControllerNum);
		break;
	case Rightstep:
		RightstepAction(Chara);
		break;
	case Leftstep:
		LeftstepAction(Chara);
		break;
	case Guard:
		GuardAction(Chara, ControllerNum);
		break;
	case Damage:
		DamageAction(Chara);
		break;
	case Down:
		DownAction(Chara);
		break;
	case Downpose:
		DownposeAction(Chara);
		break;
	case Getup:
		GetupAction(Chara);
		break;
	case Punchi:
		PunchiAction(Chara, ControllerNum);
		break;
	case Straight:
		StraightAction(Chara, ControllerNum);
		break;
	case Upper:
		UpperAction(Chara);
		break;
	case Kick:
		KickAction(Chara);
		break;
	case Hadou:
		HadouAction(Chara);
		break;
	case Shoryu:
		ShoryuAction(Chara);
		break;
	case SPattack:
		SPattackAction(Chara);
		break;
	case Throw:
		ThrowAction(Chara);
		break;
	case Win:
		WinAction(Chara);
		break;
	case Miss:
		MissAction(Chara);
		break;
	case ThrowedPose:
		ThrowedposeAction(Chara);
		break;
	default:
		break;
	}
}

//=============================================================================
// �R�}���h���샂�[�h
//=============================================================================
void CommandInput(CHARA *Chara, int ControllerNum)
{
	int *phase = GetPhase();

	// �|�[�Y��ʂ�
	if (*phase == PhaseTutorial)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(ControllerNum, BUTTON_M))
		{
			SetPhase(PhaseTitle);
			StopSound(BGM_TUTORIAL);
			PlaySound(BGM_TITLE);
			ReInit();
		}
	}
	else if (*phase == PhaseGame)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(ControllerNum, BUTTON_M))
		{
			SetPhase(PhasePause);
			StopSound(BGM_BATTLE);
		}
	}
	else if (*phase == PhaseTraining)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(ControllerNum, BUTTON_M))
		{
			SetPhase(PhaseTrainingPause);
			StopSound(BGM_TRAINING);
		}
	}


	int input = 0;

	// �������{�^���ɉ����āA�r�b�g���I���ɂ���
	if (IsButtonTriggered(ControllerNum, BUTTON_UP))
	{
		input |= BUTTON_UP;
	}
	else if (IsButtonTriggered(ControllerNum, BUTTON_DOWN))
	{
		input |= BUTTON_DOWN;
	}
	// 2P���͍��E�𔽓]����
	else if (IsButtonPressed(ControllerNum, BUTTON_RIGHT))
	{
		input |= (ControllerNum == 0 ? BUTTON_RIGHT : BUTTON_LEFT);
	}
	else if (IsButtonPressed(ControllerNum, BUTTON_LEFT))
	{
		input |= (ControllerNum == 0 ? BUTTON_LEFT : BUTTON_RIGHT);
	}
	else if (IsButtonPressed(ControllerNum, BUTTON_LEFTUP))
	{
		input |= (ControllerNum == 0 ? BUTTON_LEFTUP : BUTTON_RIGHTUP);
	}
	else if (IsButtonPressed(ControllerNum, BUTTON_RIGHTUP))
	{
		input |= (ControllerNum == 0 ? BUTTON_RIGHTUP : BUTTON_LEFTUP);
	}
	else if (IsButtonPressed(ControllerNum, BUTTON_LEFTDOWN))
	{
		input |= (ControllerNum == 0 ? BUTTON_LEFTDOWN : BUTTON_RIGHTDOWN);
	}
	else if (IsButtonPressed(ControllerNum, BUTTON_RIGHTDOWN))
	{
		input |= (ControllerNum == 0 ? BUTTON_RIGHTDOWN : BUTTON_LEFTDOWN);
	}

	// �����\���L�[����͂��Ă��Ȃ��ꍇ
	if (!IsButtonTriggered(ControllerNum, BUTTON_UP) && !IsButtonTriggered(ControllerNum, BUTTON_DOWN)
		&& !IsButtonTriggered(ControllerNum, BUTTON_RIGHT) && !IsButtonTriggered(ControllerNum, BUTTON_LEFT)
		&& !IsButtonTriggered(ControllerNum, BUTTON_RIGHTUP) && !IsButtonTriggered(ControllerNum, BUTTON_LEFTUP)
		&& !IsButtonTriggered(ControllerNum, BUTTON_RIGHTDOWN) && !IsButtonTriggered(ControllerNum, BUTTON_LEFTDOWN))
	{
		input |= BUTTON_DEFAULT;
	}

	if (IsButtonTriggered(ControllerNum, BUTTON_A))
	{
		input |= BUTTON_A;
	}
	if (IsButtonTriggered(ControllerNum, BUTTON_B))
	{
		input |= BUTTON_B;
	}
	if (IsButtonTriggered(ControllerNum, BUTTON_C))
	{
		input |= BUTTON_C;
	}
	if (IsButtonTriggered(ControllerNum, BUTTON_X))
	{
		input |= BUTTON_X;
	}
	if (IsButtonTriggered(ControllerNum, BUTTON_START))
	{
		input |= BUTTON_START;
	}
	if (IsButtonTriggered(ControllerNum, BUTTON_M))
	{
		input |= BUTTON_M;
	}
	if (IsButtonPressed(ControllerNum, BUTTON_Y))
	{
		input |= BUTTON_Y;
	}

	// �z�������E�Ɉړ�������
	for (int i = INPUT_MAX - 1; i > 0; i--)
	{
		Chara->Input[i] = Chara->Input[i - 1];
	}

	// �z��0�Ԃɍ����͂������̂�����
	Chara->Input[0] = input;


	// �U�����̓����蔻�蔭������
	UpdateAttackCollision(Chara);

	switch (Chara->Animation->CurrentAnimID)
	{
	case Idle:
		IdleAction(Chara, ControllerNum);
		break;
	case Frontwalk:
		FrontwalkAction(Chara, ControllerNum);
		break;
	case Backwalk:
		BackwalkAction(Chara, ControllerNum);
		break;
	case Rightstep:
		RightstepAction(Chara);
		break;
	case Leftstep:
		LeftstepAction(Chara);
		break;
	case Guard:
		GuardAction(Chara, ControllerNum);
		break;
	case Damage:
		DamageAction(Chara);
		break;
	case Down:
		DownAction(Chara);
		break;
	case Downpose:
		DownposeAction(Chara);
		break;
	case Getup:
		GetupAction(Chara);
		break;
	case Punchi:
		PunchiAction(Chara, ControllerNum);
		break;
	case Straight:
		StraightAction(Chara, ControllerNum);
		break;
	case Upper:
		UpperAction(Chara);
		break;
	case Kick:
		KickAction(Chara);
		break;
	case Hadou:
		HadouAction(Chara);
		break;
	case Shoryu:
		ShoryuAction(Chara);
		break;
	case SPattack:
		SPattackAction(Chara);
		break;
	case Throw:
		ThrowAction(Chara);
		break;
	case Win:
		WinAction(Chara);
		break;
	case Miss:
		MissAction(Chara);
		break;
	case ThrowedPose:
		ThrowedposeAction(Chara);
		break;
	}

}

//=============================================================================
// ���͂̔���
//=============================================================================
bool CheckInput(int Input[], const int *command)
{
	int i = 0;
	int j = 0;


	// �R�}���h�̍Ō�𒲂ׂ�
	while (!(command[i] & INPUT_END))
	{
		i++;
	}

	// ���͗P�\���Ԃ�ݒ肷��
	int time = command[i] & ~INPUT_END;

	// ���͗����𒲂ׂāA�R�}���h�����������͂���Ă����true����Ă��Ȃ�������false
	for (i--; i >= 0; i--)
	{
		int input = command[i];

		// ��������R�}���h�̗v�f��T��
		while (j < INPUT_MAX && (Input[j] & input) != input)
		{
			j++;
		}

		// ���͎��Ԃ𒴂�����A�����̖����ɒB������false
		if (j >= time || j == INPUT_MAX)
		{
			return false;
		}
	}

	// �R�}���h�̑S�Ă̗v�f�������邱�Ƃ��ł�����true
	return true;
}
