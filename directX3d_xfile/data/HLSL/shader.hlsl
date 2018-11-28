//=============================================================================
//
// シェーダ処理 [shader.hlsl]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================

//====================================================================================================================================================================================
// グローバル変数
//====================================================================================================================================================================================
float4x4 Matrix_Crucible;		//	合成用マトリクス
float4x4 Matrix_World;			//	ワールドマトリクス
float4 Material_Color_Diffuse;	//	マテリアルカラー( 拡散光 )
float4 Material_Color_Ambient;	//	マテリアルカラー( 環境光 )
float4 Light_Color_Diffuse;		//	ライトカラー( 拡散光 )
float4 Light_Color_Ambient;		//	ライトカラー( 環境光 )
float3 Light_Direction;			//	ライトの方向
								//====================================================================================================================================================================================
								// テクスチャ
								//====================================================================================================================================================================================
texture Mesh_Texture;			//	メッシュテクスチャ
texture Shade_Texture;			//	シェードテクスチャ
								//====================================================================================================================================================================================
								// 構造体定義
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
// 頂点シェーダ( トゥーン )
// 関数名	：VERTEX_SHADER_OUTPUT Vertex_Shader_Toon
// 戻り値	：VERTEX_SHADER_OUTPUT
// 引数 1	：float4
// 引数 1	：float3
// 引数 1	：float2
//====================================================================================================================================================================================
VERTEX_SHADER_OUTPUT Vertex_Shader_Toon(float4 In_Position : POSITION, float3 In_Normal_Vector : NORMAL, float2 Texture_Coordinate : TEXCOORD0)
{
	// 変数宣言 / 初期化
	VERTEX_SHADER_OUTPUT Output = (VERTEX_SHADER_OUTPUT)0;

	// 頂点座標
	Output.Position = mul(In_Position, Matrix_Crucible);	//	mul関数："x"と"y"の行列乗算を実行する( 内側の次元の"x"列と、"y"行は等しくなければいけない

															// 頂点テクスチャ
															// max関数："x"と"y"のうちの、大きい方の値を選択する
															// dot関数：２つのベクトルの、内積を返す
	Output.Shade_Texture_UV.x = max(dot(mul(In_Normal_Vector, (float3x3)Matrix_World), -Light_Direction), 0);
	Output.Shade_Texture_UV.y = 0.5f;

	// 頂点カラー
	Output.Color = Material_Color_Diffuse * Light_Color_Diffuse + Light_Color_Ambient;

	// 固定頂点方式の頂点色計算
	//float3 Normal_W = normalize(mul(In_Normal_Vector, (float3x3)Matrix_World));
	//Output.Color = Material_Color_Diffuse * Light_Color_Diffuse * max(dot(-Light_Direction, Normal_W), 0) + Material_Color_Ambient * Light_Color_Ambient;

	// テクスチャマッピング
	Output.Texture_UV = Texture_Coordinate;

	// 終了
	return Output;
}

////====================================================================================================================================================================================
//// 頂点シェーダ( アウトライン )
//// 関数名	：float4 Vertex_Shader_Outline
//// 戻り値	：float4
//// 引数 1	：float4
//// 引数 1	：float3
//// 引数 1	：float2
////====================================================================================================================================================================================
//float4 Vertex_Shader_Outline(float4 Position : POSITION, float3 Normal_Vector : NORMAL, float2 Texture_Coordinate : TEXCOORD0) : POSITION
//{
//	// 変数宣言 / 初期化
//	float4 Parameter_A = mul(Position, Matrix_Crucible);
//	float4 Parameter_B = mul(Normal_Vector, Matrix_Crucible);
//
//	Parameter_B.zw = 0;
//	Parameter_B = normalize(Parameter_B) * 0.005;	//	ラインの太さ
//
//	// 終了
//	return Parameter_B * Parameter_A.w + Parameter_A;
//}

//====================================================================================================================================================================================
// ピクセルシェーダ( トゥーン )
// 関数名	：float4 Pixel_Shader_Toon
// 戻り値	：float4
// 引数 1	：VERTEX_SHADER_OUTPUT
//====================================================================================================================================================================================
float4 Pixel_Shader_Toon(VERTEX_SHADER_OUTPUT Input) : COLOR0
{
	// 終了
	// tex2D関数：2Dテクスチャをサンプリングする
	return tex2D(Mesh_Texture_Sampler, Input.Texture_UV) * tex2D(Shade_Texture_Sampler, Input.Shade_Texture_UV) * Input.Color;
}

//====================================================================================================================================================================================
// テクニック( トゥーン )
// 関数名	：technique Toon_Shader
// 戻り値	：technique
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
//// テクニック( アウトライン )
//// 関数名	：technique Outline_Shader
//// 戻り値	：technique
////====================================================================================================================================================================================
//technique Outline_Shader
//{
//	pass P0
//	{
//		VertexShader = compile vs_2_0 Vertex_Shader_Outline();
//	}
//}