#include "stdafx.h"
#include "..\Header\Bullet.h"

#include "PlayerCamera.h"
#include "BulletSpawn.h"
#include "UtilityManger.h"
#include "MsgManager.h"
#include "..\..\Engine\Header\GameObject.h"

CBullet::CBullet()
{
}

CBullet::CBullet(const CBullet & _rOther)
	:CGameObject(_rOther)
	, m_fLivetime(_rOther.m_fLivetime)
	, m_fLive(_rOther.m_fLive)
	,m_fMoveSpeed(_rOther.m_fMoveSpeed)
	, m_vDiraction(_rOther.m_vDiraction)
	, m_bDead(_rOther.m_bDead)
	, m_fTextureIndex(_rOther.m_fTextureIndex)
	, m_fAnimateSpeed(_rOther.m_fAnimateSpeed)
	, m_fAnimateOneCycleTime(_rOther.m_fAnimateOneCycleTime)
	, m_nMaxTexture(_rOther.m_nMaxTexture)

{
}

HRESULT CBullet::InitializePrototype()
{
	m_pMeshRenderer = nullptr;
	m_pTexturePool = nullptr;
	m_fLivetime = 0.f;
	m_fLive = 0.f;
	m_fMoveSpeed = 0;
	m_bDead = false;
	m_vDiraction = vZero;
	m_fTextureIndex = 0.f;
	m_fAnimateSpeed = 0.f;
	m_fAnimateOneCycleTime = 0.f;
	m_nMaxTexture = 0;
	return S_OK;
}
//레이저는 부모 호출하지말고 box로 바꿔야함
HRESULT CBullet::Awake()
{
	CGameObject::Awake();

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));
	m_eRenderID = ERenderID::Alpha;

	CCollider* pCollider = (CCollider*)(AddComponent<CCollider>());
	pCollider->SetMesh(TEXT("Sphere"), BOUND::SPHERE);

	return S_OK;
}

HRESULT CBullet::Start()
{
	return S_OK;
}

UINT CBullet::Update(const float _fDeltaTime)
{
	


	return OBJ_NOENVET;
}

UINT CBullet::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);


	return OBJ_NOENVET;
}

HRESULT CBullet::Render()
{
	return S_OK;
}

HRESULT CBullet::Fire()
{
	CPlayerCamera* pCamera = (CPlayerCamera*)FindGameObjectOfType<CPlayerCamera>();
	CBulletSpawn* pSpawn = (CBulletSpawn*)FindGameObjectOfType<CBulletSpawn>();
	auto  pSpawnTrans = ((CTransform*)pSpawn->GetComponent<CTransform>());
	_vector vBulletPos = pSpawnTrans->Get_WorldPosition();
	CAMERA_DESC CameraDesc = pCamera->Get_Camera();

	_vector PickPos;
	CGameObject* pGameObj = nullptr;

	if (CMsgManager::GetInstance()->GetAutoAimEnable() && CUtilityManger::AutoAim(m_nSceneID,pGameObj))
	{
		if (nullptr != pGameObj)
		{
			auto  pMobTrans = ((CTransform*)pGameObj->GetComponent<CTransform>());
			m_vDiraction = pMobTrans->Get_WorldPosition() - pSpawnTrans->Get_WorldPosition();
			D3DXVec3Normalize(&m_vDiraction, &m_vDiraction);
			//Ui추가
		}
		else
		{
			m_vDiraction = CameraDesc.vAt - pSpawnTrans->Get_WorldPosition();
			D3DXVec3Normalize(&m_vDiraction, &m_vDiraction);
		}
	}
	else
	{
		if (CUtilityManger::CrossHairPicking(m_nSceneID, PickPos))
		{
			m_vDiraction = PickPos - pSpawnTrans->Get_WorldPosition();
			D3DXVec3Normalize(&m_vDiraction, &m_vDiraction);
		}
		else
		{
			m_vDiraction = CameraDesc.vAt - pSpawnTrans->Get_WorldPosition();
			D3DXVec3Normalize(&m_vDiraction, &m_vDiraction);
		}

	}
	m_pTransform->Set_Position(vBulletPos);
	return S_OK;
}



HRESULT CBullet::Animate(const float _fDeltaTime)
{
	m_fTextureIndex += _fDeltaTime * m_fAnimateSpeed;
	if (m_nMaxTexture < m_fTextureIndex)
	{
		m_fTextureIndex = 0.f;// 디버깅작업 생각해서 아예 0으로 초기화 
	}
	return S_OK;
}

HRESULT CBullet::IsBillboarding()
{
	CCamera* pCamera = (CCamera*)FindGameObjectOfType<CCamera>();
	if (pCamera == nullptr)
		return E_FAIL;

	_matrix matBill, matView;
	D3DXMatrixIdentity(&matBill);
	matView = pCamera->Get_Camera().matView;

	memcpy(&matBill.m[0][0], &matView.m[0][0], sizeof(_vector));
	memcpy(&matBill.m[1][0], &matView.m[1][0], sizeof(_vector));
	memcpy(&matBill.m[2][0], &matView.m[2][0], sizeof(_vector));

	D3DXMatrixInverse(&matBill, 0, &matBill);

	_matrix matWorld = m_pTransform->Get_WorldMatrix();
	_matrix NewWorld = matBill * matWorld;

	m_pTransform->Set_WorldMatrix(NewWorld);
	return S_OK;
}

void CBullet::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
