//=============================================================================
//
// �o�g�������i�v���C���[���G�l�~�[���Ăяo�������j [battle.cpp]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#include "main.h"
#include "battle.h"

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
			if (HitBC(AttackChara->HitBall[LeftHand].pos, DefendChara->HitBall[i].pos, AttackChara->HitBall[LeftHand].scl.x, DefendChara->HitBall[i].scl.x) == true)
			{
				return true;
			}
		}
		break;
	case Kick:
		// �E���Ƒ���̊e���ʂƂ̔���
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			if (HitBC(AttackChara->HitBall[RightFoot].pos, DefendChara->HitBall[i].pos, AttackChara->HitBall[RightFoot].scl.x, DefendChara->HitBall[i].scl.x) == true)
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
			if (HitBC(AttackChara->HitBall[LeftFoot].pos, DefendChara->HitBall[i].pos, AttackChara->HitBall[LeftFoot].scl.x, DefendChara->HitBall[i].scl.x) == true)
			{
				return true;
			}
			// �E���Ƃ̔���
			else if (HitBC(AttackChara->HitBall[RightFoot].pos, DefendChara->HitBall[i].pos, AttackChara->HitBall[RightFoot].scl.x, DefendChara->HitBall[i].scl.x) == true)
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
		}
		//�@�q�b�g�t���O�̕ύX
		AttackChara->HitFrag = true;
		break;
	case Shoryu:
		// �G�̏�Ԋm�F
		if (DefendChara->Animation->CurrentAnimID == Guard || DefendChara->Animation->CurrentAnimID == Down
			|| DefendChara->Animation->CurrentAnimID == Downpose || DefendChara->Animation->CurrentAnimID == Getup)
		{
			// �G�t�F�N�g
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
			//�@�g�p�t���O�̕ύX
			AttackChara->HadouBullet.use = false;
			DefendChara->HadouBullet.use = false;
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

			}
			else
			{
				// �G�̃��[�V�����ύX
				DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Damage, ANIM_SPD_1);
				// �_���[�W
				SubDamage(DefendChara, DAMAGE_HADOU);

				// �G�t�F�N�g
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