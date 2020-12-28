#include "stdafx.h"
#include "..\Header\Intro.h"
#include "IntroBackGround.h"
#include "Star.h"
#include "Logo.h"
#include "Stage.h"


#include "GemText.h"

#include "SoundMgr.h"

CIntro::CIntro()
	:CScene(GetTypeHashCode<CIntro>())
{
}

void CIntro::Free()
{
	CScene::Free();
}

CIntro * CIntro::Create()
{
	CIntro* pInstance = new CIntro;
	return pInstance;
}

HRESULT CIntro::Awake()
{
	AddPrototype(CIntroBackGround::Create());
	AddPrototype(CStar::Create());
	AddPrototype(CLogo::Create());

	CGameObject* pGameObject = nullptr;

	AddGameObject<CIntroBackGround>();
	pGameObject = AddGameObject<CStar>();
	((CTransform*)pGameObject->GetComponent<CTransform>())->Set_Position(D3DXVECTOR3(-552.f, 262.f, 0.f));
	pGameObject = AddGameObject<CStar>();
	((CTransform*)pGameObject->GetComponent<CTransform>())->Set_Position(D3DXVECTOR3(-392.f, 211.f, 0.f));
	pGameObject = AddGameObject<CStar>();
	((CTransform*)pGameObject->GetComponent<CTransform>())->Set_Position(D3DXVECTOR3(-150.f, 307.f, 0.f));
	pGameObject = AddGameObject<CStar>();
	((CTransform*)pGameObject->GetComponent<CTransform>())->Set_Position(D3DXVECTOR3(170.f, 248.f, 0.f));
	pGameObject = AddGameObject<CStar>();
	((CTransform*)pGameObject->GetComponent<CTransform>())->Set_Position(D3DXVECTOR3(345.f, 298.f, 0.f));
	pGameObject = AddGameObject<CStar>();
	((CTransform*)pGameObject->GetComponent<CTransform>())->Set_Position(D3DXVECTOR3(555.f, 265.f, 0.f));
	pGameObject = AddGameObject<CStar>();
	((CTransform*)pGameObject->GetComponent<CTransform>())->Set_Position(D3DXVECTOR3(-635.f, 75.f, 0.f));
	pGameObject = AddGameObject<CStar>();
	((CTransform*)pGameObject->GetComponent<CTransform>())->Set_Position(D3DXVECTOR3(-305.f, 15.f, 0.f));
	pGameObject = AddGameObject<CStar>();
	((CTransform*)pGameObject->GetComponent<CTransform>())->Set_Position(D3DXVECTOR3(285.f, 10.f, 0.f));
	pGameObject = AddGameObject<CStar>();
	((CTransform*)pGameObject->GetComponent<CTransform>())->Set_Position(D3DXVECTOR3(410.f, 105.f, 0.f));
	pGameObject = AddGameObject<CStar>();
	((CTransform*)pGameObject->GetComponent<CTransform>())->Set_Position(D3DXVECTOR3(-530.f, -60.f, 0.f));
	pGameObject = AddGameObject<CStar>();
	((CTransform*)pGameObject->GetComponent<CTransform>())->Set_Position(D3DXVECTOR3(-225.f, -40.f, 0.f));
	pGameObject = AddGameObject<CStar>();
	((CTransform*)pGameObject->GetComponent<CTransform>())->Set_Position(D3DXVECTOR3(-220.f, -215.f, 0.f));
	pGameObject = AddGameObject<CStar>();
	((CTransform*)pGameObject->GetComponent<CTransform>())->Set_Position(D3DXVECTOR3(270.f, -270.f, 0.f));
	pGameObject = AddGameObject<CStar>();
	((CTransform*)pGameObject->GetComponent<CTransform>())->Set_Position(D3DXVECTOR3(345.f, -230.f, 0.f));
	pGameObject = AddGameObject<CStar>();
	((CTransform*)pGameObject->GetComponent<CTransform>())->Set_Position(D3DXVECTOR3(-390.f, -305.f, 0.f));
	pGameObject = AddGameObject<CStar>();
	((CTransform*)pGameObject->GetComponent<CTransform>())->Set_Position(D3DXVECTOR3(495.f, -320.f, 0.f));
	
	AddGameObject<CLogo>();

	AddPrototype(CGemText::Create());
	AddGameObject<CGemText>();
	CScene::Awake();
	return S_OK;
}

HRESULT CIntro::Start()
{
	CScene::Start();

	CSoundMgr::GetInstance()->Initialize();
	//CSoundMgr::GetInstance()->PlayBGM(L"Title.wav");
	return S_OK;
}

UINT CIntro::Update(const float _fDeltaTime)
{
	CScene::Update(_fDeltaTime);
	if (GetAsyncKeyState(VK_RETURN) && 0x8000)
	{
		if (FAILED(SetUpCurrentScene(CStage::Create())))
		{
			PrintLog(L"Error", L"Failed To SetUpCurrentScene");
			return E_FAIL;
		}
	}
	return 0;
}

UINT CIntro::LateUpdate(const float _fDeltaTime)
{
	CScene::LateUpdate(_fDeltaTime);
	return 0;
}
