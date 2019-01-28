//=============================================================================
//
// �o�g�������i�v���C���[���G�l�~�[���Ăяo�������j [battle.cpp]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#include "main.h"
#include "battle.h"
#include "input.h"

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
			AddScore(AttackChara, DAMAGE_PUNCHI);
			AddScore(DefendChara, DAMAGE_PUNCHI);
		}
		else
		{
			// �G�̃��[�V�����ύX
			DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Damage, ANIM_SPD_2);
			// �_���[�W
			SubDamage(DefendChara, DAMAGE_PUNCHI);
			// SP�Q�[�W����
			AddSpGauge(AttackChara, DAMAGE_PUNCHI);
			AddSpGauge(DefendChara, DAMAGE_PUNCHI);
			// �G�t�F�N�g

			// �X�R�A
			AddScore(AttackChara, DAMAGE_PUNCHI);
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
			AddScore(AttackChara, DAMAGE_KICK);
			AddScore(DefendChara, DAMAGE_KICK);
		}
		else
		{
			// �G�̃��[�V�����ύX
			DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Damage, ANIM_SPD_1);
			// �_���[�W
			SubDamage(DefendChara, DAMAGE_KICK);
			// SP�Q�[�W����
			AddSpGauge(AttackChara, DAMAGE_KICK);
			AddSpGauge(DefendChara, DAMAGE_KICK);
			// �G�t�F�N�g

			// �X�R�A
			AddScore(AttackChara, DAMAGE_KICK);
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
			SubDamage(DefendChara, (int)(DAMAGE_SHORYU * 0.1f));
			// �G�t�F�N�g

			// �X�R�A
			AddScore(AttackChara, DAMAGE_SHORYU);
			AddScore(DefendChara, DAMAGE_SHORYU);
		}
		else
		{
			// �G�̃��[�V�����ύX
			DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Down, ANIM_SPD_1);
			// �_���[�W
			SubDamage(DefendChara, DAMAGE_SHORYU);
			// SP�Q�[�W����
			AddSpGauge(AttackChara, DAMAGE_SHORYU);
			AddSpGauge(DefendChara, DAMAGE_SHORYU);
			// �G�t�F�N�g

			// �X�R�A
			AddScore(AttackChara, DAMAGE_SHORYU);
		}
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

}


//=============================================================================
// SP�Q�[�W�̑���
// ����:CHARA:���₷�Ώۂ̃L�����N�^ add :�ǉ�����_���B�}�C�i�X���\�A�������ȂǂɁB
//=============================================================================
void AddSpGauge(CHARA *Chara, int add)
{
	Chara->SP += add;

	//�J���X�g����
	if (Chara->SP >= FULL_SPGUAGE)
	{
		Chara->SP = FULL_SPGUAGE;
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
			AddScore(AttackChara, DAMAGE_HADOU);
			AddScore(DefendChara, DAMAGE_HADOU);
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
				SubDamage(DefendChara, (int)(DAMAGE_HADOU * 0.1f));

				// �G�t�F�N�g

				// �X�R�A
				AddScore(AttackChara, DAMAGE_HADOU);
				AddScore(DefendChara, DAMAGE_HADOU);
			}
			else
			{
				// �G�̃��[�V�����ύX
				DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Damage, ANIM_SPD_1);
				// �_���[�W
				SubDamage(DefendChara, DAMAGE_HADOU);

				// �G�t�F�N�g

				// �X�R�A
				AddScore(AttackChara, DAMAGE_HADOU);
			}
			// SP�Q�[�W����
			AddSpGauge(DefendChara, DAMAGE_HADOU);
			AddSpGauge(AttackChara, DAMAGE_HADOU);

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
	switch (Chara->Animation->CurrentAnimID)
	{
	case Idle:
		// �ړ�
		// �O
		if (GetKeyboardPress(ControllerNum == 0 ? DIK_RIGHT : DIK_A) 
			|| IsButtonPressed(ControllerNum, BUTTON_RIGHT) || IsButtonPressed(ControllerNum, STICK_RIGHT))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Frontwalk, ANIM_SPD_2);
		}
		// ���
		else if (GetKeyboardPress(ControllerNum == 0 ? DIK_LEFT : DIK_D)
			|| IsButtonPressed(ControllerNum, BUTTON_LEFT) || IsButtonPressed(ControllerNum, STICK_LEFT))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Backwalk, ANIM_SPD_2);
		}
		// ��O
		else if (GetKeyboardPress(ControllerNum == 0 ? DIK_DOWN : DIK_S)
			|| IsButtonPressed(ControllerNum, BUTTON_DOWN) || IsButtonPressed(ControllerNum, STICK_DOWN))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Rightstep, ANIM_SPD_2);
		}
		// ��
		else if (GetKeyboardPress(ControllerNum == 0 ? DIK_UP : DIK_W)
			|| IsButtonPressed(ControllerNum, BUTTON_UP) || IsButtonPressed(ControllerNum, STICK_UP))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Leftstep, ANIM_SPD_2);
		}

		// �U��
		// �p���`
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I)
			|| IsButtonTriggered(ControllerNum, BUTTON_A))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Punchi, ANIM_SPD_15);
		}
		// �L�b�N
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K)
			|| IsButtonTriggered(ControllerNum, BUTTON_B))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Kick, ANIM_SPD_15);
		}
		// �g����
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L) 
			|| IsButtonTriggered(ControllerNum, BUTTON_X))
		{
			// ���˂���Ă��Ȃ��ꍇ
			if (Chara->HadouBullet.use == false)
			{
				Chara->Animation->ChangeAnimation(Chara->Animation, Hadou, ANIM_SPD_2);
			}
		}
		// ������
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O)
			|| IsButtonTriggered(ControllerNum, BUTTON_C))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Shoryu, ANIM_SPD_15);
		}
		// �K�[�h
		else if (GetKeyboardPress(ControllerNum == 0 ? DIK_G : DIK_P) 
			|| IsButtonPressed(ControllerNum, BUTTON_Y))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Guard, ANIM_SPD_1);
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
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, ANIM_SPD_1);
		}

		// �U��
		// �p���`
		if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I)
			|| IsButtonTriggered(ControllerNum, BUTTON_A))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Punchi, ANIM_SPD_15);
		}
		// �L�b�N
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K)
			|| IsButtonTriggered(ControllerNum, BUTTON_B))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Kick, ANIM_SPD_15);
		}
		// �g����
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L) 
			|| IsButtonTriggered(ControllerNum, BUTTON_X))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Hadou, ANIM_SPD_2);
		}
		// ������
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O)
			|| IsButtonTriggered(ControllerNum, BUTTON_C))
		{
			// ���˂���Ă��Ȃ��ꍇ�̂�
			if (Chara->HadouBullet.use == false)
			{
				Chara->Animation->ChangeAnimation(Chara->Animation, Shoryu, ANIM_SPD_15);
			}
		}
		// �K�[�h
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_G : DIK_P) 
			|| IsButtonTriggered(ControllerNum, BUTTON_Y))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Guard, ANIM_SPD_1);
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
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, ANIM_SPD_1);
		}

		// �U��
		// �p���`
		if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I)
			|| IsButtonTriggered(ControllerNum, BUTTON_A))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Punchi, ANIM_SPD_15);
		}
		// �L�b�N
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K) 
			|| IsButtonTriggered(ControllerNum, BUTTON_B))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Kick, ANIM_SPD_15);
		}
		// �g����
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L) 
			|| IsButtonTriggered(ControllerNum, BUTTON_X))
		{
			// ���˂���Ă��Ȃ��ꍇ�̂�
			if (Chara->HadouBullet.use == false)
			{
				Chara->Animation->ChangeAnimation(Chara->Animation, Hadou, ANIM_SPD_2);
			}
		}
		// ������
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O) 
			|| IsButtonTriggered(ControllerNum, BUTTON_C))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Shoryu, ANIM_SPD_15);
		}
		// �K�[�h
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_G : DIK_P)
			|| IsButtonTriggered(ControllerNum, BUTTON_Y))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Guard, ANIM_SPD_1);
		}
		break;
	case Rightstep:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, ANIM_SPD_1);
		}
		break;
	case Leftstep:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, ANIM_SPD_1);
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
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, ANIM_SPD_1);
		}

		// �U��
		// �p���`
		if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I)
			|| IsButtonTriggered(ControllerNum, BUTTON_A))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Punchi, ANIM_SPD_15);
		}
		// �L�b�N
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K) 
			|| IsButtonTriggered(ControllerNum, BUTTON_B))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Kick, ANIM_SPD_15);
		}
		// �g����
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L)
			|| IsButtonTriggered(ControllerNum, BUTTON_X))
		{
			if (Chara->HadouBullet.use == false)
			{
				Chara->Animation->ChangeAnimation(Chara->Animation, Hadou, ANIM_SPD_2);
			}
		}
		// ������
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O)
			|| IsButtonTriggered(ControllerNum, BUTTON_C))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Shoryu, ANIM_SPD_15);
		}
	case Damage:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, ANIM_SPD_1);
			Chara->HadouBullet.frame = 0;
		}
		break;
	case Down:
		// �A�j���[�V�����I���ŋN���オ��Ɉڍs
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Downpose, ANIM_SPD_1);
			// �v���C���[���W�����[�V�������W�ɍ��킹��
			D3DXMATRIXA16 newmatrix;	// ���[�V�������W���擾���邽�߂̍s��
			newmatrix = GetBoneMatrix(Chara->Animation, "Hips");
			Chara->pos = D3DXVECTOR3(newmatrix._41, 0.0f, newmatrix._43);
		}
		break;
	case Downpose:
		// �A�j���[�V�����I���ŋN���オ��Ɉڍs
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Getup, ANIM_SPD_15);
		}
	case Getup:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, ANIM_SPD_1);
			Chara->HadouBullet.frame = 0;
		}
		break;
	case Punchi:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, ANIM_SPD_1);
			Chara->HitFrag = false;
		}
		break;
	case Kick:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, ANIM_SPD_1);
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
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, ANIM_SPD_1);
			Chara->HadouBullet.frame = 0;
			Chara->HitFrag = false;
		}
		break;
	case Shoryu:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, ANIM_SPD_1);
			Chara->HitFrag = false;
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
