//=============================================================================
//
// �����蔻�菈�� [HitCheck.cpp]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#include "main.h"
#include "HitCheck.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=====================================================================================================
// �����蔻��
// centerPosA = ���S�ʒu, centerPosB = ���S�ʒu, r1 = ���a, r2 = ���a
//=====================================================================================================
bool hitBC(D3DXVECTOR3 centerPosA, D3DXVECTOR3 centerPosB, float r1, float r2)
{
	//�����̌v�Z
	D3DXVECTOR3 temp = centerPosA - centerPosB;
	float fLengthSq = D3DXVec3LengthSq(&temp);

	if (fLengthSq <= ((r1 + r2) * (r1 + r2)))		//���������Ƃ�
	{
		return true;
	}

	return false;
}

//=====================================================================================================
// �����蔻��
// pos1 = �U�����U������, pos2 = �h�䑤�̔���, size1 = �U�����U���͈�, size2 = �h�䑤�͈̔�
//=====================================================================================================
bool hitBB(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 size1, D3DXVECTOR3 size2)
{
	if ((pos2.x + size2.x > pos1.x - size1.x) && (pos1.x + size1.x > pos2.x - size2.x) &&
		(pos2.y + size2.y > pos1.y - size1.y) && (pos1.y + size1.y > pos2.y - size2.y) &&
		(pos2.z + size2.z > pos1.y - size1.z) && (pos1.z + size1.z > pos2.z - size2.z))
	{
		return true;
	}

	return false;
}

//=============================================================================
// ����
//=============================================================================
float dotProduct(D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
{
#if 1
	float ans = vl->x * vr->x + vl->y * vr->y + vl->z * vr->z;
#else
	float ans = D3DXVec3Dot(vl, vr);
#endif	
	return ans;
}


//=============================================================================
// �O��
//=============================================================================
void crossProduct(D3DXVECTOR3 *ret, D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
{
#if 1
	ret->x = (vl->y * vr->z) - (vl->z * vr->y);
	ret->y = (vl->z * vr->x) - (vl->x * vr->z);
	ret->z = (vl->x * vr->y) - (vl->y * vr->x);
#else
	D3DXVec3Cross(ret, vl, vr);
#endif
}


//=============================================================================
// �|���S���Ɛ����Ƃ̓����蔻��
// p0  :�|���S���̒��_1
// p1  :�|���S���̒��_2
// p2  :�|���S���̒��_3
// pos0:�n�_�i�ړ��O�j
// pos1:�I�_�i�ړ���j
//=============================================================================
int hitCheck(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	D3DXVECTOR3		nor;		// �|���S���̖@��

	{	// �|���S���̊O�ς��Ƃ��Ė@�������߂�B�����Đ��K�����Ă����B
		D3DXVECTOR3		vec01 = p1 - p0;
		D3DXVECTOR3		vec02 = p2 - p0;
		crossProduct(&nor, &vec01, &vec02);
	}

	{	// ���ʂƐ����̓��ςƂ��ďՓ˂��Ă���\���𒲂ׂ�
		// ���߂��@���ƃx�N�g���Q�i�����̗��[�ƃ|���S����̔C�ӂ̓_�j�̓��ςƂ��ďՓ˂��Ă���\���𒲂ׂ�
		D3DXVECTOR3		vec1 = pos0 - p0;
		D3DXVECTOR3		vec2 = pos1 - p0;
		float a = dotProduct(&vec1, &nor);
		float b = dotProduct(&vec2, &nor);

		if ((a * b) > 0)
		{
			// �������Ă���\���͖���
			return(0);
		}
	}


	{	// �|���S���Ɛ����̌�_�����߂�
		D3DXVECTOR3		vec1 = pos0 - p0;
		D3DXVECTOR3		vec2 = pos1 - p0;
		float			d1 = (float)fabs(dotProduct(&nor, &vec1));			// ����������߂�ׂ̓_�ƃ|���S���Ƃ̋���
		float			d2 = (float)fabs(dotProduct(&nor, &vec2));			// ����������߂�ׂ̓_�ƃ|���S���Ƃ̋���
		float			a = d1 / (d1 + d2);									// ������
		D3DXVECTOR3		vec3 = (1 - a) * vec1 + a * vec2;
		D3DXVECTOR3		p3 = p0 + vec3;										// ��_

		{	// ���߂���_���|���S���̒��ɂ��邩���ׂ�

			// �|���S���̊e�ӂ̃x�N�g��
			D3DXVECTOR3		v1 = p1 - p0;
			D3DXVECTOR3		v2 = p2 - p1;
			D3DXVECTOR3		v3 = p3 - p2;

			// �e���_�ƌ�_�Ƃ̃x�N�g��
			D3DXVECTOR3		v4 = p3 - p1;
			D3DXVECTOR3		v5 = p3 - p2;
			D3DXVECTOR3		v6 = p3 - p0;

			// �e�ӂƂ̊O�ςŖ@�������߂�
			D3DXVECTOR3		n1, n2, n3;
			crossProduct(&n1, &v1, &v4);
			crossProduct(&n2, &v2, &v5);
			crossProduct(&n3, &v3, &v6);

			// ���ꂼ��̃x�N�g���̌�������ςŃ`�F�b�N
			if (dotProduct(&n1, &nor) < 0) return(0);
			if (dotProduct(&n2, &nor) < 0) return(0);
			if (dotProduct(&n3, &nor) < 0) return(0);

		}
	}

	return(1);
}
