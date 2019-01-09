//=============================================================================
//
// モデルのアニメーション処理 [D3DXAnimation.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#include "main.h"
#include "D3DXAnimation.h"
#include "Player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void DrawMeshContainer(D3DXANIMATION* Animation, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);
void DrawFrame(D3DXANIMATION* Animation, LPD3DXFRAME pFrame);
HRESULT SetupBoneMatrixPointers(LPD3DXFRAME pFrameBase, LPD3DXFRAME pFrameRoot);
void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);
D3DXFRAME_DERIVED* SearchBoneFrame(D3DXANIMATION* Animation, const char* BoneName, D3DXFRAME* Frame);

// =============================================================================

HRESULT InitAnimation(D3DXANIMATION* Animation, LPCSTR SetName, int Set_No);
//HRESULT InitAnimation(D3DXANIMATION* Animation);
void UninitAnimation(D3DXANIMATION* Animation);
void UpdateAnimation(D3DXANIMATION* Animation, float Time);
void DrawAnimation(D3DXANIMATION* Animation, LPD3DXMATRIX WorldMatrix);
void ChangeAnimation(D3DXANIMATION* Animation, UINT AnimID, float PlaySpeed);

//*****************************************************************************
// グローバル変数
//*****************************************************************************


//--------------------------------------------------------------------------------------
// Name: SetupBoneMatrixPointers()
// Desc: ボーン行列のポインタの準備
//--------------------------------------------------------------------------------------
HRESULT SetupBoneMatrixPointers(LPD3DXFRAME pFrameBase, LPD3DXFRAME pFrameRoot)
{
	if (pFrameBase->pMeshContainer != NULL)
	{
		D3DXFRAME_DERIVED* pFrame = NULL;
		D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrameBase->pMeshContainer;

		// if there is a skinmesh, then setup the bone matrices
		if (pMeshContainer->pSkinInfo != NULL)
		{
			UINT cBones = pMeshContainer->pSkinInfo->GetNumBones();
			pMeshContainer->ppBoneMatrix = new D3DXMATRIX*[cBones];
			for (UINT iBone = 0; iBone < cBones; iBone++)
			{
				pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(iBone));
				if (pFrame == NULL) return E_FAIL;

				pMeshContainer->ppBoneMatrix[iBone] = &pFrame->CombinedTransformationMatrix;
			}
		}
	}

	if (pFrameBase->pFrameSibling != NULL)
	{
		if (FAILED(SetupBoneMatrixPointers(pFrameBase->pFrameSibling, pFrameRoot)))
			return E_FAIL;
	}

	if (pFrameBase->pFrameFirstChild != NULL)
	{
		if (FAILED(SetupBoneMatrixPointers(pFrameBase->pFrameFirstChild, pFrameRoot)))
			return E_FAIL;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Name: DrawFrame()
// Desc: フレームの描画
//--------------------------------------------------------------------------------------
void DrawFrame(D3DXANIMATION* Animation, LPD3DXFRAME pFrame)
//void DrawFrame(LPD3DXFRAME pFrame)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	LPD3DXMESHCONTAINER pMeshContainer;

	if (pFrame == NULL)
	{
		return;
	}
	pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer != NULL)
	{
		DrawMeshContainer(Animation, pMeshContainer, pFrame);
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	DrawFrame(Animation, pFrame->pFrameSibling);
	DrawFrame(Animation, pFrame->pFrameFirstChild);
}


//--------------------------------------------------------------------------------------
// Name: DrawMeshContainer()
// Desc: メッシュコンテナの描画
//--------------------------------------------------------------------------------------
void DrawMeshContainer(D3DXANIMATION* Animation, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase)
//void DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase)
{
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
	LPDIRECT3DDEVICE9 Device = GetDevice();
	UINT iMaterial;
	UINT NumBlend;
	UINT iAttrib;
	DWORD AttribIdPrev;
	LPD3DXBONECOMBINATION pBoneComb;

	UINT iMatrixIndex;
	D3DXMATRIXA16 matTemp;
	D3DCAPS9 d3dCaps;
	Device->GetDeviceCaps(&d3dCaps);

	// first check for skinning
	if (pMeshContainer->pSkinInfo != NULL)
	{
		AttribIdPrev = UNUSED32;
		pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

		// Draw using default vtx processing of the device (typically HW)
		for (iAttrib = 0; iAttrib < pMeshContainer->BoneNum; iAttrib++)
		{
			NumBlend = 0;
			for (DWORD i = 0; i < pMeshContainer->BoneWeightNum; ++i)
			{
				if (pBoneComb[iAttrib].BoneId[i] != UINT_MAX)
				{
					NumBlend = i;
				}
			}

			if (d3dCaps.MaxVertexBlendMatrices >= NumBlend + 1)
			{
				// first calculate the world matrices for the current set of blend weights and get the accurate count of the number of blends
				for (DWORD i = 0; i < pMeshContainer->BoneWeightNum; ++i)
				{
					iMatrixIndex = pBoneComb[iAttrib].BoneId[i];
					if (iMatrixIndex != UINT_MAX)
					{
						D3DXMatrixMultiply(&matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
							pMeshContainer->ppBoneMatrix[iMatrixIndex]);
						Device->SetTransform(D3DTS_WORLDMATRIX(i), &matTemp);
					}
				}

				Device->SetRenderState(D3DRS_VERTEXBLEND, NumBlend);

				// lookup the material used for this subset of faces
				if ((AttribIdPrev != pBoneComb[iAttrib].AttribId) || (AttribIdPrev == UNUSED32))
				{
					Device->SetMaterial(&pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D);
					Device->SetTexture(0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId]);
					AttribIdPrev = pBoneComb[iAttrib].AttribId;
				}

				// draw the subset now that the correct material and matrices are loaded
				pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
			}
		}
		Device->SetRenderState(D3DRS_VERTEXBLEND, 0);
	}
	else  // standard mesh, just draw it after setting material properties
	{
		Device->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix);

		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			Device->SetMaterial(&pMeshContainer->pMaterials[iMaterial].MatD3D);
			Device->SetTexture(0, pMeshContainer->ppTextures[iMaterial]);
			pMeshContainer->MeshData.pMesh->DrawSubset(iMaterial);
		}
	}
}

//--------------------------------------------------------------------------------------
// Name: UpdateFrameMatrics()
// Desc: フレーム行列の更新
//--------------------------------------------------------------------------------------
void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
	LPDIRECT3DDEVICE9 Device = GetDevice();

	if (pFrameBase == NULL || pParentMatrix == NULL)
	{
		return;
	}

	D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);

	UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
}


//---------------------------------------------------------
//Name:Load_xFile
//Desc:Xファイルの読み込み
//---------------------------------------------------------
HRESULT Load_xFile(D3DXANIMATION* D3DXAnimation, LPCTSTR filename, const char* ErrorSrc)
{
	char Message[64];
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXAnimation->AllocateHier = new AllocateHierarchy();

	if (
		FAILED(D3DXLoadMeshHierarchyFromX(filename,
			D3DXMESH_MANAGED,
			Device,
			D3DXAnimation->AllocateHier,
			NULL,
			&D3DXAnimation->FrameRoot,
			&D3DXAnimation->AnimController))
		)
	{
		sprintf_s(Message, "Load %s Model Failed！", ErrorSrc);
		MessageBox(0, Message, "Error", 0);
		return E_FAIL;
	}

	if (FAILED(SetupBoneMatrixPointers(D3DXAnimation->FrameRoot, D3DXAnimation->FrameRoot)))
	{
		return E_FAIL;
	}

	D3DXAnimation->MotionEnd = false;
	D3DXAnimation->AnimSetNum = D3DXAnimation->AnimController->GetMaxNumAnimationSets();
	D3DXAnimation->Status = (ANIMATIONSTATUS*)malloc(sizeof(ANIMATIONSTATUS) * D3DXAnimation->AnimSetNum);

	return S_OK;
}


//=============================================================================
// 特定なボーンのマトリックスを取得
//=============================================================================
D3DXMATRIX GetBoneMatrix(D3DXANIMATION* Animation, const char* BoneName)
{
	char Message[64];
	D3DXFRAME_DERIVED* pFrame = SearchBoneFrame(Animation, BoneName, Animation->FrameRoot);

	// ボーンが見つかれば
	if (pFrame != NULL && pFrame->Name != NULL && strcmp(pFrame->Name, BoneName) == 0)
	{
		return pFrame->CombinedTransformationMatrix;
	}
	// ボーンが見つからなければ
	else
	{
		sprintf_s(Message, "Find %s Bone Failed！", BoneName);
		MessageBox(0, Message, "Error", 0);
		D3DXMATRIX TmpMatrix;
		D3DXMatrixIdentity(&TmpMatrix);
		return TmpMatrix;
	}
}

//=============================================================================
// 特定なボーンを探す
//=============================================================================
D3DXFRAME_DERIVED* SearchBoneFrame(D3DXANIMATION* Animation, const char* BoneName, D3DXFRAME* Frame)
{
	D3DXFRAME_DERIVED* pFrame = NULL;

	if (Frame == NULL)
	{
		return NULL;
	}

	if (Frame->Name != NULL && strcmp(Frame->Name, BoneName) == 0)
	{
		pFrame = (D3DXFRAME_DERIVED*)Frame;
		return pFrame;
	}

	if (Frame->pFrameSibling != NULL)
	{
		pFrame = SearchBoneFrame(Animation, BoneName, Frame->pFrameSibling);
		if (pFrame != NULL && strcmp(pFrame->Name, BoneName) == 0)
		{
			return pFrame;
		}
	}

	if (Frame->pFrameFirstChild != NULL)
	{
		pFrame = SearchBoneFrame(Animation, BoneName, Frame->pFrameFirstChild);
		if (pFrame != NULL && strcmp(pFrame->Name, BoneName) == 0)
		{
			return pFrame;
		}
	}

	return NULL;
}


//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitAnimation(D3DXANIMATION* Animation, LPCSTR SetName, int Set_No)
{
	char Message[64];

	if (FAILED(Animation->AnimController->GetAnimationSetByName(SetName, &Animation->Status[Set_No].AnimSet)))
	{
		sprintf_s(Message, "Can't find %s AnimationSet！", SetName);
		MessageBox(0, Message, "Error", 0);
		return E_FAIL;
	}

	Animation->Status[Set_No].SetName = Animation->Status[Set_No].AnimSet->GetName();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitAnimation(D3DXANIMATION* Animation)
{
	D3DXFrameDestroy(Animation->FrameRoot, Animation->AllocateHier);
	//Animation->AllocateHier->DestroyFrame(Animation->FrameRoot);
	SAFE_DELETE(Animation->Status);
	SAFE_RELEASE(Animation->AnimController);
	SAFE_DELETE(Animation->AllocateHier);
	SAFE_DELETE(Animation);

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateAnimation(D3DXANIMATION* Animation, float Time)
{
	AnimCallBackHandler CallBackHandler;
	CallBackHandler.AnimPointer = Animation;
	CallBackHandler.SetName = Animation->Status[Animation->CurrentAnimID].SetName;

	// 合成中か否かを判定
	Animation->Status[Animation->CurrentAnimID].CurWeightTime += Time;

	if (Animation->Status[Animation->CurrentAnimID].CurWeightTime <= Animation->Status[Animation->CurrentAnimID].ShiftTime)
	{
		// 合成中。ウェイトを算出
		float Weight = Animation->Status[Animation->CurrentAnimID].CurWeightTime / Animation->Status[Animation->CurrentAnimID].ShiftTime;
		// ウェイトを登録
		Animation->AnimController->SetTrackWeight(0, Weight);       // 現在のアニメーション
		Animation->AnimController->SetTrackWeight(1, 1 - Weight);   // 前のアニメーション
	}
	else
	{
		// 合成終了中。通常アニメーションをするTrack0のウェイトを最大値に
		Animation->AnimController->SetTrackWeight(0, 1.0f);			// 現在のアニメーション
		Animation->AnimController->SetTrackEnable(1, false);		// 前のアニメーションを無効にする
	}

	// 時間を更新
	//Animation->AnimController->AdvanceTime(Time, NULL);
	Animation->AnimController->AdvanceTime(Time, &CallBackHandler);

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawAnimation(D3DXANIMATION* Animation, LPD3DXMATRIX WorldMatrix)
{
	UpdateFrameMatrices(Animation->FrameRoot, WorldMatrix);

	DrawFrame(Animation, Animation->FrameRoot);

	return;
}

//=============================================================================
// アニメーションを切り替え
//=============================================================================
void ChangeAnimation(D3DXANIMATION* Animation, UINT AnimID, float PlaySpeed)
{
	D3DXTRACK_DESC TD;   // トラックの能力

	Animation->MotionEnd = false;
	Animation->StopMove = false;
	Animation->KeyframeCount = 0;

	// 指定のアニメーションIDの存在をチェック
	if (AnimID > (UINT)Animation->AnimSetNum)
	{
		MessageBox(0, "AnimationSet Don't Exist", "Error", 0);
		return;
	}

	// 異なるアニメーションであるかをチェック
	// 更新する必要がない
	if (Animation->CurrentAnimID == AnimID)
	{
		return;
	}

	// 現在のアニメーションセットの設定値を取得
	Animation->AnimController->GetTrackDesc(0, &TD);

	// 今のアニメーションをトラック1に移行し
	// トラックの設定値も移行
	Animation->AnimController->SetTrackAnimationSet(1, Animation->Status[Animation->CurrentAnimID].AnimSet);
	Animation->AnimController->SetTrackDesc(1, &TD);
	Animation->AnimController->SetTrackSpeed(1, 0.0f);

	// 新しいアニメーションセットをトラック0に設定
	Animation->AnimController->SetTrackAnimationSet(0, Animation->Status[AnimID].AnimSet);

	// トラック0のスピードの設定
	Animation->AnimController->SetTrackSpeed(0, PlaySpeed);

	// トラック0の位置は最初からに設定
	Animation->AnimController->SetTrackPosition(0, 0.0f);

	// トラックの合成を許可
	Animation->AnimController->SetTrackEnable(0, true);
	Animation->AnimController->SetTrackEnable(1, true);

	// ウェイト時間を初期化
	Animation->Status[AnimID].CurWeightTime = 0.0f;
	Animation->AnimController->ResetTime();

	// 現在のアニメーション番号を切り替え
	Animation->PreventAnimID = Animation->CurrentAnimID;
	Animation->CurrentAnimID = AnimID;

	return;
}

//=============================================================================
// 動作開始にかかる時間を設定
//=============================================================================
void SetShiftTime(D3DXANIMATION* Animation, UINT AnimID, float Interval)
{
	// 指定のアニメーションIDの存在をチェック
	if (AnimID > (UINT)Animation->AnimSetNum)
	{
		MessageBox(0, "AnimationSet Don't Exist", "Error", 0);
		return;
	}

	// シフト時間を登録
	Animation->Status[AnimID].ShiftTime = Interval;

	return;
}

//=============================================================================
// オブジェクトを作る
//=============================================================================
D3DXANIMATION* CreateAnimationObject(void)
{
	D3DXANIMATION *Object = (D3DXANIMATION*)calloc(1, sizeof(D3DXANIMATION));

	Object->InitAnimation = InitAnimation;
	Object->UninitAnimation = UninitAnimation;
	Object->UpdateAnimation = UpdateAnimation;
	Object->DrawAnimation = DrawAnimation;
	Object->ChangeAnimation = ChangeAnimation;
	Object->SetShiftTime = SetShiftTime;

	return Object;
}

//=============================================================================
// アニメ再生の中断イベント
//=============================================================================
HRESULT AnimCallBackHandler::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	PLAYER *Player = GetPlayer();

	if (AnimPointer->KeyframeCount == 0)
	{
		AnimPointer->StopMove = true;
	}
	else if (AnimPointer->KeyframeCount == 1)
	{
		AnimPointer->MotionEnd = true;
		return D3D_OK;
	}

	AnimPointer->KeyframeCount++;

	return D3D_OK;
}

//=============================================================================
// アニメーション中断イベントのKeyframesを設置する
//=============================================================================
HRESULT SetupCallbackKeyframes(D3DXANIMATION* Animation, LPCSTR SetName)
{
	// Keyframeを設置する予定のAnimationSet
	ID3DXKeyframedAnimationSet* AnimSetTemp = 0;
	// Keyframeを設置した新しいAnimationSet
	ID3DXCompressedAnimationSet* CompressedAnimSet = 0;
	// エラーメッセージ	
	char Message[64];
	// 設置するCallback Keysの数
	const UINT CallbacksNum = 2;
	D3DXKEY_CALLBACK Keys[CallbacksNum];
	//CALLBACK_DATA CallbackData;

	if (FAILED(Animation->AnimController->GetAnimationSetByName(SetName, (ID3DXAnimationSet**)&AnimSetTemp)))
	{
		sprintf_s(Message, "Setup Callback in %s AnimationSet Failed！", SetName);
		MessageBox(0, Message, "Error", 0);
		return E_FAIL;
	}

	// Compress it.
	ID3DXBuffer* CompressedInfo = 0;
	AnimSetTemp->Compress(D3DXCOMPRESS_DEFAULT, 0.5f, 0, &CompressedInfo);

	// GetSourceTicksPerSecond() returns the number of 
	// animation keyframe ticks that occur per second.
	// Callback keyframe times are tick based. 
	double ticks = AnimSetTemp->GetSourceTicksPerSecond();

	Keys[0].Time = (float)(AnimSetTemp->GetPeriod() * (90.0f / 125.0f) * ticks);
	//keys[0].pCallbackData = (void*)&CallbackData;

	// Set the second callback key to trigger callback at the end of the animation sequence.
	Keys[1].Time = (float)(AnimSetTemp->GetPeriod() * 0.9f * ticks);
	//keys[1].pCallbackData = (void*)&CallbackData;

	D3DXCreateCompressedAnimationSet(AnimSetTemp->GetName(),
		AnimSetTemp->GetSourceTicksPerSecond(),
		AnimSetTemp->GetPlaybackType(), CompressedInfo, CallbacksNum, Keys, &CompressedAnimSet);

	CompressedInfo->Release();

	// Remove the old (non-compressed) animation set.
	Animation->AnimController->UnregisterAnimationSet(AnimSetTemp);
	AnimSetTemp->Release();

	// Add the new (compressed) animation set.
	Animation->AnimController->RegisterAnimationSet(CompressedAnimSet);
	CompressedAnimSet->Release();

	return S_OK;
}
