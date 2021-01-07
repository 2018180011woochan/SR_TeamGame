#include "stdafx.h"
#include "..\Header\Ending.h"

#include "IntroBackGround.h"
#include "Star.h"
#include "EndingText.h"

CEnding::CEnding()
	: CScene(GetTypeHashCode<CEnding>())
{
}

void CEnding::Free()
{
	CScene::Free();
}

CEnding * CEnding::Create()
{
	CEnding* pInstance = new CEnding;
	return pInstance;
}

HRESULT CEnding::Awake()
{
	AddPrototype(CIntroBackGround::Create());
	AddPrototype(CStar::Create());
	AddPrototype(CEndingText::Create());
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


	AddGameObject<CEndingText>();
	CScene::Awake();
	return S_OK;
}

HRESULT CEnding::Start()
{
	CScene::Start();
	return S_OK;
}

UINT CEnding::Update(const float _fDeltaTime)
{
	CScene::Update(_fDeltaTime);
	return 0;
}

UINT CEnding::LateUpdate(const float _fDeltaTime)
{
	CScene::LateUpdate(_fDeltaTime);
	return 0;
}

