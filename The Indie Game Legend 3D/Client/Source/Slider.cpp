#include "stdafx.h"
#include "..\Header\Slider.h"
#include "TexturePoolManager.h"
#include "Camera.h"



CSlider::CSlider()
{
}

CSlider::CSlider(const CSlider & _rOther)
	:CGameObject(_rOther)
{
}


void CSlider::Move(const float _fDeltaTime)
{
	//Test
	//나중에 툴로 배치신넘들 z x 방향 지정하는거 이거 리스트 가져와서 룸아이디로 하는걸로 
	_vector vDir = _vector(1,0,0); 
	vDir *= m_fMoveSpeed * (m_bReturn ? _fDeltaTime : -_fDeltaTime);
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
	return S_OK;
}

HRESULT CSlider::Awake()
{
	CGameObject::Awake();
	return S_OK;
}

HRESULT CSlider::Start()
{
	m_fMoveSpeed = 40.f;

	m_pTransform->Set_Scale(_vector(3, 3, 3));
	//Test
	m_pTransform->Set_Position(_vector(1, 2, 3));

	m_vSavePos = m_pTransform->Get_Position();

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));
	m_eRenderID = ERenderID::Alpha;

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Object"));
	SafeAddRef(m_pTexturePool);
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Slider"))[0]);
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

	//Test
	if (m_vSavePos.x -26.f > m_pTransform->Get_Position().x)
		m_bReturn = true;
	else if(m_vSavePos.x + 26.f < m_pTransform->Get_Position().x)
		m_bReturn = false;

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

CSlider * CSlider::Create()
{
	CSlider* pInstance = new CSlider();
	return pInstance;
}

void CSlider::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
