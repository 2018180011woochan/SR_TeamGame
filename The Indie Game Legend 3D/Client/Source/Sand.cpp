#include "stdafx.h"
#include "..\Header\Sand.h"
#include "TexturePoolManager.h"
#include "WormPart.h"
#include "Camera.h"

CSand::CSand()
	: m_pMeshRenderer(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_nIndex(0)
	, m_nMaxFrame(0)
	, m_fTime(0.f)
	, m_fAnimationSpeed(0.1f)
{
}

CSand::CSand(const CSand & _rOther)
	: CGameObject(_rOther)
	, m_pMeshRenderer(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_nIndex(0)
	, m_nMaxFrame(0)
	, m_fTime(0.f)
	, m_fAnimationSpeed(0.1f)
{
}

void CSand::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}

CSand * CSand::Create()
{
	CSand* pInstance = new CSand;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CSand::Clone()
{
	CSand* pClone = new CSand(*this);
	return pClone;
}

HRESULT CSand::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CSand::Awake()
{
	CGameObject::Awake();
	m_pTransform->Set_Scale(D3DXVECTOR3(8.f, 8.f, 5.f));

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CSand::Start()
{
	CGameObject::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("SandBurst"));
	SafeAddRef(m_pTexturePool);

	m_sTextureKey = TEXT("Idle");
	m_nMaxFrame = m_pTexturePool->GetTexture(m_sTextureKey).size();

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(m_sTextureKey)[m_nIndex]);
	m_bRemove = false;
	return S_OK;
}

UINT CSand::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	if (true == m_bRemove)
		return OBJ_DEAD;
	OnRender();
	OffRender();
	UpdatePrevPos();
	Animate(_fDeltaTime);
	Billboard();
	return 0;
}

UINT CSand::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CSand::Render()
{
	CGameObject::Render();
	if (false == m_bRender)
		return S_OK;
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CSand::SetSand(Direction _eDir, CWormPart * _pWorm, CWormPart * _pTail)
{
	m_eDirection = _eDir;
	m_pWorm = _pWorm;
	m_pWormTail = _pTail;
}

void CSand::Remove()
{
	m_bRemove = true;
}

void CSand::OnRender()
{
	if (true == m_bRender)
		return;
	D3DXVECTOR3 vWormPos;
	m_pWorm->GetWorldPos(vWormPos);

	if (vWormPos.y * m_vWormPrev.y < 0.f)
	{
		m_bRender = true;
		D3DXVECTOR3 vPosition = vWormPos;
		vPosition.y = 4.f;
		m_pTransform->Set_Position(vPosition);
		m_pTransform->UpdateTransform();
		if(m_eDirection == UP)
		cout << "On" << endl;
	}
}

void CSand::OffRender()
{
	if (false == m_bRender)
		return;
	D3DXVECTOR3 vTailPos;
	m_pWormTail->GetWorldPos(vTailPos);
	switch (m_eDirection)
	{
	case CSand::UP:
		if (vTailPos.y > 0.f)
		{
			m_bRender = false;
			cout << "Off" << endl;

		}
		break;
	case CSand::DOWN:
		if (vTailPos.y < 0.f)
			m_bRender = false;
		break;
	default:
		break;
	}
}

void CSand::UpdatePrevPos()
{
	m_pWorm->GetWorldPos(m_vWormPrev);
}

void CSand::Billboard()
{
	CCamera* pCamera = (CCamera*)FindGameObjectOfType<CCamera>();
	if (pCamera == nullptr)
		return ;

	_matrix matBillY, matView;
	D3DXMatrixIdentity(&matBillY);
	matView = pCamera->Get_Camera().matView;

	matBillY._11 = matView._11;
	matBillY._13 = matView._13;
	matBillY._31 = matView._31;
	matBillY._33 = matView._33;

	D3DXMatrixInverse(&matBillY, 0, &matBillY);

	_matrix matWorld = m_pTransform->Get_WorldMatrix();

	D3DXVECTOR3 vPos = m_pTransform->Get_Position();
	D3DXVECTOR3 vScale = m_pTransform->Get_TransformDesc().vScale;
	D3DXVECTOR3 vRot = m_pTransform->Get_TransformDesc().vRotation;

	D3DXMATRIX matScale, matRX, matRY, matRZ, matTrans;

	D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
	D3DXMatrixRotationX(&matRX, D3DXToRadian(vRot.x));
	D3DXMatrixRotationY(&matRY, D3DXToRadian(vRot.y));
	D3DXMatrixRotationZ(&matRZ, D3DXToRadian(vRot.z));
	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);



	_matrix NewWorld = matScale * matRX * matRY * matRZ * matBillY * matTrans;
	m_pTransform->Set_WorldMatrix(NewWorld);
}

void CSand::Animate(const float _fDeltaTime)
{
	m_fTime += _fDeltaTime;

	if (m_fTime >= m_fAnimationSpeed)
	{
		m_fTime -= m_fAnimationSpeed;

		++m_nIndex;

		if (m_nIndex >= m_nMaxFrame)
			m_nIndex = 0;

		m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(m_sTextureKey)[m_nIndex]);
	}
}

