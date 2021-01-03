#include "stdafx.h"
#include "..\Header\Slider.h"
#include "TexturePoolManager.h"
#include "Camera.h"



CSlider::CSlider()
{
}

CSlider::CSlider(const CSlider & _rOther)
	:CGameObject(_rOther)
	, m_vDir(_rOther.m_vDir)
	, m_fMoveSpeed(_rOther.m_fMoveSpeed)
{
}


void CSlider::Move(const float _fDeltaTime)
{
	//Test
	//나중에 툴로 배치신넘들 z x 방향 지정하는거 이거 리스트 가져와서 룸아이디로 하는걸로 
	
	_vector vDir =m_vDir * m_fMoveSpeed * (m_bReverse ? _fDeltaTime : -_fDeltaTime);
	m_pTransform->Add_Position(vDir);
}

void CSlider::IsBillboarding()
{
	CCamera* pCamera = (CCamera*)FindGameObjectOfType<CCamera>();
	if (pCamera == nullptr)
		return ;

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

HRESULT CSlider::InitializePrototype()
{
	m_vDir = _vector(1, 0, 0);
	m_fMoveSpeed = 40.f;
	m_sName = L"Slider";
	return S_OK;
}

HRESULT CSlider::Awake()
{
	CGameObject::Awake();
	m_pTransform->Set_Scale(_vector(3, 3, 3));
	m_pTransform->Add_Position(_vector(0, 4, 0));
	m_fMoveSpeed = 50.f;
	return S_OK;
}

HRESULT CSlider::Start()
{
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));
	m_eRenderID = ERenderID::Alpha;
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Object"));
	SafeAddRef(m_pTexturePool);
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Slider"))[0]);

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Cube"), BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->m_bIsRigid = true;

	return S_OK;
}

UINT CSlider::Update(const float _fDeltaTime)
{
	Move(_fDeltaTime);
	m_pTransform->UpdateTransform();
	return OBJ_NOENVET;
}

UINT CSlider::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);

	return OBJ_NOENVET;
}

HRESULT CSlider::Render()
{
	IsBillboarding();
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

CGameObject * CSlider::Clone()
{
	CSlider* pInstance = new CSlider(*this);
	return pInstance;
}

void CSlider::OnCollision(CGameObject * _pGameObject)
{
	//벽체크 
	if (L"Wall" == _pGameObject->GetName())
	{
		m_bReverse = !m_bReverse;
	}
}

void CSlider::SetDirVertical()
{
	//툴에서 배치시 세로 무브는 이거 호출로 세팅함
	m_vDir = _vector(0, 0, 1);
}

CSlider * CSlider::Create()
{
	CSlider* pInstance = new CSlider();
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return pInstance;
	}
	return pInstance;
}

void CSlider::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
