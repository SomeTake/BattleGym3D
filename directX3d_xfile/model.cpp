//=============================================================================
//
// Xファイル読み込み処理 [particle.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#include "model.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// トークン
static char* Pointer;						// ファイルの読み込み位置
static char Token[1024];					// 読み込んだトークン

// テクスチャ
static vector<IDirect3DTexture9*> Texture;	// 読み込んだテクスチャの一覧
static vector<char*> TextureName;			// 読み込んだテクスチャファイル名の一覧

//=============================================================================
// テクスチャの読み込み
//=============================================================================
IDirect3DTexture9* GetTexture(IDirect3DDevice9* device, const char* file_name)
{
	// 指定されたテクスチャが読み込み済みの場合には、重複して読み込まずに、以前に読み込んだテクスチャを返す
	for (size_t i = 0; i < TextureName.size(); i++)
	{
		if (strcmp(TextureName[i], file_name) == 0)
		{
			return Texture[i];
		}
	}
	IDirect3DTexture9* texture = NULL;

	// テクスチャを読み込む
	D3DXCreateTextureFromFileEx(
		device,							// インターフェイスへのポインタ
		file_name,						// ファイル名を指定する文字列へのポインタ
		D3DX_DEFAULT,					// 幅。0またはD3DX_DEFAULTの場合ファイルから取得
		D3DX_DEFAULT,					// 高さ。0またはD3DX_DEFAULTの場合ファイルから取得
		D3DX_DEFAULT,					// 要求されるミップレベルの数。0またはD3DX_DEFAULTの場合完全なミップマップチェーンが作成
		0,								// 0、D3DUSAGE_RENDERTARGET、または D3DUSAGE_DYNAMIC。このフラグを D3DUSAGE_RENDERTARGET に設定すると、そのサーフェイスはレンダリング ターゲットとして使われることを示す
		D3DFMT_UNKNOWN,					// D3DFORMAT 列挙型のメンバ。テクスチャに対して要求されたピクセル フォーマットを記述する。返されるテクスチャのフォーマットは、Format で指定したフォーマットと異なる場合がある。D3DFMT_UNKNOWN の場合、フォーマットはファイルから取得される。
		D3DPOOL_MANAGED,				// D3DPOOL 列挙型のメンバ。テクスチャの配置先となるメモリ クラスを記述する。
		D3DX_DEFAULT,					// イメージをフィルタリングする方法を制御する 1 つあるいは複数の D3DX_FILTER の組み合わせ。このパラメータに D3DX_DEFAULT を指定することは、D3DX_FILTER_TRIANGLE | D3DX_FILTER_DITHER を指定することと等しい。
		D3DX_DEFAULT,					// イメージをフィルタリングする方法を制御する 1 つあるいは複数の D3DX_FILTER の組み合わせ。このパラメータに D3DX_DEFAULT を指定することは、D3DX_FILTER_BOX を指定することと等しい。
		0,								// 透明となる D3DCOLOR の値。カラーキーを無効にする場合は 0 を指定する。
		NULL,							// ソース イメージ ファイル内のデータの記述を格納する D3DXIMAGE_INFO 構造体へのポインタ
		NULL,							// 格納する 256 色パレットを表す PALETTEENTRY 構造体へのポインタ
		&texture);						// 作成されたキューブ テクスチャ オブジェクトを表す、IDirect3DTexture9 インターフェイスへのポインタのアドレス

	// テクスチャを配列に格納
	Texture.push_back(texture);

	// テクスチャファイル名を配列に格納
	char* t = new char[strlen(file_name) + 1];
	strcpy(t, file_name);
	TextureName.push_back(t);

	// 読み込んだテクスチャを返す
	return texture;
}
//=============================================================================
// エラー表示
//=============================================================================
static void Error(const char* message)
{
	// ダイアログボックスの表示
	MessageBox(NULL, message, "モデルの読み込みエラー", MB_OK);
	PostQuitMessage(0);
}

//=============================================================================
// トークンの取得
//=============================================================================
static void GetToken(void)
{
	// p=読み込み位置、q=読み込んだトークン
	char* p = Pointer;
	char* q = Token;

	// 区切り文字を読み飛ばす
	while (*p != '\0' && strchr(" \t\r\n,;\"", *p))
	{
		p++;
	}

	// {}は単体でトークンとする
	if (*p == '{' || *p == '}')
	{
		*q++ = *p++;
	}

	// {}以外を読み込んだ場合、再び区切り文字が現れるまでの部分をトークンとする
	else {
		while (*p != '\0' && !strchr(" \t\r\n,;\"{}", *p))
		{
			*q++ = *p++;
		}
	}

	// 読み込み位置の更新
	Pointer = p;
	// トークンの末尾にNULLの追加
	*q = '\0';
}

//=============================================================================
// 指定したトークンの取得
//=============================================================================
static void GetToken(const char* token)
{
	// トークンの読み込み
	GetToken();

	// 読み込んだトークンが引数が示す文字列と異なる場合、エラーを表示して終了
	if (strcmp(Token, token) != 0)
	{
		char s[1024];
		sprintf_s(s, sizeof(s), "想定トークン：%s\n、読み込みトークン：%s", token, Token);
		Error(s);
	}
}

//=============================================================================
// 数値データの取得（float型）
//=============================================================================
static float GetFloatToken(void)
{
	// トークンの読み込み
	GetToken();
	// float型にして返す
	return atof(Token);
}

//=============================================================================
// 数値データの取得（Int型）
//=============================================================================
static int GetIntToken(void)
{
	// トークンの読み込み
	GetToken();
	// int型にして返す
	return atoi(Token);
}

//=============================================================================
// ノードのスキップ処理
//=============================================================================
static void SkipNode(void)
{
	// {が出現するまで読み飛ばす
	while (*Pointer != '\0') {
		GetToken();
		if (strchr(Token, '{'))
		{
			break;
		}
	}
	// {の数を表すカウンタ
	int count = 1;

	// ファイルの末尾に達するか、}が読み込まれるまで（対応する}が読み込まれるとカウンタが0に戻る）
	while (*Pointer != '\0' && count > 0) {

		// トークンの取得
		GetToken();

		// {を読み込んだらカウンタを+1
		if (strchr(Token, '{'))
		{
			count++;
		}
		// }を読み込んだらカウンタを-1
		else if (strchr(Token, '}'))
		{
			count--;
		}
	}

	// カウンタが1以上で終了したら括弧の対応関係がおかしいのでエラー表示して終了
	if (count > 0) {
		Error("括弧の非対応");
		return;
	}
}

//=============================================================================
// モデル読み込み
//=============================================================================
CModel::CModel(IDirect3DDevice9* device, const char* file_name, bool anim_only)
	//: Device(device),
	//OriginalVertexBuffer(NULL), AnimatedVertexBuffer(NULL), VertexCount(0),
	//IndexBuffer(NULL), FaceCount(0), SmootherWeight(0)
{
	// ファイルを開く
	HANDLE file = CreateFile(
		file_name,					// ファイルの名前
		GENERIC_READ,				// アクセスモード
		FILE_SHARE_READ,			// 共有モード
		NULL,						// セキュリティ記述子
		OPEN_EXISTING,				// 作成方法
		FILE_ATTRIBUTE_READONLY,	// ファイル属性
		NULL);						// テンプレートファイルのハンドル

	// ファイルが開けなかった時はメッセージを表示して終了
	if (file == INVALID_HANDLE_VALUE)
	{
		char s[1024];
		sprintf_s(s, sizeof(s), "ファイルの読み込みに失敗", file_name);
		Error(s);
		return;
	}

	// ファイルのサイズを取得し、読み込み用のバッファを確保する
	DWORD size = GetFileSize(file, NULL);
	char* buf = new char[size + 1];
	buf[size] = '\0';

	// ファイルをバッファに読み込む
	DWORD read_size;
	ReadFile(file, buf, size, &read_size, NULL);

	// ファイルを閉じて、読み込んだサイズをチェックする
	CloseHandle(file);
	if (read_size != size) return;

	// テンプレートを読み飛ばす
	// ファイルの先頭から読み込む
	Pointer = buf;

	// ファイルの末尾に達するまで繰り返す
	while (*Pointer != '\0')
	{
		// トークンの取得
		GetToken();

		// テンプレート
		if (strcmp(Token, "template") == 0) {
			// 要素を読み飛ばす
			SkipNode();
		}
		// 読み込んだトークンが「Frame」の場合、フレーム要素だと判断する
		else if (strcmp(Token, "Frame") == 0)
		{
			// Xファイルに複数のルートフレームがある場合にはエラーを表示して終了する
			if (!Frame.empty())
			{
				Error("ファイル内に複数のルートフレーム");
				return;
			}

			// フレームの階層構造を読み込む
			new CModelFrame(&Frame);
		}

		// アニメーションセットの読み込み
		// 読み込んだトークンが「AnimationSet」のとき、アニメーション要素だと判断
		else if (strcmp(Token, "AnimationSet") == 0)
		{
			// 格納するオブジェクトの作成
			CModelAnimation* anim = new CModelAnimation();

			// アニメーションの配列にオブジェクトを格納
			Animation.push_back(anim);

			// 時間とウェイトの初期化
			anim->Time = 0;
			anim->Weight = 0;

			// アニメーション名の取得
			GetToken();
			anim->Name = new char[strlen(Token) + 1];
			strcpy(anim->Name, Token);

			// 次のトークンが「｛」だということを確認
			GetToken("{");

			// 末尾まで読み込む
			while (*Pointer != '\0')
			{
				// トークンの取得
				GetToken();

				// 「｝」があったら終了
				if (strchr(Token, '}'))
				{
					break;
				}

				// 「Animation」ｗ読み込んだらアニメーション要素だと判断
				if (strcmp(Token, "Animation") == 0)
				{
					// キーのオブジェクトを作成し、配列に格納
					CModelAnimationKey* key = new CModelAnimationKey();
					anim->Key.push_back(key);

					// 名前を読み飛ばす
					GetToken();

					// 次のトークンが2個の「｛」だということを確認
					GetToken("{");
					GetToken("{");

					// フレーム名を取得する
					GetToken();
					key->FrameName = new char[strlen(Token) + 1];
					strcpy(key->FrameName, Token);

					// フレーム名に対応するフレームのオブジェクトを検索し、フレーム番号を取得する
					key->FrameIndex = FindFrame(key->FrameName)->Index;

					// 次のトークンが「｝」だということを確認
					GetToken("}");

					// 次のトークンが「AnimationKey」「｛」「4」だということを確認
					GetToken("AnimationKey");
					GetToken("{");
					GetToken("4");

					// キーの数を取得する
					DWORD key_count = GetIntToken();

					// 全てのキーを取得する
					for (DWORD i = 0; i < key_count; i++)
					{
						// 時間の取得
						key->Time.push_back((float)GetIntToken());

						// 次のトークンが16だということを確認
						GetToken("16");

						// 行列の取得
						float f[16];
						for (DWORD j = 0; j < 16; j++)
						{
							f[j] = GetFloatToken();
						}
						key->Matrix.push_back(new D3DXMATRIXA16(f));
					}

					// 次のトークンが２個の「｝」だということを確認
					GetToken("}");
					GetToken("}");
				}
			}
		}
	}

	// ファイルの先頭から読み込む
	Pointer = buf;

	// アニメーションデータ以外も読み込む場合
	if (!anim_only)
	{
		// ファイルの末尾に達するまで
		while (*Pointer != '\0')
		{
			//トークンの取得
			GetToken();

			// テンプレートのスキップ
			if (strcmp(Token, "template") == 0)
			{
				SkipNode();
			}
			// メッシュの読み込み
			else if (strcmp(Token, "Mesh") == 0)
			{
				// メッシュ情報が複数あるファイルには対応していない
				// ワンスキンモデルにのみ対応
				if (OriginalVertexBuffer)
				{
					Error("ファイル内に複数のメッシュ");
					return;
				}

				GetToken();
				GetToken("{");

				// 頂点座標の読み込み
				// 頂点数を取得
				VertexCount = GetIntToken();

				// オリジナル頂点バッファの作成
				device->CreateVertexBuffer(
					sizeof(ORIGINAL_VERTEX)*VertexCount, 0,
					ORIGINAL_FVF, D3DPOOL_SYSTEMMEM,
					&OriginalVertexBuffer, NULL);

				// アニメーション頂点バッファの作成
				device->CreateVertexBuffer(
					sizeof(ANIMATED_VERTEX)*VertexCount, 0,
					ANIMATED_FVF, D3DPOOL_MANAGED,
					&AnimatedVertexBuffer, NULL);

				// 頂点バッファが正しく作成できなければエラーを表示する
				if (!OriginalVertexBuffer || !AnimatedVertexBuffer)
				{
					Error("頂点バッファの作成に失敗");
					return;
				}

				// 頂点バッファに書き込むためロックする
				void* vb;
				OriginalVertexBuffer->Lock(0, 0, &vb, 0);
				ORIGINAL_VERTEX* v = (ORIGINAL_VERTEX*)vb;

				// 頂点座標を書き込む
				for (DWORD i = 0; i < VertexCount; i++)
				{
					// 頂点座標を取得して書き込む
					v[i].Pos.x = GetFloatToken();
					v[i].Pos.y = GetFloatToken();
					v[i].Pos.z = GetFloatToken();
					// ボーン番号とボーンウェイトを初期化する
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

				// 頂点バッファのアンロック
				OriginalVertexBuffer->Unlock();

				// 面の読み込み
				// 面数を取得
				FaceCount = GetIntToken();

				// インデックスバッファの作成
				device->CreateIndexBuffer(
					sizeof(WORD) * 3 * FaceCount, 0,
					D3DFMT_INDEX16, D3DPOOL_MANAGED,
					&IndexBuffer, NULL);

				// インデックスバッファが正しく作成できなければエラーを表示
				if (!IndexBuffer)
				{
					Error("インデックスバッファの作成に失敗");
					return;
				}

				// インデックスバッファに書き込むためロックする
				void* ib;
				IndexBuffer->Lock(0, 0, &ib, 0);
				WORD* index = (WORD*)ib;

				// 頂点番号を書き込む
				for (DWORD i = 0, n = FaceCount * 3; i < n; i += 3)
				{
					GetToken("3");
					for (DWORD j = 0; j < 3; j++)
					{
						index[i + j] = GetIntToken();
					}
				}

				// インデックスバッファのアンロック
				IndexBuffer->Unlock();
			}
			// メッシュ法線の読み込み
			// メッシュ法線要素だと判断する
			else if (strcmp(Token, "MeshNormals") == 0)
			{
				// 次のトークンが｛だということを確認する
				GetToken("{");

				// 頂点数を取得し、メッシュ要素で指定された頂点数と異なる場合にはエラーを表示する
				if (GetIntToken() != VertexCount)
				{
					Error("頂点数と法線数が不一致");
					return;
				}

				// 頂点バッファに書き込むためロックする
				void* vb;
				OriginalVertexBuffer->Lock(0, 0, &vb, 0);
				ORIGINAL_VERTEX* v = (ORIGINAL_VERTEX*)vb;

				// 法線データを書き込む
				for (DWORD i = 0; i < VertexCount; i++)
				{
					v[i].Normal.x = GetFloatToken();
					v[i].Normal.y = GetFloatToken();
					v[i].Normal.z = GetFloatToken();
				}

				// 頂点バッファのアンロック
				OriginalVertexBuffer->Unlock();

				// 面数を取得し、メッシュ要素で指定された面数と異なる場合にはエラーを表示する
				if (GetIntToken() != FaceCount)
				{
					Error("面数と法線インデックス数が不一致");
					return;
				}

				// 法線番号は読み飛ばす
				for (DWORD i = 0, n = FaceCount * 4; i < n; i++)
				{
					GetToken();
				}
			}
			// 読み込んだトークンが「MeshTextureCoords」のときには、メッシュテクスチャ座標要素だと判断する

			else if (strcmp(Token, "MeshTextureCoords") == 0)
			{
				// 次のトークンgファ｛であることの確認
				GetToken("{");

				// 取得した頂点数とメッシュ要素で指定された頂点数が異なる場合にはエラー表示
				if (GetIntToken() != VertexCount)
				{
					Error("頂点数とテクスチャ座標数が不一致");
					return;
				}

				// 頂点バッファに書き込むためオリジナル頂点バッファをロック
				void* ovb;
				OriginalVertexBuffer->Lock(0, 0, &ovb, 0);
				ORIGINAL_VERTEX* ov = (ORIGINAL_VERTEX*)ovb;

				// 頂点バッファに書き込むためアニメーション頂点バッファをロック
				void* avb;
				AnimatedVertexBuffer->Lock(0, 0, &avb, 0);
				ANIMATED_VERTEX* av = (ANIMATED_VERTEX*)avb;

				// テクスチャ座標を書き込む
				for (DWORD i = 0; i < VertexCount; i++)
				{
					ov[i].UV.x = av[i].UV.x = GetFloatToken();
					ov[i].UV.y = av[i].UV.y = GetFloatToken();
				}

				// 頂点バッファのアンロック
				OriginalVertexBuffer->Unlock();
				AnimatedVertexBuffer->Unlock();
			}
			// メッシュマテリアルリストの読み込み
			// 読み込んだトークンが「MeshMaterialList」のときにはメッシュマテリアルリスト要素だと判断する
			else if (strcmp(Token, "MeshMaterialList") == 0)
			{
				// 次のトークンが｛だということを確認する
				GetToken("{");

				// マテリアル数の取得
				DWORD material_count = GetIntToken();

				// 取得した面数とメッシュ要素で指定された面数が違っていればエラーを表示
				if (GetIntToken() != FaceCount)
				{
					Error("面数とマテリアルリスト数の不一致");
					return;
				}

				// サブセットの作成
				CModelSubset* subset = NULL;
				DWORD prev_index = -1;
				for (DWORD i = 0; i < FaceCount; i++)
				{
					// マテリアル番号の取得
					DWORD index = GetIntToken();

					// 取得したマテリアル番号が直前に取得したマテリアル番号と違う時には新しいサブセットを作成
					if (index != prev_index)
					{
						// サブセットの作成
						subset = new CModelSubset();
						// 面数・開始面番号・マテリアル番号の設定
						subset->FaceCount = 1;
						subset->FaceIndex = i;
						subset->MaterialIndex = index;

						// サブセットを配列に格納
						Subset.push_back(subset);

						// 次に取得するマテリアル番号と比較するため、取得したマテリアル番号を保存する
						prev_index = index;
					}
					else
					{
						// 取得したマテリアル番号が、直前に取得したマテリアル番号と一致するときには、前回作成したサブセットの面数を増やす
						subset->FaceCount++;
					}
				}

				// マテリアルの読み込み
				// メッシュマテリアルリスト要素に書かれていた個数に応じて、マテリアル要素を読み込む
				for (DWORD i = 0; i < material_count; i++)
				{
					// マテリアルのオブジェクトを作成
					CModelMaterial* material = new CModelMaterial();

					// マテリアルを配列に追加
					Material.push_back(material);

					// 次のトークンが「Material」と「｛」だということを確認
					GetToken("Material");
					GetToken("{");

					// 基本色を取得し、オブジェクトに格納
					material->Color = D3DXVECTOR4(
						GetFloatToken(), GetFloatToken(),
						GetFloatToken(), GetFloatToken());

					// ハイライト強度・ハイライト色・自己発光色は読み飛ばす
					for (int j = 0; j < 7; j++)
					{
						GetToken();
					}

					// 次のトークンが「TextureFilename」と「｛」だということを確認
					GetToken("TextureFilename");
					GetToken("{");

					// テクスチャファイル名を取得し、テクスチャを作成してオブジェクトに格納
					GetToken();
					material->Texture = GetTexture(Device, Token);

					// 次のトークンが２個の「｝」だということを確認
					GetToken("}");
					GetToken("}");
				}
			}
			// 読み込んだトークンが「SkinWeights」だった場合、スキンウェイト要素だと判断する
			else if (strcmp(Token, "SkinWeights") == 0)
			{
				// トークンが｛であることを確認
				GetToken("{");

				// フレームを取得し、対応するオブジェクトを検索する
				GetToken();
				CModelFrame* frame = FindFrame(Token);

				// ウェイト数の取得
				DWORD count = GetIntToken();

				// 頂点番号とウェイトを一時的に格納するためのバッファを確保
				DWORD* vertex = new DWORD[count];
				float* weight = new float[count];

				// オフセット行列の成分を格納するための配列
				float matrix[16];

				// 頂点番号の取得
				for (DWORD i = 0; i < count; i++)
				{
					vertex[i] = GetIntToken();
				}

				// ウェイトの取得
				for (DWORD i = 0; i < count; i++)
				{
					weight[i] = GetFloatToken();
				}

				// オフセット行列の取得
				for (DWORD i = 0; i < 16; i++)
				{
					matrix[i] = GetFloatToken();
				}

				// 次のトークンが｝だということを確認
				GetToken("}");

				// フレームオブジェクトが検索できた場合、フレーム番号とウェイトを頂点バッファに書き込む
				if (frame)
				{
					// 頂点バッファに書き込むためにロック
					void* vb;
					OriginalVertexBuffer->Lock(0, 0, &vb, 0);
					ORIGINAL_VERTEX* v = (ORIGINAL_VERTEX*)vb;

					// 拡張点について、フレーム番号とウェイトを書き込む
					for (DWORD i = 0; i < count; i++)
					{
						// ウェイトの配列を調べて、まだウェイトを書き込んでいない要素を探す
						DWORD j = 0;
						while (j < 3 && v[vertex[i]].Weight[j]>0 && v[vertex[i]].Weight[j] < 1)
						{
							j++;
						}

						// ウェイトを書き込んでいない要素があった場合には、その要素にウェイトを書き込む。
						// また、フレーム番号の配列にはウェイトに対応した位置にフレーム番号を書き込む
						if (j < 3)
						{
							v[vertex[i]].Weight[j] = weight[i];
							v[vertex[i]].Index[j] = (unsigned char)frame->Index;
						}
					}

					// 頂点バッファのアンロック
					OriginalVertexBuffer->Unlock();

					// オフセット行列をフレームオブジェクトに格納
					frame->OffsetMatrix = D3DXMATRIXA16(matrix);
				}

				// 頂点番号とウェイトのバッファを解放
				delete[] vertex;
				delete[] weight;
			}
		}
	}

	// 後片づけ
	delete[] buf;

}

//============================================================================
// フレームの階層構造の読み込み
// フレームオブジェクトのコンストラクタ。引数frameはモデルの全フレームを格納した配列
//============================================================================
CModelFrame::CModelFrame(vector<CModelFrame*>* frame)
{
	// フレーム番号
	Index = frame->size();

	// フレームを配列に追加
	frame->push_back(this);

	// 変換行列とオフセット行列を、とりあえず単位行列で初期化する
	D3DXMatrixIdentity(&TransformMatrix);
	D3DXMatrixIdentity(&OffsetMatrix);

	// フレーム名の取得
	GetToken();
	Name = new char[strlen(Token) + 1];
	strcpy(Name, Token);

	// 次のトークンが「｛」だということを確認
	GetToken("{");

	// 変換行列と子フレームの読み込み
	while (*Pointer != '\0')
	{
		// トークンの取得
		GetToken();
		// }を読み込んだら終了
		if (strchr(Token, '}'))
		{
			break;
		}

		// 「Frame」を読み込んだら子フレームだと判断し、フレームの読み込み処理を再帰的に呼び出す。生成した子フレームは配列に格納
		if (strcmp(Token, "Frame") == 0)
		{
			Child.push_back(new CModelFrame(frame));
		}
		else
		{
			// 「FrameTransformMatrix」を読み込んだら変換行列を読み込む
			if (strcmp(Token, "FrameTransformMatrix") == 0)
			{
				// 次のトークンが「｛」であることの確認
				GetToken("{");

				// 行列の成分（16個）を読み込む
				float f[16];
				for (int i = 0; i < 16; i++)
				{
					f[i] = GetFloatToken();
				}

				// 変換行列に成分を設定する
				TransformMatrix = D3DXMATRIXA16(f);

				// 次のトークンが「｝」であることを確認する
				GetToken("}");
			}
			else
			{
				// 子フレームと変換行列以外の要素は読み飛ばす
				SkipNode();
			}
		}
	}
}

//============================================================================
// アニメーション（行列の更新と頂点変換）
// フレームの更新
//============================================================================
void CModel::AnimateFrame(D3DXMATRIXA16* world)
{
	// アニメーションキーが関係するフレームの変形行列を初期化
	for (size_t i = 0; i < Animation.size(); i++)
	{
		// ウェイトが0のアニメーションについては処理しない
		CModelAnimation* anim = Animation[i];
		if (anim->Weight == 0)
		{
			continue;
		}

		// キーに含まれるフレームの変換行列を0で初期化
		for (size_t j = 0; j < anim->Key.size(); j++)
		{
			CModelAnimationKey* key = anim->Key[j];
			ZeroMemory(&Frame[key->FrameIndex]->TransformMatrix, sizeof(D3DXMATRIXA16));
		}
	}

	// アニメーションキーが関係するフレームの変形行列を計算
	for (size_t i = 0; i < Animation.size(); i++)
	{
		// ウェイトが0のアニメーションについては処理しない
		CModelAnimation* anim = Animation[i];
		if (anim->Weight == 0)
		{
			continue;
		}

		//全てのキーについて処理する
		for (size_t j = 0; j < anim->Key.size(); j++)
		{
			// キーとフレームの取得
			CModelAnimationKey* key = anim->Key[j];
			CModelFrame* frame = Frame[key->FrameIndex];

			// キーが空の場合スキップ
			if (key->Time.empty())
			{
				continue;
			}

			// 現在の時間がアニメーションの有効時間よりも前のときは、最初のキーを使用する
			float time = anim->Time;
			if (time < key->Time.front())
			{
				frame->TransformMatrix += *key->Matrix.front()*anim->Weight;
			}

			// 現在の時間がアニメーションの有効時間よりも後のときは、最後のキーを使用する
			else if (time >= key->Time.back())
			{
				frame->TransformMatrix += *key->Matrix.back()*anim->Weight;
			}

			// 現在の時間がアニメーションの有効時間内の場合
			else
			{
				for (size_t k = 1; k < key->Time.size(); k++)
				{
					// 現在の時間の前後にあるキーを探す
					if (time < key->Time[k] && key->Time[k - 1] != key->Time[k])
					{
						// キーの距離に応じた比率を求める
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

	// フレームの合成行列とスキニング行列を計算
	Frame[0]->Animate(world);

	// アニメーション切り替え補間
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
// 頂点の変換
//============================================================================
void CModel::AnimateVertex(void)
{
	// オリジナル頂点バッファを呼び出すためロック
	void* ovb;
	OriginalVertexBuffer->Lock(0, 0, &ovb, D3DLOCK_READONLY);
	ORIGINAL_VERTEX* ov = (ORIGINAL_VERTEX*)ovb;

	// アニメーション頂点バッファに書き込むためロック
	void* avb;
	AnimatedVertexBuffer->Lock(0, 0, &avb, 0);
	ANIMATED_VERTEX* av = (ANIMATED_VERTEX*)avb;

	// すべての頂点について変換処理を行う
	for (DWORD i = 0; i < VertexCount; i++)
	{
		// 座標のベクトルを初期化する
		D3DXVECTOR4 pos = D3DXVECTOR4(0, 0, 0, 0);

		// 頂点に影響する全てのフレームについて処理する
		for (DWORD j = 0; j < MODEL_BLEND_COUNT; j++)
		{
			// フレームのスキニング行列を使って頂点を変換する
			D3DXVECTOR4 v;
			D3DXVec3Transform(&v, &ov[i].Pos, &Frame[ov[i].Index[j]]->SkinningMatrix);

			// 変換結果にウェイトを掛けたベクトルを合計する
			pos += v * ov[i].Weight[j];
		}

		// 求めた合計値を座標のベクトルとする
		av[i].Pos = (D3DXVECTOR3)pos;

		// 法線の初期化
		D3DXVECTOR4 normal = D3DXVECTOR4(0, 0, 0, 0);

		// 頂点に影響する全てのフレームについて処理
		for (DWORD j = 0; j < MODEL_BLEND_COUNT; j++)
		{
			// フレームのスキニング行列を使って頂点を変換する
			D3DXVECTOR3& n = ov[i].Normal;
			D3DXVECTOR4 v(n.x, n.y, n.z, 0);
			D3DXVec4Transform(&v, &v, &Frame[ov[i].Index[j]]->SkinningMatrix);

			// 変換結果にウェイトを掛けたベクトルを合計する
			normal += v * ov[i].Weight[j];
		}

		// 求めた合計値を正規化したものを法線のベクトルとする
		D3DXVec4Normalize(&normal, &normal);
		av[i].Normal = (D3DXVECTOR3)normal;
	}

	// 頂点バッファのアンロック
	OriginalVertexBuffer->Unlock();
	AnimatedVertexBuffer->Unlock();
}
//============================================================================
// 行列の合成
//============================================================================
void CModelFrame::Animate(D3DXMATRIXA16* parent)
{
	// 合成行列の計算
	CombinedMatrix = TransformMatrix * (*parent);

	// 全ての子フレームについて、再帰的に行列の合成処理を呼び出す
	for (size_t i = 0; i < Child.size(); i++)
	{
		Child[i]->Animate(&CombinedMatrix);
	}

	//スキニング行列の計算
	SkinningMatrix = OffsetMatrix * CombinedMatrix;
}


//============================================================================
// モデルの描画
// シェーダ用
//============================================================================
void CModel::Draw(ID3DXEffect* effect, D3DXMATRIXA16* view, D3DXMATRIXA16* proj) {

	// 頂点バッファ・インデクスバッファ・FVFの設定
	Device->SetStreamSource(0, AnimatedVertexBuffer, 0, sizeof(ANIMATED_VERTEX));
	Device->SetIndices(IndexBuffer);
	Device->SetFVF(ANIMATED_FVF);

	// ビュー射影行列の設定
	D3DXMATRIXA16 vproj = (*view)*(*proj);
	effect->SetMatrix("VProj", &vproj);

	// 全てサブセットを描画
	for (size_t i = 0; i < Subset.size(); i++)
	{
		CModelSubset* subset = Subset[i];

		// 色、テクスチャの指定
		DWORD mi = subset->MaterialIndex;
		effect->SetVector("Diffuse", &Material[mi]->Color);
		effect->SetTexture("MeshTex", Material[mi]->Texture);

		// エフェクトを適応してポリゴンを描画する
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
// モデルの描画
// 固定パイプライン用
//============================================================================
void CModel::Draw(D3DXMATRIXA16* view, D3DXMATRIXA16* proj)
{
	// 頂点バッファ・インデクスバッファ・FVFの設定
	Device->SetStreamSource(0, AnimatedVertexBuffer, 0, sizeof(ANIMATED_VERTEX));
	Device->SetIndices(IndexBuffer);
	Device->SetFVF(ANIMATED_FVF);

	// ワールド行列・ビュー行列・射影行列の設定
	D3DXMATRIXA16 world;
	D3DXMatrixIdentity(&world);
	Device->SetTransform(D3DTS_WORLD, &world);
	Device->SetTransform(D3DTS_VIEW, view);
	Device->SetTransform(D3DTS_PROJECTION, proj);

	// サブセットの描画
	for (size_t i = 0; i < Subset.size(); i++)
	{
		CModelSubset* subset = Subset[i];

		// 色の設定
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

		// テクスチャの設定
		Device->SetTexture(0, Material[mi]->Texture);

		// エフェクトを適応してポリゴンを描画する
		Device->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,				// レンダリングするプリミティブの種類
			0,								// インデックス バッファの開始地点から最初の頂点インデックスまでのオフセット
			0,								// この呼び出しの間に使用される頂点群に対する最小の頂点インデックス 
			VertexCount,					// この呼び出しで使用される頂点の数 (BaseVertexIndex + MinIndex から始まる)
			subset->FaceIndex * 3,			// インデックス配列の中で頂点の読み取りを開始する位置
			subset->FaceCount);				// レンダリングするプリミティブの数

		//Device->DrawIndexedPrimitiveUP(
		//	D3DPT_TRIANGLELIST,				// レンダリングするプリミティブの種類を記述する
		//	0,								// この呼び出しの間に使用される頂点群に対する、0(pVertexStreamZeroData の開始位置) を基準とする最小の頂点インデックス
		//	VertexCount,					// この呼び出しで使われる頂点の数 (MinVertexIndex から始まる)
		//	subset->FaceCount,				// レンダリングするプリミティブの数。使われるインデックスの数は、プリミティブ カウントの機能とプリミティブの種類である
		//	,			// インデックス データに対するユーザー メモリ ポインタ
		//	D3DFMT_INDEX16,					// インデックス データのフォーマットを記述する16or32
		//	,								// 頂点ストリーム 0 に使うための頂点データのユーザー メモリ ポインタ
		//	);								// 各頂点のデータ間のストライド


	}
}


//============================================================================
// フレーム・アニメーションなどの操作
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
