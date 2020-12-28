#include "stdafx.h"
#include "BulletSpark.h"
#include "TexturePoolManager.h"
#include "PlayerCamera.h"


CBulletSpark::CBulletSpark()
	:m_pTexturePool(nullptr)
{
}

CBulletSpark::CBulletSpark(const CBulletSpark & _rOther)
	:CGameObject(_rOther)
	, m_pTexturePool(nullptr)
{
}


void CBulletSpark::IsBillBording()
{
	CPlayerCamera* pCamera = (CPlayerCamera*)FindGameObjectOfType<CPlayerCamera>();
	if (nullptr == pCamera)
		return;

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
}

HRESULT CBulletSpark::Animate(const float _fDeltaTime)
{
	m_fTextureIndex += _fDeltaTime * m_fAnimateSpeed;
	if (m_nMaxFrame <= m_fTextureIndex)
	{
		m_bDead = true;
		return S_OK;
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("BulletSpark"))[(_int)m_fTextureIndex]);
	return S_OK;
}

void CBulletSpark::SetPosition(const _vector & _vPos)
{
	if (nullptr == m_pTransform)
	{
		return;
	}
	m_pTransform->Set_Position(_vPos);
	m_pTransform->UpdateTransform();
}

HRESULT CBulletSpark::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBulletSpark::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return S_OK;

	m_eRenderID = ERenderID::Alpha;
	m_sName = L"Effect";
	return S_OK;
}

HRESULT CBulletSpark::Start()
{
	CGameObject::Awake();
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Effect"));
	SafeAddRef(m_pTexturePool);

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("BulletSpark"))[0]);
	m_nMaxFrame = m_pTexturePool->GetTexture(TEXT("BulletSpark")).size();

	m_fTextureIndex = 0.f;
	m_fAnimateOneCycleTime = 3.f;
	m_fAnimateSpeed = (m_nMaxFrame) / 1.f * m_fAnimateOneCycleTime;

	m_pTransform->Set_Scale(_vector(4, 4, 4));

	return S_OK;
}

UINT CBulletSpark::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;


	if (FAILED(Animate(_fDeltaTime)))
		return OBJ_NOENVET;
	m_pTransform->UpdateTransform();

	return OBJ_NOENVET;
}

UINT CBulletSpark::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	IsBillBording();
	return OBJ_NOENVET;
}

HRESULT CBulletSpark::Render()
{
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();

	return S_OK;;
}

CGameObject * CBulletSpark::Clone()
{
	CGameObject* pInstance = new CBulletSpark(*this);
	return pInstance;
}

CBulletSpark * CBulletSpark::Create()
{
	CBulletSpark* pInstance = new CBulletSpark();
	return pInstance;
}

void CBulletSpark::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}

