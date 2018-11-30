//=============================================================================
//
// X�t�@�C���ǂݍ��ݏ��� [particle.cpp]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#include "model.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �g�[�N��
static char* Pointer;						// �t�@�C���̓ǂݍ��݈ʒu
static char Token[1024];					// �ǂݍ��񂾃g�[�N��

// �e�N�X�`��
static vector<IDirect3DTexture9*> Texture;	// �ǂݍ��񂾃e�N�X�`���̈ꗗ
static vector<char*> TextureName;			// �ǂݍ��񂾃e�N�X�`���t�@�C�����̈ꗗ

//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
IDirect3DTexture9* GetTexture(IDirect3DDevice9* device, const char* file_name)
{
	// �w�肳�ꂽ�e�N�X�`�����ǂݍ��ݍς݂̏ꍇ�ɂ́A�d�����ēǂݍ��܂��ɁA�ȑO�ɓǂݍ��񂾃e�N�X�`����Ԃ�
	for (size_t i = 0; i < TextureName.size(); i++)
	{
		if (strcmp(TextureName[i], file_name) == 0)
		{
			return Texture[i];
		}
	}
	IDirect3DTexture9* texture = NULL;

	// �e�N�X�`����ǂݍ���
	D3DXCreateTextureFromFileEx(
		device,							// �C���^�[�t�F�C�X�ւ̃|�C���^
		file_name,						// �t�@�C�������w�肷�镶����ւ̃|�C���^
		D3DX_DEFAULT,					// ���B0�܂���D3DX_DEFAULT�̏ꍇ�t�@�C������擾
		D3DX_DEFAULT,					// �����B0�܂���D3DX_DEFAULT�̏ꍇ�t�@�C������擾
		D3DX_DEFAULT,					// �v�������~�b�v���x���̐��B0�܂���D3DX_DEFAULT�̏ꍇ���S�ȃ~�b�v�}�b�v�`�F�[�����쐬
		0,								// 0�AD3DUSAGE_RENDERTARGET�A�܂��� D3DUSAGE_DYNAMIC�B���̃t���O�� D3DUSAGE_RENDERTARGET �ɐݒ肷��ƁA���̃T�[�t�F�C�X�̓����_�����O �^�[�Q�b�g�Ƃ��Ďg���邱�Ƃ�����
		D3DFMT_UNKNOWN,					// D3DFORMAT �񋓌^�̃����o�B�e�N�X�`���ɑ΂��ėv�����ꂽ�s�N�Z�� �t�H�[�}�b�g���L�q����B�Ԃ����e�N�X�`���̃t�H�[�}�b�g�́AFormat �Ŏw�肵���t�H�[�}�b�g�ƈقȂ�ꍇ������BD3DFMT_UNKNOWN �̏ꍇ�A�t�H�[�}�b�g�̓t�@�C������擾�����B
		D3DPOOL_MANAGED,				// D3DPOOL �񋓌^�̃����o�B�e�N�X�`���̔z�u��ƂȂ郁���� �N���X���L�q����B
		D3DX_DEFAULT,					// �C���[�W���t�B���^�����O������@�𐧌䂷�� 1 ���邢�͕����� D3DX_FILTER �̑g�ݍ��킹�B���̃p�����[�^�� D3DX_DEFAULT ���w�肷�邱�Ƃ́AD3DX_FILTER_TRIANGLE | D3DX_FILTER_DITHER ���w�肷�邱�ƂƓ������B
		D3DX_DEFAULT,					// �C���[�W���t�B���^�����O������@�𐧌䂷�� 1 ���邢�͕����� D3DX_FILTER �̑g�ݍ��킹�B���̃p�����[�^�� D3DX_DEFAULT ���w�肷�邱�Ƃ́AD3DX_FILTER_BOX ���w�肷�邱�ƂƓ������B
		0,								// �����ƂȂ� D3DCOLOR �̒l�B�J���[�L�[�𖳌��ɂ���ꍇ�� 0 ���w�肷��B
		NULL,							// �\�[�X �C���[�W �t�@�C�����̃f�[�^�̋L�q���i�[���� D3DXIMAGE_INFO �\���̂ւ̃|�C���^
		NULL,							// �i�[���� 256 �F�p���b�g��\�� PALETTEENTRY �\���̂ւ̃|�C���^
		&texture);						// �쐬���ꂽ�L���[�u �e�N�X�`�� �I�u�W�F�N�g��\���AIDirect3DTexture9 �C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X

	// �e�N�X�`����z��Ɋi�[
	Texture.push_back(texture);

	// �e�N�X�`���t�@�C������z��Ɋi�[
	char* t = new char[strlen(file_name) + 1];
	strcpy(t, file_name);
	TextureName.push_back(t);

	// �ǂݍ��񂾃e�N�X�`����Ԃ�
	return texture;
}
//=============================================================================
// �G���[�\��
//=============================================================================
static void Error(const char* message)
{
	// �_�C�A���O�{�b�N�X�̕\��
	MessageBox(NULL, message, "���f���̓ǂݍ��݃G���[", MB_OK);
	PostQuitMessage(0);
}

//=============================================================================
// �g�[�N���̎擾
//=============================================================================
static void GetToken(void)
{
	// p=�ǂݍ��݈ʒu�Aq=�ǂݍ��񂾃g�[�N��
	char* p = Pointer;
	char* q = Token;

	// ��؂蕶����ǂݔ�΂�
	while (*p != '\0' && strchr(" \t\r\n,;\"", *p))
	{
		p++;
	}

	// {}�͒P�̂Ńg�[�N���Ƃ���
	if (*p == '{' || *p == '}')
	{
		*q++ = *p++;
	}

	// {}�ȊO��ǂݍ��񂾏ꍇ�A�Ăы�؂蕶���������܂ł̕������g�[�N���Ƃ���
	else {
		while (*p != '\0' && !strchr(" \t\r\n,;\"{}", *p))
		{
			*q++ = *p++;
		}
	}

	// �ǂݍ��݈ʒu�̍X�V
	Pointer = p;
	// �g�[�N���̖�����NULL�̒ǉ�
	*q = '\0';
}

//=============================================================================
// �w�肵���g�[�N���̎擾
//=============================================================================
static void GetToken(const char* token)
{
	// �g�[�N���̓ǂݍ���
	GetToken();

	// �ǂݍ��񂾃g�[�N��������������������ƈقȂ�ꍇ�A�G���[��\�����ďI��
	if (strcmp(Token, token) != 0)
	{
		char s[1024];
		sprintf_s(s, sizeof(s), "�z��g�[�N���F%s\n�A�ǂݍ��݃g�[�N���F%s", token, Token);
		Error(s);
	}
}

//=============================================================================
// ���l�f�[�^�̎擾�ifloat�^�j
//=============================================================================
static float GetFloatToken(void)
{
	// �g�[�N���̓ǂݍ���
	GetToken();
	// float�^�ɂ��ĕԂ�
	return atof(Token);
}

//=============================================================================
// ���l�f�[�^�̎擾�iInt�^�j
//=============================================================================
static int GetIntToken(void)
{
	// �g�[�N���̓ǂݍ���
	GetToken();
	// int�^�ɂ��ĕԂ�
	return atoi(Token);
}

//=============================================================================
// �m�[�h�̃X�L�b�v����
//=============================================================================
static void SkipNode(void)
{
	// {���o������܂œǂݔ�΂�
	while (*Pointer != '\0') {
		GetToken();
		if (strchr(Token, '{'))
		{
			break;
		}
	}
	// {�̐���\���J�E���^
	int count = 1;

	// �t�@�C���̖����ɒB���邩�A}���ǂݍ��܂��܂Łi�Ή�����}���ǂݍ��܂��ƃJ�E���^��0�ɖ߂�j
	while (*Pointer != '\0' && count > 0) {

		// �g�[�N���̎擾
		GetToken();

		// {��ǂݍ��񂾂�J�E���^��+1
		if (strchr(Token, '{'))
		{
			count++;
		}
		// }��ǂݍ��񂾂�J�E���^��-1
		else if (strchr(Token, '}'))
		{
			count--;
		}
	}

	// �J�E���^��1�ȏ�ŏI�������犇�ʂ̑Ή��֌W�����������̂ŃG���[�\�����ďI��
	if (count > 0) {
		Error("���ʂ̔�Ή�");
		return;
	}
}

//=============================================================================
// ���f���ǂݍ���
//=============================================================================
CModel::CModel(IDirect3DDevice9* device, const char* file_name, bool anim_only)
	//: Device(device),
	//OriginalVertexBuffer(NULL), AnimatedVertexBuffer(NULL), VertexCount(0),
	//IndexBuffer(NULL), FaceCount(0), SmootherWeight(0)
{
	// �t�@�C�����J��
	HANDLE file = CreateFile(
		file_name,					// �t�@�C���̖��O
		GENERIC_READ,				// �A�N�Z�X���[�h
		FILE_SHARE_READ,			// ���L���[�h
		NULL,						// �Z�L�����e�B�L�q�q
		OPEN_EXISTING,				// �쐬���@
		FILE_ATTRIBUTE_READONLY,	// �t�@�C������
		NULL);						// �e���v���[�g�t�@�C���̃n���h��

	// �t�@�C�����J���Ȃ��������̓��b�Z�[�W��\�����ďI��
	if (file == INVALID_HANDLE_VALUE)
	{
		char s[1024];
		sprintf_s(s, sizeof(s), "�t�@�C���̓ǂݍ��݂Ɏ��s", file_name);
		Error(s);
		return;
	}

	// �t�@�C���̃T�C�Y���擾���A�ǂݍ��ݗp�̃o�b�t�@���m�ۂ���
	DWORD size = GetFileSize(file, NULL);
	char* buf = new char[size + 1];
	buf[size] = '\0';

	// �t�@�C�����o�b�t�@�ɓǂݍ���
	DWORD read_size;
	ReadFile(file, buf, size, &read_size, NULL);

	// �t�@�C������āA�ǂݍ��񂾃T�C�Y���`�F�b�N����
	CloseHandle(file);
	if (read_size != size) return;

	// �e���v���[�g��ǂݔ�΂�
	// �t�@�C���̐擪����ǂݍ���
	Pointer = buf;

	// �t�@�C���̖����ɒB����܂ŌJ��Ԃ�
	while (*Pointer != '\0')
	{
		// �g�[�N���̎擾
		GetToken();

		// �e���v���[�g
		if (strcmp(Token, "template") == 0) {
			// �v�f��ǂݔ�΂�
			SkipNode();
		}
		// �ǂݍ��񂾃g�[�N�����uFrame�v�̏ꍇ�A�t���[���v�f���Ɣ��f����
		else if (strcmp(Token, "Frame") == 0)
		{
			// X�t�@�C���ɕ����̃��[�g�t���[��������ꍇ�ɂ̓G���[��\�����ďI������
			if (!Frame.empty())
			{
				Error("�t�@�C�����ɕ����̃��[�g�t���[��");
				return;
			}

			// �t���[���̊K�w�\����ǂݍ���
			new CModelFrame(&Frame);
		}

		// �A�j���[�V�����Z�b�g�̓ǂݍ���
		// �ǂݍ��񂾃g�[�N�����uAnimationSet�v�̂Ƃ��A�A�j���[�V�����v�f���Ɣ��f
		else if (strcmp(Token, "AnimationSet") == 0)
		{
			// �i�[����I�u�W�F�N�g�̍쐬
			CModelAnimation* anim = new CModelAnimation();

			// �A�j���[�V�����̔z��ɃI�u�W�F�N�g���i�[
			Animation.push_back(anim);

			// ���ԂƃE�F�C�g�̏�����
			anim->Time = 0;
			anim->Weight = 0;

			// �A�j���[�V�������̎擾
			GetToken();
			anim->Name = new char[strlen(Token) + 1];
			strcpy(anim->Name, Token);

			// ���̃g�[�N�����u�o�v���Ƃ������Ƃ��m�F
			GetToken("{");

			// �����܂œǂݍ���
			while (*Pointer != '\0')
			{
				// �g�[�N���̎擾
				GetToken();

				// �u�p�v����������I��
				if (strchr(Token, '}'))
				{
					break;
				}

				// �uAnimation�v���ǂݍ��񂾂�A�j���[�V�����v�f���Ɣ��f
				if (strcmp(Token, "Animation") == 0)
				{
					// �L�[�̃I�u�W�F�N�g���쐬���A�z��Ɋi�[
					CModelAnimationKey* key = new CModelAnimationKey();
					anim->Key.push_back(key);

					// ���O��ǂݔ�΂�
					GetToken();

					// ���̃g�[�N����2�́u�o�v���Ƃ������Ƃ��m�F
					GetToken("{");
					GetToken("{");

					// �t���[�������擾����
					GetToken();
					key->FrameName = new char[strlen(Token) + 1];
					strcpy(key->FrameName, Token);

					// �t���[�����ɑΉ�����t���[���̃I�u�W�F�N�g���������A�t���[���ԍ����擾����
					key->FrameIndex = FindFrame(key->FrameName)->Index;

					// ���̃g�[�N�����u�p�v���Ƃ������Ƃ��m�F
					GetToken("}");

					// ���̃g�[�N�����uAnimationKey�v�u�o�v�u4�v���Ƃ������Ƃ��m�F
					GetToken("AnimationKey");
					GetToken("{");
					GetToken("4");

					// �L�[�̐����擾����
					DWORD key_count = GetIntToken();

					// �S�ẴL�[���擾����
					for (DWORD i = 0; i < key_count; i++)
					{
						// ���Ԃ̎擾
						key->Time.push_back((float)GetIntToken());

						// ���̃g�[�N����16���Ƃ������Ƃ��m�F
						GetToken("16");

						// �s��̎擾
						float f[16];
						for (DWORD j = 0; j < 16; j++)
						{
							f[j] = GetFloatToken();
						}
						key->Matrix.push_back(new D3DXMATRIXA16(f));
					}

					// ���̃g�[�N�����Q�́u�p�v���Ƃ������Ƃ��m�F
					GetToken("}");
					GetToken("}");
				}
			}
		}
	}

	// �t�@�C���̐擪����ǂݍ���
	Pointer = buf;

	// �A�j���[�V�����f�[�^�ȊO���ǂݍ��ޏꍇ
	if (!anim_only)
	{
		// �t�@�C���̖����ɒB����܂�
		while (*Pointer != '\0')
		{
			//�g�[�N���̎擾
			GetToken();

			// �e���v���[�g�̃X�L�b�v
			if (strcmp(Token, "template") == 0)
			{
				SkipNode();
			}
			// ���b�V���̓ǂݍ���
			else if (strcmp(Token, "Mesh") == 0)
			{
				// ���b�V����񂪕�������t�@�C���ɂ͑Ή����Ă��Ȃ�
				// �����X�L�����f���ɂ̂ݑΉ�
				if (OriginalVertexBuffer)
				{
					Error("�t�@�C�����ɕ����̃��b�V��");
					return;
				}

				GetToken();
				GetToken("{");

				// ���_���W�̓ǂݍ���
				// ���_�����擾
				VertexCount = GetIntToken();

				// �I���W�i�����_�o�b�t�@�̍쐬
				device->CreateVertexBuffer(
					sizeof(ORIGINAL_VERTEX)*VertexCount, 0,
					ORIGINAL_FVF, D3DPOOL_SYSTEMMEM,
					&OriginalVertexBuffer, NULL);

				// �A�j���[�V�������_�o�b�t�@�̍쐬
				device->CreateVertexBuffer(
					sizeof(ANIMATED_VERTEX)*VertexCount, 0,
					ANIMATED_FVF, D3DPOOL_MANAGED,
					&AnimatedVertexBuffer, NULL);

				// ���_�o�b�t�@���������쐬�ł��Ȃ���΃G���[��\������
				if (!OriginalVertexBuffer || !AnimatedVertexBuffer)
				{
					Error("���_�o�b�t�@�̍쐬�Ɏ��s");
					return;
				}

				// ���_�o�b�t�@�ɏ������ނ��߃��b�N����
				void* vb;
				OriginalVertexBuffer->Lock(0, 0, &vb, 0);
				ORIGINAL_VERTEX* v = (ORIGINAL_VERTEX*)vb;

				// ���_���W����������
				for (DWORD i = 0; i < VertexCount; i++)
				{
					// ���_���W���擾���ď�������
					v[i].Pos.x = GetFloatToken();
					v[i].Pos.y = GetFloatToken();
					v[i].Pos.z = GetFloatToken();
					// �{�[���ԍ��ƃ{�[���E�F�C�g������������
					for (DWORD j = 0; j < MODEL_BLEND_COUNT; j++)
					{
						v[i].Index[j] = 0;
					}
					v[i].Weight[0] = 1;
					for (DWORD j = 1; j < MODEL_BLEND_COUNT; j++)
					{
						v[i].Weight[j] = 0;
					}
				}

				// ���_�o�b�t�@�̃A�����b�N
				OriginalVertexBuffer->Unlock();

				// �ʂ̓ǂݍ���
				// �ʐ����擾
				FaceCount = GetIntToken();

				// �C���f�b�N�X�o�b�t�@�̍쐬
				device->CreateIndexBuffer(
					sizeof(WORD) * 3 * FaceCount, 0,
					D3DFMT_INDEX16, D3DPOOL_MANAGED,
					&IndexBuffer, NULL);

				// �C���f�b�N�X�o�b�t�@���������쐬�ł��Ȃ���΃G���[��\��
				if (!IndexBuffer)
				{
					Error("�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s");
					return;
				}

				// �C���f�b�N�X�o�b�t�@�ɏ������ނ��߃��b�N����
				void* ib;
				IndexBuffer->Lock(0, 0, &ib, 0);
				WORD* index = (WORD*)ib;

				// ���_�ԍ�����������
				for (DWORD i = 0, n = FaceCount * 3; i < n; i += 3)
				{
					GetToken("3");
					for (DWORD j = 0; j < 3; j++)
					{
						index[i + j] = GetIntToken();
					}
				}

				// �C���f�b�N�X�o�b�t�@�̃A�����b�N
				IndexBuffer->Unlock();
			}
			// ���b�V���@���̓ǂݍ���
			// ���b�V���@���v�f���Ɣ��f����
			else if (strcmp(Token, "MeshNormals") == 0)
			{
				// ���̃g�[�N�����o���Ƃ������Ƃ��m�F����
				GetToken("{");

				// ���_�����擾���A���b�V���v�f�Ŏw�肳�ꂽ���_���ƈقȂ�ꍇ�ɂ̓G���[��\������
				if (GetIntToken() != VertexCount)
				{
					Error("���_���Ɩ@�������s��v");
					return;
				}

				// ���_�o�b�t�@�ɏ������ނ��߃��b�N����
				void* vb;
				OriginalVertexBuffer->Lock(0, 0, &vb, 0);
				ORIGINAL_VERTEX* v = (ORIGINAL_VERTEX*)vb;

				// �@���f�[�^����������
				for (DWORD i = 0; i < VertexCount; i++)
				{
					v[i].Normal.x = GetFloatToken();
					v[i].Normal.y = GetFloatToken();
					v[i].Normal.z = GetFloatToken();
				}

				// ���_�o�b�t�@�̃A�����b�N
				OriginalVertexBuffer->Unlock();

				// �ʐ����擾���A���b�V���v�f�Ŏw�肳�ꂽ�ʐ��ƈقȂ�ꍇ�ɂ̓G���[��\������
				if (GetIntToken() != FaceCount)
				{
					Error("�ʐ��Ɩ@���C���f�b�N�X�����s��v");
					return;
				}

				// �@���ԍ��͓ǂݔ�΂�
				for (DWORD i = 0, n = FaceCount * 4; i < n; i++)
				{
					GetToken();
				}
			}
			// �ǂݍ��񂾃g�[�N�����uMeshTextureCoords�v�̂Ƃ��ɂ́A���b�V���e�N�X�`�����W�v�f���Ɣ��f����

			else if (strcmp(Token, "MeshTextureCoords") == 0)
			{
				// ���̃g�[�N��g�t�@�o�ł��邱�Ƃ̊m�F
				GetToken("{");

				// �擾�������_���ƃ��b�V���v�f�Ŏw�肳�ꂽ���_�����قȂ�ꍇ�ɂ̓G���[�\��
				if (GetIntToken() != VertexCount)
				{
					Error("���_���ƃe�N�X�`�����W�����s��v");
					return;
				}

				// ���_�o�b�t�@�ɏ������ނ��߃I���W�i�����_�o�b�t�@�����b�N
				void* ovb;
				OriginalVertexBuffer->Lock(0, 0, &ovb, 0);
				ORIGINAL_VERTEX* ov = (ORIGINAL_VERTEX*)ovb;

				// ���_�o�b�t�@�ɏ������ނ��߃A�j���[�V�������_�o�b�t�@�����b�N
				void* avb;
				AnimatedVertexBuffer->Lock(0, 0, &avb, 0);
				ANIMATED_VERTEX* av = (ANIMATED_VERTEX*)avb;

				// �e�N�X�`�����W����������
				for (DWORD i = 0; i < VertexCount; i++)
				{
					ov[i].UV.x = av[i].UV.x = GetFloatToken();
					ov[i].UV.y = av[i].UV.y = GetFloatToken();
				}

				// ���_�o�b�t�@�̃A�����b�N
				OriginalVertexBuffer->Unlock();
				AnimatedVertexBuffer->Unlock();
			}
			// ���b�V���}�e���A�����X�g�̓ǂݍ���
			// �ǂݍ��񂾃g�[�N�����uMeshMaterialList�v�̂Ƃ��ɂ̓��b�V���}�e���A�����X�g�v�f���Ɣ��f����
			else if (strcmp(Token, "MeshMaterialList") == 0)
			{
				// ���̃g�[�N�����o���Ƃ������Ƃ��m�F����
				GetToken("{");

				// �}�e���A�����̎擾
				DWORD material_count = GetIntToken();

				// �擾�����ʐ��ƃ��b�V���v�f�Ŏw�肳�ꂽ�ʐ�������Ă���΃G���[��\��
				if (GetIntToken() != FaceCount)
				{
					Error("�ʐ��ƃ}�e���A�����X�g���̕s��v");
					return;
				}

				// �T�u�Z�b�g�̍쐬
				CModelSubset* subset = NULL;
				DWORD prev_index = -1;
				for (DWORD i = 0; i < FaceCount; i++)
				{
					// �}�e���A���ԍ��̎擾
					DWORD index = GetIntToken();

					// �擾�����}�e���A���ԍ������O�Ɏ擾�����}�e���A���ԍ��ƈႤ���ɂ͐V�����T�u�Z�b�g���쐬
					if (index != prev_index)
					{
						// �T�u�Z�b�g�̍쐬
						subset = new CModelSubset();
						// �ʐ��E�J�n�ʔԍ��E�}�e���A���ԍ��̐ݒ�
						subset->FaceCount = 1;
						subset->FaceIndex = i;
						subset->MaterialIndex = index;

						// �T�u�Z�b�g��z��Ɋi�[
						Subset.push_back(subset);

						// ���Ɏ擾����}�e���A���ԍ��Ɣ�r���邽�߁A�擾�����}�e���A���ԍ���ۑ�����
						prev_index = index;
					}
					else
					{
						// �擾�����}�e���A���ԍ����A���O�Ɏ擾�����}�e���A���ԍ��ƈ�v����Ƃ��ɂ́A�O��쐬�����T�u�Z�b�g�̖ʐ��𑝂₷
						subset->FaceCount++;
					}
				}

				// �}�e���A���̓ǂݍ���
				// ���b�V���}�e���A�����X�g�v�f�ɏ�����Ă������ɉ����āA�}�e���A���v�f��ǂݍ���
				for (DWORD i = 0; i < material_count; i++)
				{
					// �}�e���A���̃I�u�W�F�N�g���쐬
					CModelMaterial* material = new CModelMaterial();

					// �}�e���A����z��ɒǉ�
					Material.push_back(material);

					// ���̃g�[�N�����uMaterial�v�Ɓu�o�v���Ƃ������Ƃ��m�F
					GetToken("Material");
					GetToken("{");

					// ��{�F���擾���A�I�u�W�F�N�g�Ɋi�[
					material->Color = D3DXVECTOR4(
						GetFloatToken(), GetFloatToken(),
						GetFloatToken(), GetFloatToken());

					// �n�C���C�g���x�E�n�C���C�g�F�E���Ȕ����F�͓ǂݔ�΂�
					for (int j = 0; j < 7; j++)
					{
						GetToken();
					}

					// ���̃g�[�N�����uTextureFilename�v�Ɓu�o�v���Ƃ������Ƃ��m�F
					GetToken("TextureFilename");
					GetToken("{");

					// �e�N�X�`���t�@�C�������擾���A�e�N�X�`�����쐬���ăI�u�W�F�N�g�Ɋi�[
					GetToken();
					material->Texture = GetTexture(Device, Token);

					// ���̃g�[�N�����Q�́u�p�v���Ƃ������Ƃ��m�F
					GetToken("}");
					GetToken("}");
				}
			}
			// �ǂݍ��񂾃g�[�N�����uSkinWeights�v�������ꍇ�A�X�L���E�F�C�g�v�f���Ɣ��f����
			else if (strcmp(Token, "SkinWeights") == 0)
			{
				// �g�[�N�����o�ł��邱�Ƃ��m�F
				GetToken("{");

				// �t���[�����擾���A�Ή�����I�u�W�F�N�g����������
				GetToken();
				CModelFrame* frame = FindFrame(Token);

				// �E�F�C�g���̎擾
				DWORD count = GetIntToken();

				// ���_�ԍ��ƃE�F�C�g���ꎞ�I�Ɋi�[���邽�߂̃o�b�t�@���m��
				DWORD* vertex = new DWORD[count];
				float* weight = new float[count];

				// �I�t�Z�b�g�s��̐������i�[���邽�߂̔z��
				float matrix[16];

				// ���_�ԍ��̎擾
				for (DWORD i = 0; i < count; i++)
				{
					vertex[i] = GetIntToken();
				}

				// �E�F�C�g�̎擾
				for (DWORD i = 0; i < count; i++)
				{
					weight[i] = GetFloatToken();
				}

				// �I�t�Z�b�g�s��̎擾
				for (DWORD i = 0; i < 16; i++)
				{
					matrix[i] = GetFloatToken();
				}

				// ���̃g�[�N�����p���Ƃ������Ƃ��m�F
				GetToken("}");

				// �t���[���I�u�W�F�N�g�������ł����ꍇ�A�t���[���ԍ��ƃE�F�C�g�𒸓_�o�b�t�@�ɏ�������
				if (frame)
				{
					// ���_�o�b�t�@�ɏ������ނ��߂Ƀ��b�N
					void* vb;
					OriginalVertexBuffer->Lock(0, 0, &vb, 0);
					ORIGINAL_VERTEX* v = (ORIGINAL_VERTEX*)vb;

					// �g���_�ɂ��āA�t���[���ԍ��ƃE�F�C�g����������
					for (DWORD i = 0; i < count; i++)
					{
						// �E�F�C�g�̔z��𒲂ׂāA�܂��E�F�C�g����������ł��Ȃ��v�f��T��
						DWORD j = 0;
						while (j < 3 && v[vertex[i]].Weight[j]>0 && v[vertex[i]].Weight[j] < 1)
						{
							j++;
						}

						// �E�F�C�g����������ł��Ȃ��v�f���������ꍇ�ɂ́A���̗v�f�ɃE�F�C�g���������ށB
						// �܂��A�t���[���ԍ��̔z��ɂ̓E�F�C�g�ɑΉ������ʒu�Ƀt���[���ԍ�����������
						if (j < 3)
						{
							v[vertex[i]].Weight[j] = weight[i];
							v[vertex[i]].Index[j] = (unsigned char)frame->Index;
						}
					}

					// ���_�o�b�t�@�̃A�����b�N
					OriginalVertexBuffer->Unlock();

					// �I�t�Z�b�g�s����t���[���I�u�W�F�N�g�Ɋi�[
					frame->OffsetMatrix = D3DXMATRIXA16(matrix);
				}

				// ���_�ԍ��ƃE�F�C�g�̃o�b�t�@�����
				delete[] vertex;
				delete[] weight;
			}
		}
	}

	// ��ЂÂ�
	delete[] buf;

}

//============================================================================
// �t���[���̊K�w�\���̓ǂݍ���
// �t���[���I�u�W�F�N�g�̃R���X�g���N�^�B����frame�̓��f���̑S�t���[�����i�[�����z��
//============================================================================
CModelFrame::CModelFrame(vector<CModelFrame*>* frame)
{
	// �t���[���ԍ�
	Index = frame->size();

	// �t���[����z��ɒǉ�
	frame->push_back(this);

	// �ϊ��s��ƃI�t�Z�b�g�s����A�Ƃ肠�����P�ʍs��ŏ���������
	D3DXMatrixIdentity(&TransformMatrix);
	D3DXMatrixIdentity(&OffsetMatrix);

	// �t���[�����̎擾
	GetToken();
	Name = new char[strlen(Token) + 1];
	strcpy(Name, Token);

	// ���̃g�[�N�����u�o�v���Ƃ������Ƃ��m�F
	GetToken("{");

	// �ϊ��s��Ǝq�t���[���̓ǂݍ���
	while (*Pointer != '\0')
	{
		// �g�[�N���̎擾
		GetToken();
		// }��ǂݍ��񂾂�I��
		if (strchr(Token, '}'))
		{
			break;
		}

		// �uFrame�v��ǂݍ��񂾂�q�t���[�����Ɣ��f���A�t���[���̓ǂݍ��ݏ������ċA�I�ɌĂяo���B���������q�t���[���͔z��Ɋi�[
		if (strcmp(Token, "Frame") == 0)
		{
			Child.push_back(new CModelFrame(frame));
		}
		else
		{
			// �uFrameTransformMatrix�v��ǂݍ��񂾂�ϊ��s���ǂݍ���
			if (strcmp(Token, "FrameTransformMatrix") == 0)
			{
				// ���̃g�[�N�����u�o�v�ł��邱�Ƃ̊m�F
				GetToken("{");

				// �s��̐����i16�j��ǂݍ���
				float f[16];
				for (int i = 0; i < 16; i++)
				{
					f[i] = GetFloatToken();
				}

				// �ϊ��s��ɐ�����ݒ肷��
				TransformMatrix = D3DXMATRIXA16(f);

				// ���̃g�[�N�����u�p�v�ł��邱�Ƃ��m�F����
				GetToken("}");
			}
			else
			{
				// �q�t���[���ƕϊ��s��ȊO�̗v�f�͓ǂݔ�΂�
				SkipNode();
			}
		}
	}
}

//============================================================================
// �A�j���[�V�����i�s��̍X�V�ƒ��_�ϊ��j
// �t���[���̍X�V
//============================================================================
void CModel::AnimateFrame(D3DXMATRIXA16* world)
{
	// �A�j���[�V�����L�[���֌W����t���[���̕ό`�s���������
	for (size_t i = 0; i < Animation.size(); i++)
	{
		// �E�F�C�g��0�̃A�j���[�V�����ɂ��Ă͏������Ȃ�
		CModelAnimation* anim = Animation[i];
		if (anim->Weight == 0)
		{
			continue;
		}

		// �L�[�Ɋ܂܂��t���[���̕ϊ��s���0�ŏ�����
		for (size_t j = 0; j < anim->Key.size(); j++)
		{
			CModelAnimationKey* key = anim->Key[j];
			ZeroMemory(&Frame[key->FrameIndex]->TransformMatrix, sizeof(D3DXMATRIXA16));
		}
	}

	// �A�j���[�V�����L�[���֌W����t���[���̕ό`�s����v�Z
	for (size_t i = 0; i < Animation.size(); i++)
	{
		// �E�F�C�g��0�̃A�j���[�V�����ɂ��Ă͏������Ȃ�
		CModelAnimation* anim = Animation[i];
		if (anim->Weight == 0)
		{
			continue;
		}

		//�S�ẴL�[�ɂ��ď�������
		for (size_t j = 0; j < anim->Key.size(); j++)
		{
			// �L�[�ƃt���[���̎擾
			CModelAnimationKey* key = anim->Key[j];
			CModelFrame* frame = Frame[key->FrameIndex];

			// �L�[����̏ꍇ�X�L�b�v
			if (key->Time.empty())
			{
				continue;
			}

			// ���݂̎��Ԃ��A�j���[�V�����̗L�����Ԃ����O�̂Ƃ��́A�ŏ��̃L�[���g�p����
			float time = anim->Time;
			if (time < key->Time.front())
			{
				frame->TransformMatrix += *key->Matrix.front()*anim->Weight;
			}

			// ���݂̎��Ԃ��A�j���[�V�����̗L�����Ԃ�����̂Ƃ��́A�Ō�̃L�[���g�p����
			else if (time >= key->Time.back())
			{
				frame->TransformMatrix += *key->Matrix.back()*anim->Weight;
			}

			// ���݂̎��Ԃ��A�j���[�V�����̗L�����ԓ��̏ꍇ
			else
			{
				for (size_t k = 1; k < key->Time.size(); k++)
				{
					// ���݂̎��Ԃ̑O��ɂ���L�[��T��
					if (time < key->Time[k] && key->Time[k - 1] != key->Time[k])
					{
						// �L�[�̋����ɉ������䗦�����߂�
						float r = (key->Time[k] - time) / (key->Time[k] - key->Time[k - 1]);
						frame->TransformMatrix +=
							(*key->Matrix[k - 1] * r + *key->Matrix[k] * (1 - r))*
							anim->Weight;
						break;
					}
				}
			}
		}
	}

	// �t���[���̍����s��ƃX�L�j���O�s����v�Z
	Frame[0]->Animate(world);

	// �A�j���[�V�����؂�ւ����
	if (SmootherWeight > 0)
	{
		for (size_t i = 0; i < Frame.size(); i++)
		{
			D3DXMATRIXA16& a = Frame[i]->CombinedMatrix;
			D3DXMATRIXA16& b = Frame[i]->SmootherMatrix;
			a = a * (1 - SmootherWeight) + b * SmootherWeight;
			Frame[i]->SkinningMatrix = Frame[i]->OffsetMatrix*a;
		}
	}
}

//============================================================================
// ���_�̕ϊ�
//============================================================================
void CModel::AnimateVertex(void)
{
	// �I���W�i�����_�o�b�t�@���Ăяo�����߃��b�N
	void* ovb;
	OriginalVertexBuffer->Lock(0, 0, &ovb, D3DLOCK_READONLY);
	ORIGINAL_VERTEX* ov = (ORIGINAL_VERTEX*)ovb;

	// �A�j���[�V�������_�o�b�t�@�ɏ������ނ��߃��b�N
	void* avb;
	AnimatedVertexBuffer->Lock(0, 0, &avb, 0);
	ANIMATED_VERTEX* av = (ANIMATED_VERTEX*)avb;

	// ���ׂĂ̒��_�ɂ��ĕϊ��������s��
	for (DWORD i = 0; i < VertexCount; i++)
	{
		// ���W�̃x�N�g��������������
		D3DXVECTOR4 pos = D3DXVECTOR4(0, 0, 0, 0);

		// ���_�ɉe������S�Ẵt���[���ɂ��ď�������
		for (DWORD j = 0; j < MODEL_BLEND_COUNT; j++)
		{
			// �t���[���̃X�L�j���O�s����g���Ē��_��ϊ�����
			D3DXVECTOR4 v;
			D3DXVec3Transform(&v, &ov[i].Pos, &Frame[ov[i].Index[j]]->SkinningMatrix);

			// �ϊ����ʂɃE�F�C�g���|�����x�N�g�������v����
			pos += v * ov[i].Weight[j];
		}

		// ���߂����v�l�����W�̃x�N�g���Ƃ���
		av[i].Pos = (D3DXVECTOR3)pos;

		// �@���̏�����
		D3DXVECTOR4 normal = D3DXVECTOR4(0, 0, 0, 0);

		// ���_�ɉe������S�Ẵt���[���ɂ��ď���
		for (DWORD j = 0; j < MODEL_BLEND_COUNT; j++)
		{
			// �t���[���̃X�L�j���O�s����g���Ē��_��ϊ�����
			D3DXVECTOR3& n = ov[i].Normal;
			D3DXVECTOR4 v(n.x, n.y, n.z, 0);
			D3DXVec4Transform(&v, &v, &Frame[ov[i].Index[j]]->SkinningMatrix);

			// �ϊ����ʂɃE�F�C�g���|�����x�N�g�������v����
			normal += v * ov[i].Weight[j];
		}

		// ���߂����v�l�𐳋K���������̂�@���̃x�N�g���Ƃ���
		D3DXVec4Normalize(&normal, &normal);
		av[i].Normal = (D3DXVECTOR3)normal;
	}

	// ���_�o�b�t�@�̃A�����b�N
	OriginalVertexBuffer->Unlock();
	AnimatedVertexBuffer->Unlock();
}
//============================================================================
// �s��̍���
//============================================================================
void CModelFrame::Animate(D3DXMATRIXA16* parent)
{
	// �����s��̌v�Z
	CombinedMatrix = TransformMatrix * (*parent);

	// �S�Ă̎q�t���[���ɂ��āA�ċA�I�ɍs��̍����������Ăяo��
	for (size_t i = 0; i < Child.size(); i++)
	{
		Child[i]->Animate(&CombinedMatrix);
	}

	//�X�L�j���O�s��̌v�Z
	SkinningMatrix = OffsetMatrix * CombinedMatrix;
}


//============================================================================
// ���f���̕`��
// �V�F�[�_�p
//============================================================================
void CModel::Draw(ID3DXEffect* effect, D3DXMATRIXA16* view, D3DXMATRIXA16* proj) {

	// ���_�o�b�t�@�E�C���f�N�X�o�b�t�@�EFVF�̐ݒ�
	Device->SetStreamSource(0, AnimatedVertexBuffer, 0, sizeof(ANIMATED_VERTEX));
	Device->SetIndices(IndexBuffer);
	Device->SetFVF(ANIMATED_FVF);

	// �r���[�ˉe�s��̐ݒ�
	D3DXMATRIXA16 vproj = (*view)*(*proj);
	effect->SetMatrix("VProj", &vproj);

	// �S�ăT�u�Z�b�g��`��
	for (size_t i = 0; i < Subset.size(); i++)
	{
		CModelSubset* subset = Subset[i];

		// �F�A�e�N�X�`���̎w��
		DWORD mi = subset->MaterialIndex;
		effect->SetVector("Diffuse", &Material[mi]->Color);
		effect->SetTexture("MeshTex", Material[mi]->Texture);

		// �G�t�F�N�g��K�����ă|���S����`�悷��
		UINT np;
		effect->Begin(&np, 0);
		for (UINT p = 0; p < np; p++)
		{
			effect->BeginPass(p);
			Device->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST, 0, 0, VertexCount,
				subset->FaceIndex * 3, subset->FaceCount);
			effect->EndPass();
		}
		effect->End();
	}
}

//============================================================================
// ���f���̕`��
// �Œ�p�C�v���C���p
//============================================================================
void CModel::Draw(D3DXMATRIXA16* view, D3DXMATRIXA16* proj)
{
	// ���_�o�b�t�@�E�C���f�N�X�o�b�t�@�EFVF�̐ݒ�
	Device->SetStreamSource(0, AnimatedVertexBuffer, 0, sizeof(ANIMATED_VERTEX));
	Device->SetIndices(IndexBuffer);
	Device->SetFVF(ANIMATED_FVF);

	// ���[���h�s��E�r���[�s��E�ˉe�s��̐ݒ�
	D3DXMATRIXA16 world;
	D3DXMatrixIdentity(&world);
	Device->SetTransform(D3DTS_WORLD, &world);
	Device->SetTransform(D3DTS_VIEW, view);
	Device->SetTransform(D3DTS_PROJECTION, proj);

	// �T�u�Z�b�g�̕`��
	for (size_t i = 0; i < Subset.size(); i++)
	{
		CModelSubset* subset = Subset[i];

		// �F�̐ݒ�
		DWORD mi = subset->MaterialIndex;
		D3DMATERIAL9 material;
		D3DXVECTOR4 c = Material[mi]->Color;
		material.Diffuse.r = c.x;
		material.Diffuse.g = c.y;
		material.Diffuse.b = c.z;
		material.Diffuse.a = c.w;
		material.Specular = material.Diffuse;
		material.Ambient = material.Diffuse;
		material.Emissive.r = 0;
		material.Emissive.g = 0;
		material.Emissive.b = 0;
		material.Emissive.a = 0;
		Device->SetMaterial(&material);

		// �e�N�X�`���̐ݒ�
		Device->SetTexture(0, Material[mi]->Texture);

		// �G�t�F�N�g��K�����ă|���S����`�悷��
		Device->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,				// �����_�����O����v���~�e�B�u�̎��
			0,								// �C���f�b�N�X �o�b�t�@�̊J�n�n�_����ŏ��̒��_�C���f�b�N�X�܂ł̃I�t�Z�b�g
			0,								// ���̌Ăяo���̊ԂɎg�p����钸�_�Q�ɑ΂���ŏ��̒��_�C���f�b�N�X 
			VertexCount,					// ���̌Ăяo���Ŏg�p����钸�_�̐� (BaseVertexIndex + MinIndex ����n�܂�)
			subset->FaceIndex * 3,			// �C���f�b�N�X�z��̒��Œ��_�̓ǂݎ����J�n����ʒu
			subset->FaceCount);				// �����_�����O����v���~�e�B�u�̐�

		//Device->DrawIndexedPrimitiveUP(
		//	D3DPT_TRIANGLELIST,				// �����_�����O����v���~�e�B�u�̎�ނ��L�q����
		//	0,								// ���̌Ăяo���̊ԂɎg�p����钸�_�Q�ɑ΂���A0(pVertexStreamZeroData �̊J�n�ʒu) ����Ƃ���ŏ��̒��_�C���f�b�N�X
		//	VertexCount,					// ���̌Ăяo���Ŏg���钸�_�̐� (MinVertexIndex ����n�܂�)
		//	subset->FaceCount,				// �����_�����O����v���~�e�B�u�̐��B�g����C���f�b�N�X�̐��́A�v���~�e�B�u �J�E���g�̋@�\�ƃv���~�e�B�u�̎�ނł���
		//	,			// �C���f�b�N�X �f�[�^�ɑ΂��郆�[�U�[ ������ �|�C���^
		//	D3DFMT_INDEX16,					// �C���f�b�N�X �f�[�^�̃t�H�[�}�b�g���L�q����16or32
		//	,								// ���_�X�g���[�� 0 �Ɏg�����߂̒��_�f�[�^�̃��[�U�[ ������ �|�C���^
		//	);								// �e���_�̃f�[�^�Ԃ̃X�g���C�h


	}
}


//============================================================================
// �t���[���E�A�j���[�V�����Ȃǂ̑���
CModelFrame* CModel::FindFrame(const char* name)
{
	for (size_t i = 0; i < Frame.size(); i++)
	{
		if (strcmp(Frame[i]->Name, name) == 0)
		{
			return Frame[i];
		}
	}
	return NULL;
}

CModelAnimation* CModel::FindAnimation(const char* name)
{
	for (size_t i = 0; i < Animation.size(); i++)
	{
		if (strcmp(Animation[i]->Name, name) == 0)
		{
			return Animation[i];
		}
	}
	return NULL;
}

void CModel::AddAnimation(CModelAnimation* anim)
{
	Animation.push_back(new CModelAnimation(anim));
}

CModelAnimation::CModelAnimation(const CModelAnimation* anim)
	: Name(anim->Name), Time(anim->Time), Weight(anim->Weight), Key(anim->Key)
{

}

void CModel::ResetAnimationWeight()
{
	for (size_t i = 0; i < Animation.size(); i++)
	{
		Animation[i]->Weight = 0;
	}
}

void CModel::SaveSmoother()
{
	for (size_t i = 0; i < Frame.size(); i++)
	{
		Frame[i]->SmootherMatrix = Frame[i]->CombinedMatrix;
	}
}
