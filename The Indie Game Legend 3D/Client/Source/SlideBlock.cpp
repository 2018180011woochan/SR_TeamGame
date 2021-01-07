#include "stdafx.h"
#include "..\Header\SlideBlock.h"
#include "Mouse.h"



CSlideBlock::CSlideBlock()
{
}

CSlideBlock::CSlideBlock(const CSlideBlock & _rOther)
	:CObstacle(_rOther)
	, m_eMoveDir(eMoveDir::End)
	, m_bPick(false)
	, m_fSpeed(0.f)
{
}

HRESULT CSlideBlock::InitializePrototype()
{
	return 	CObstacle::InitializePrototype();
}

HRESULT CSlideBlock::Awake()
{
	CObstacle::Awake();

	m_fSpeed = 18.f;
#ifdef _DEBUG
	m_nTag = 0;
#endif // _DEBUG
	return S_OK;
}

HRESULT CSlideBlock::Start()
{
	CObstacle::Start();

	m_pTransform->UpdateTransform();

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Cube"), BOUND::BOUNDTYPE::BOX);
	m_pCollider->m_bIsRigid = true;
	m_pCollider->m_bIsStand = true;
	return S_OK;
}

UINT CSlideBlock::Update(const float _fDeltaTime)
{
	if (m_bPick)
	{
		_vector vMouse = ((CMouse*)FindGameObjectOfType<CMouse>())->Get_MouseDir();
		_vector vMouseDir = _vector(vMouse.x, 0, -vMouse.y);
		if (m_eMoveDir == eMoveDir::LR)
			vMouseDir.z = 0;
		else
			vMouseDir.x = 0;

		m_pTransform->Add_Position(vMouseDir * m_fSpeed * _fDeltaTime);
	}


	m_pTransform->UpdateTransform();
	m_pTransform->UpdateWorld();
	return OBJ_NOENVET;
}

UINT CSlideBlock::LateUpdate(const float _fDeltaTime)
{
	CObstacle::LateUpdate(_fDeltaTime);
	return OBJ_NOENVET;
}

HRESULT CSlideBlock::Render()
{
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}



void CSlideBlock::SetBlockMoveDir(const eMoveDir & _eID)
{
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Cube"));
	m_eRenderID = ERenderID::NoAlpha;

	m_pTransform->Set_Scale(_vector(8, 8, 8));
	m_pTransform->Add_Position(_vector(0, 4, 0));

	

	D3DMATERIAL9 temp;
	ZeroMemory(&temp, sizeof(D3DMATERIAL9));
	m_eMoveDir = _eID;
	switch (m_eMoveDir)
	{
	case eMoveDir::LR:
		temp.Ambient.a = 0;
		temp.Ambient.r = 1;
		temp.Ambient.g = 1;
		temp.Ambient.b = 1;
		temp.Diffuse.a = 1;
		temp.Diffuse.r = 0.86274510622025f;
		temp.Diffuse.g = 0.07843137532473f;
		temp.Diffuse.b = 0.1352941185236f;
		temp.Specular.a = 1;
		temp.Specular.r = 1;
		temp.Specular.g = 1;
		temp.Specular.b = 1;
		temp.Power = 7.f;
		m_pMeshRenderer->SetMaterial(0, temp);
		break;
	case eMoveDir::UD:
		temp.Ambient.a = 1;
		temp.Ambient.r = 1;
		temp.Ambient.g = 1;
		temp.Ambient.b = 1;
		temp.Diffuse.a = 1;
		temp.Diffuse.r = 0;
		temp.Diffuse.g = 0;
		temp.Diffuse.b = 0.80392158031464f;
		temp.Specular.a = 1;
		temp.Specular.r = 1;
		temp.Specular.g = 1;
		temp.Specular.b = 1;
		temp.Power = 7.f;
		m_pMeshRenderer->SetMaterial(0, temp);
		break;
	default:
		break;
	}
}
void CSlideBlock::SetPicking(const bool & _bPick)
{
	m_bPick = _bPick;
	m_pCollider->m_bIsStand = !m_bPick;


	switch (m_eMoveDir)
	{
	case eMoveDir::UD:
		cout << "UD : "  << (m_pCollider->m_bIsStand ? "TRUE " : "false") << endl;
		break;
	//case eMoveDir::LR:
	//	//cout << "LR : " << (m_pCollider->m_bIsStand ? "TRUE " : "false") << endl;
	}
}
void CSlideBlock::SetSize(float _x, float _y)
{
	m_pTransform->Set_Scale(_vector(8.f *_x, 8, 8 * _y));
}
void CSlideBlock::OnCollision(CGameObject * _pGameObject)
{
	if (L"PlayerBullet" == _pGameObject->GetName())
	{
	}
}
CGameObject * CSlideBlock::Clone()
{
	CGameObject* pInstance = new CSlideBlock(*this);

	return pInstance;
}



CSlideBlock * CSlideBlock::Create()
{
	CSlideBlock* pInstance = new CSlideBlock();
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSlideBlock::Free()
{
	CObstacle::Free();
}
