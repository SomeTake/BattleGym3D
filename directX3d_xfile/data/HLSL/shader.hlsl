//=============================================================================
//
// �V�F�[�_���� [shader.hlsl]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================

//====================================================================================================================================================================================
// �O���[�o���ϐ�
//====================================================================================================================================================================================
float4x4 Matrix_Crucible;		//	�����p�}�g���N�X
float4x4 Matrix_World;			//	���[���h�}�g���N�X
float4 Material_Color_Diffuse;	//	�}�e���A���J���[( �g�U�� )
float4 Material_Color_Ambient;	//	�}�e���A���J���[( ���� )
float4 Light_Color_Diffuse;		//	���C�g�J���[( �g�U�� )
float4 Light_Color_Ambient;		//	���C�g�J���[( ���� )
float3 Light_Direction;			//	���C�g�̕���
								//====================================================================================================================================================================================
								// �e�N�X�`��
								//====================================================================================================================================================================================
texture Mesh_Texture;			//	���b�V���e�N�X�`��
texture Shade_Texture;			//	�V�F�[�h�e�N�X�`��
								//====================================================================================================================================================================================
								// �\���̒�`
								//====================================================================================================================================================================================
sampler Mesh_Texture_Sampler = sampler_state
{
	Texture = <Mesh_Texture>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
sampler Shade_Texture_Sampler = sampler_state
{
	Texture = <Shade_Texture>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
typedef struct
{
	float4 Position			: POSITION;
	float4 Color			: COLOR0;
	float2 Texture_UV		: TEXCOORD0;
	float2 Shade_Texture_UV	: TEXCOORD1;
}VERTEX_SHADER_OUTPUT;
//====================================================================================================================================================================================
// ���_�V�F�[�_( �g�D�[�� )
// �֐���	�FVERTEX_SHADER_OUTPUT Vertex_Shader_Toon
// �߂�l	�FVERTEX_SHADER_OUTPUT
// ���� 1	�Ffloat4
// ���� 1	�Ffloat3
// ���� 1	�Ffloat2
//====================================================================================================================================================================================
VERTEX_SHADER_OUTPUT Vertex_Shader_Toon(float4 In_Position : POSITION, float3 In_Normal_Vector : NORMAL, float2 Texture_Coordinate : TEXCOORD0)
{
	// �ϐ��錾 / ������
	VERTEX_SHADER_OUTPUT Output = (VERTEX_SHADER_OUTPUT)0;

	// ���_���W
	Output.Position = mul(In_Position, Matrix_Crucible);	//	mul�֐��F"x"��"y"�̍s���Z�����s����( �����̎�����"x"��ƁA"y"�s�͓������Ȃ���΂����Ȃ�

															// ���_�e�N�X�`��
															// max�֐��F"x"��"y"�̂����́A�傫�����̒l��I������
															// dot�֐��F�Q�̃x�N�g���́A���ς�Ԃ�
	Output.Shade_Texture_UV.x = max(dot(mul(In_Normal_Vector, (float3x3)Matrix_World), -Light_Direction), 0);
	Output.Shade_Texture_UV.y = 0.5f;

	// ���_�J���[
	Output.Color = Material_Color_Diffuse * Light_Color_Diffuse + Light_Color_Ambient;

	// �Œ蒸�_�����̒��_�F�v�Z
	//float3 Normal_W = normalize(mul(In_Normal_Vector, (float3x3)Matrix_World));
	//Output.Color = Material_Color_Diffuse * Light_Color_Diffuse * max(dot(-Light_Direction, Normal_W), 0) + Material_Color_Ambient * Light_Color_Ambient;

	// �e�N�X�`���}�b�s���O
	Output.Texture_UV = Texture_Coordinate;

	// �I��
	return Output;
}

////====================================================================================================================================================================================
//// ���_�V�F�[�_( �A�E�g���C�� )
//// �֐���	�Ffloat4 Vertex_Shader_Outline
//// �߂�l	�Ffloat4
//// ���� 1	�Ffloat4
//// ���� 1	�Ffloat3
//// ���� 1	�Ffloat2
////====================================================================================================================================================================================
//float4 Vertex_Shader_Outline(float4 Position : POSITION, float3 Normal_Vector : NORMAL, float2 Texture_Coordinate : TEXCOORD0) : POSITION
//{
//	// �ϐ��錾 / ������
//	float4 Parameter_A = mul(Position, Matrix_Crucible);
//	float4 Parameter_B = mul(Normal_Vector, Matrix_Crucible);
//
//	Parameter_B.zw = 0;
//	Parameter_B = normalize(Parameter_B) * 0.005;	//	���C���̑���
//
//	// �I��
//	return Parameter_B * Parameter_A.w + Parameter_A;
//}

//====================================================================================================================================================================================
// �s�N�Z���V�F�[�_( �g�D�[�� )
// �֐���	�Ffloat4 Pixel_Shader_Toon
// �߂�l	�Ffloat4
// ���� 1	�FVERTEX_SHADER_OUTPUT
//====================================================================================================================================================================================
float4 Pixel_Shader_Toon(VERTEX_SHADER_OUTPUT Input) : COLOR0
{
	// �I��
	// tex2D�֐��F2D�e�N�X�`�����T���v�����O����
	return tex2D(Mesh_Texture_Sampler, Input.Texture_UV) * tex2D(Shade_Texture_Sampler, Input.Shade_Texture_UV) * Input.Color;
}

//====================================================================================================================================================================================
// �e�N�j�b�N( �g�D�[�� )
// �֐���	�Ftechnique Toon_Shader
// �߂�l	�Ftechnique
//====================================================================================================================================================================================
technique Toon_Shader
{
	pass P0
	{
		VertexShader = compile vs_2_0 Vertex_Shader_Toon();
		//PixelShader = compile ps_2_0 Pixel_Shader_Toon();
	}
}

////====================================================================================================================================================================================
//// �e�N�j�b�N( �A�E�g���C�� )
//// �֐���	�Ftechnique Outline_Shader
//// �߂�l	�Ftechnique
////====================================================================================================================================================================================
//technique Outline_Shader
//{
//	pass P0
//	{
//		VertexShader = compile vs_2_0 Vertex_Shader_Outline();
//	}
//}