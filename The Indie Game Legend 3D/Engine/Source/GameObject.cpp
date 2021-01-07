#include "..\Header\GameObject.h"

USING(Engine)

CGameObject::CGameObject()
	: m_pManagement(CManagement::GetInstance())
	, m_pDevice(m_pManagement->GetDevice())
	, m_pTransform(nullptr)
	, m_nTag(-1)
	, m_eRenderID(ERenderID::End)
	, m_bEnable(true)
	, m_sName(L"")
	, m_bDelete(false)
{
	SafeAddRef(m_pDevice);
	SafeAddRef(m_pManagement);
}

CGameObject::CGameObject(const CGameObject & _rOther)
	: m_pDevice(_rOther.m_pDevice)
	, m_pManagement(_rOther.m_pManagement)
	, m_pTransform(nullptr)
	, m_nTag(_rOther.m_nTag)
	, m_eRenderID(_rOther.m_eRenderID)
	, m_bEnable(true)
	, m_bDelete(false)
	, m_sName(_rOther.m_sName)
{
	SafeAddRef(m_pDevice);
	SafeAddRef(m_pManagement);
	m_pTransform = (CTransform*)AddComponent<CTransform>();
}

void CGameObject::Free()
{
	for (auto rPair : m_Components)
	{
		SafeRelease(rPair.second);
	}
	m_Components.clear();
	SafeRelease(m_pDevice);
	SafeRelease(m_pManagement);
}

HRESULT CGameObject::InitializePrototype()
{
	return S_OK;
}

HRESULT CGameObject::Awake()
{
	return S_OK;
}

HRESULT CGameObject::Start()
{
	return S_OK;
}

UINT CGameObject::Update(const float _fDeltaTime)
{
	return 0;
}

UINT CGameObject::LateUpdate(const float _fDeltaTime)
{
	m_pManagement->AddInRenderer(m_eRenderID, this);
	return 0;
}

HRESULT CGameObject::Render()
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	//Test
	if (IsEnable() == false)
		return E_FAIL;

	return S_OK;
}

CGameObject * CGameObject::Clone()
{
	return nullptr;
}

void CGameObject::OnCollision(CGameObject * _pGameObject)
{
}

void CGameObject::OnTrigger(CGameObject * _pGameObject)
{
}

void CGameObject::SetTag(const _int & _nTag)
{
	m_nTag = _nTag;
}



_int CGameObject::GetTag()
{
	return m_nTag;
}

TSTRING CGameObject::GetName()
{
	return m_sName;
}

void CGameObject::SetDelete()
{
	m_bDelete = true;
}

bool CGameObject::IsDelete()
{
	return m_bDelete;
}

CComponent * CGameObject::GetComponent(const TSTRING & _sTypeName)
{
	auto iter = m_Components.find(_sTypeName);

	if (m_Components.end() == iter)
	{
		TCHAR szBuf[128] = TEXT("");
		_stprintf_s(szBuf, 128, TEXT("Failed to get component. %s does not exist."), _sTypeName.c_str());
		return nullptr;
	}
	return iter->second;
}

void CGameObject::SetSceneID(const UINT _nSceneID)
{
	m_nSceneID = _nSceneID;
}

bool CGameObject::IsEnable()
{
	return m_bEnable;
}

void CGameObject::OnEnable()
{
}

void CGameObject::OnDisable()
{
}

void CGameObject::SetEnable(const bool _bEnable)
{
	m_bEnable = _bEnable;

	if (true == m_bEnable)
		OnEnable();
	else
		OnDisable();
}

HRESULT CGameObject::GetWorldPos(OUT D3DXVECTOR3& _vWorldPos)
{
	if (m_pTransform != nullptr)
	{
		_vWorldPos = m_pTransform->Get_WorldPosition();
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CGameObject::SetPosition(const D3DXVECTOR3 & _vPosition)
{
	if (m_pTransform != nullptr)
	{
		 m_pTransform->Set_Position(_vPosition);
		return S_OK;
	}
	return E_FAIL;
}
