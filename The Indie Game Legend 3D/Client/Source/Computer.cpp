#include "stdafx.h"
#include "..\Header\Computer.h"
#include "Player.h"
#include "KeyManager.h"
#include "FinalStage.h"


CComputer::CComputer()
{
}

CComputer::CComputer(const CComputer & _rOther)
	:CInteractionObj(_rOther)
	, m_fInteracitonDis(0.f)
{
}

HRESULT CComputer::InitializePrototype()
{
	CInteractionObj::InitializePrototype();
	m_sName = L"Obstacle";
	m_eRenderID = ERenderID::Alpha;
	m_fInteracitonDis = 10.f;
	return S_OK;
}

HRESULT CComputer::Awake()
{
	return S_OK;
}

HRESULT CComputer::Start()
{

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("PC"));
	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(L"PC", BOUND::BOUNDTYPE::BOX);
	m_pCollider->m_bIsRigid = true;

	return S_OK;
}

UINT CComputer::Update(const float _fDeltaTime)
{
	m_pTransform->UpdateTransform();
	_vector vPlayerPos = vZero;
	FindGameObjectOfType<CPlayer>()->GetWorldPos(vPlayerPos);
	_vector vDis = m_pTransform->Get_WorldPosition() - vPlayerPos;


	if (CKeyManager::GetInstance()->Key_Down(KEY_X) && m_fInteracitonDis > D3DXVec3Length(&vDis))
	{
		m_pManagement->SetUpCurrentScene(CFinalStage::Create());
	}

	return OBJ_NOENVET;
}

UINT CComputer::LateUpdate(const float _fDeltaTime)
{
	m_pTransform->UpdateTransform();
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

HRESULT CComputer::Render()
{
	return E_NOTIMPL;
}

CGameObject * CComputer::Clone()
{
	CGameObject* pIns = new CComputer(*this);
	return pIns;
}

CComputer * CComputer::Create()
{
	CComputer* pIns = new CComputer();
	if (FAILED(pIns->InitializePrototype()))
	{
		SafeRelease(pIns);
		return nullptr;
	}
	return pIns;
}

void CComputer::Free()
{
	CInteractionObj::Free();
}

void CComputer::OnCollision(CGameObject * _pGameObject)
{
}

void CComputer::OnNotify()
{
}
