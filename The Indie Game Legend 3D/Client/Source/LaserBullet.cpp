#include "stdafx.h"
#include "..\Header\LaserBullet.h"
#include "Player.h"
#include "PlayerCamera.h"
#include "MsgManager.h"
#include "SoundMgr.h"

CLaserBullet::CLaserBullet()
{
}

CLaserBullet::CLaserBullet(const CLaserBullet & _rOther)
	:CBullet(_rOther)
	,m_matTest(_rOther.m_matTest)
{
}


HRESULT CLaserBullet::InitializePrototype()
{
	if (FAILED(CBullet::InitializePrototype()))
		return E_FAIL;
	D3DXMatrixIdentity(&m_matTest);
	return S_OK;
}

HRESULT CLaserBullet::Awake()
{
	CBullet::Awake();

	m_sName = L"PlayerBullet";
	m_eRenderID = ERenderID::AlphaCullNone;
	return S_OK;
}

HRESULT CLaserBullet::Start()
{
	m_pTransform->Set_Scale(_vector(6,2, 2));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Bullet"));
	SafeAddRef(m_pTexturePool);
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("LaserBullet"))[0]);
	m_fMoveSpeed = 100.f;

	//아직 충돌 이외의 삭제 처리가 필요 함
	m_fLive = 5.f;
	return S_OK;
}

UINT CLaserBullet::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;
	CMsgManager::GetInstance()->Freeze(&_fDeltaTime);


	m_pTransform->Add_Rotation(_vector( 360 * _fDeltaTime,0,0));
	m_pTransform->Add_Position(m_vDiraction* m_fMoveSpeed * _fDeltaTime);
	return OBJ_NOENVET;
}

UINT CLaserBullet::LateUpdate(const float _fDeltaTime)
{
	CBullet::LateUpdate(_fDeltaTime);
	CMsgManager::GetInstance()->Freeze(&_fDeltaTime);

	m_fLivetime += _fDeltaTime;
	if (m_fLivetime >= m_fLive)
		m_bDead = true;

	return OBJ_NOENVET;
}

HRESULT CLaserBullet::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;
	IsBillboarding();
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

CGameObject * CLaserBullet::Clone()
{
	CGameObject* pIns = new CLaserBullet(*this);
	return pIns;
}

HRESULT CLaserBullet::Fire()
{
	CBullet::Fire();
	CGameObject* pPlayer = FindGameObjectOfType<CPlayer>();
	_vector vLook = ((CTransform*)pPlayer->GetComponent<CTransform>())->Get_Look();

	CPlayerCamera* pCamera = (CPlayerCamera*)FindGameObjectOfType<CPlayerCamera>();
	if (pCamera == nullptr)
		return E_FAIL;

	_matrix matView;
	D3DXMatrixIdentity(&m_matTest);
	matView = pCamera->Get_Camera().matView;

	memcpy(&m_matTest.m[0][0], &matView.m[0][0], sizeof(_vector));
	memcpy(&m_matTest.m[1][0], &matView.m[1][0], sizeof(_vector));
	memcpy(&m_matTest.m[2][0], &matView.m[2][0], sizeof(_vector));
	D3DXMatrixInverse(&m_matTest, 0, &m_matTest);

	m_pTransform->Add_Position(m_vDiraction* 3);
	m_pTransform->Set_Rotation(_vector(0, 80, -50));
	return S_OK;
}

void CLaserBullet::OnCollision(CGameObject * _pGameObject)
{
	//벽추가
	if (L"Obstacle" == _pGameObject->GetName())
	{
		//CBulletSpark* pEffect = nullptr;
		//pEffect = (CBulletSpark*)AddGameObject<CBulletSpark>();
		//pEffect->SetPosition(m_pTransform->Get_Position());
		m_bDead = true;
	}
}

void CLaserBullet::Free()
{
	CBullet::Free();
}

HRESULT CLaserBullet::IsBillboarding()
{
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

	_matrix NewWorld = matScale * matRX * matRY * matRZ * m_matTest * matTrans;

	m_pTransform->Set_WorldMatrix(NewWorld);
	return S_OK;
}

CLaserBullet * CLaserBullet::Create()
{
	CLaserBullet* pIns = new CLaserBullet();
	return pIns;
}
