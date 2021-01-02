#include "stdafx.h"
#include "..\Header\LaserBullet.h"
#include "Player.h"
#include "PlayerCamera.h"
#include "SoundMgr.h"
#include "BulletSpawn.h"
#include "UtilityManger.h"

CLaserBullet::CLaserBullet()
{
}

CLaserBullet::CLaserBullet(const CLaserBullet & _rOther)
	:CBullet(_rOther)
	,m_matDest(_rOther.m_matDest)
{
}


HRESULT CLaserBullet::InitializePrototype()
{
	if (FAILED(CBullet::InitializePrototype()))
		return E_FAIL;
	D3DXMatrixIdentity(&m_matDest);
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
	m_fMoveSpeed = 10.f;

	//���� �浹 �̿��� ���� ó���� �ʿ� ��
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
	//ī�޶��� ������� �ʱⰢ�� ����
	CPlayerCamera* pCamera = (CPlayerCamera*)FindGameObjectOfType<CPlayerCamera>();
	if (pCamera == nullptr)
		return E_FAIL;
	CAMERA_DESC CameraDesc = pCamera->Get_Camera();
	// �ҷ� ������������ ��ġ ������ 
	CBulletSpawn* pSpawn = (CBulletSpawn*)FindGameObjectOfType<CBulletSpawn>(); 
	auto  pSpawnTrans = ((CTransform*)pSpawn->GetComponent<CTransform>());
	_vector vBulletPos = pSpawnTrans->Get_WorldPosition();

	_vector PickPos;
	CGameObject* pGameObj = nullptr;

	m_pTransform->Set_Position(vBulletPos);
	m_bAutoEnable = false;
	
	if (CMsgManager::GetInstance()->GetAutoAimEnable() &&CUtilityManger::AutoAim(m_nSceneID, pGameObj))
	{
		//Ui�߰�
		auto  pMobTrans = ((CTransform*)pGameObj->GetComponent<CTransform>());
		_matrix matView = pCamera->Get_Camera().matView;

		_vector vMobProj = pMobTrans->Get_WorldPosition();
		_vector vSpawnProj = pSpawnTrans->Get_WorldPosition();
		//�������� ��ȯ���� ���� ���
		m_vDiraction = vMobProj - vSpawnProj;
		D3DXVec3Normalize(&m_vDiraction, &m_vDiraction);

		//�����ʶ� �� ������ǥ�� 
		D3DXVec3TransformCoord(&vMobProj, &vMobProj, &matView);
		//D3DXVec3TransformCoord(&vMobProj, &vMobProj, &matProj);
		D3DXVec3TransformCoord(&vSpawnProj, &vSpawnProj, &matView);
		//D3DXVec3TransformCoord(&vSpawnProj, &vSpawnProj, &matProj);
		//������ǥ�� ����
//		_vector vProjDir = vMobProj - vSpawnProj;
		_vector vProjDir;
		 D3DXVec3TransformNormal(&vProjDir,&m_vDiraction,&matView);

		D3DXVec3Normalize(&vProjDir, &vProjDir);
		_vector vRight;
		D3DXVec3Cross(&vRight, &_vector(0, 1, 0), &vProjDir);

		_vector vWorldRight = _vector(1, 0, 0);
		_vector vWorldLook = _vector(0,0, 1);
		_vector	vAngleY = _vector(m_vDiraction.x, 0, m_vDiraction.z);
		_vector	vAngleZ = _vector(0, vProjDir.y, vProjDir.z);

		//if (m_pTransform->Get_Position().z > pMobTrans->Get_WorldPosition().z)
		//	vAngleZ.z *= -1;

		float fDotY = D3DXVec3Dot(&vWorldRight, &vAngleY);
		float fDotZ = D3DXVec3Dot(&vWorldLook, &vAngleZ);
		float fAngleY,fAngleZ;
		fAngleY = acosf(fDotY);
		fAngleZ = acosf(fDotZ);

		if (vAngleY.z < 0)
			fAngleY = 2 * PI - acosf(fDotY);


		m_pTransform->Add_Position(m_vDiraction * 1);
		//m_pTransform->Set_Rotation(_vector(0, D3DXToDegree(-fAngleY), D3DXToDegree(-fAngleZ)));
		m_pTransform->Set_Rotation(_vector(0, D3DXToDegree(-fAngleY),0));

	}
	else
	{
		//���信�� �̻��� ����������� �������� ũ�ν���� �������� ���� �߰��� 
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

		_matrix matView;
		D3DXMatrixIdentity(&m_matDest);
		matView = pCamera->Get_Camera().matView;

		memcpy(&m_matDest.m[0][0], &matView.m[0][0], sizeof(_vector));
		memcpy(&m_matDest.m[1][0], &matView.m[1][0], sizeof(_vector));
		memcpy(&m_matDest.m[2][0], &matView.m[2][0], sizeof(_vector));
		D3DXMatrixInverse(&m_matDest, 0, &m_matDest);


		m_pTransform->Add_Position(m_vDiraction * 3);
		m_pTransform->Set_Rotation(_vector(0, 80, -50));
	}
	
	return S_OK;
}

void CLaserBullet::OnCollision(CGameObject * _pGameObject)
{
	//���߰�
	if (L"Obstacle" == _pGameObject->GetName() || L"Floor" == _pGameObject->GetName() || L"Wall" == _pGameObject->GetName())
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

	D3DXMATRIX matScale, matRX, matRY, matRZ, matAxis, matTrans;

	D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
	D3DXMatrixRotationX(&matRX, D3DXToRadian(vRot.x));
	D3DXMatrixRotationY(&matRY, D3DXToRadian(vRot.y));
	D3DXMatrixRotationZ(&matRZ, D3DXToRadian(vRot.z));


	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);
	_matrix NewWorld;

	NewWorld = matScale * matRX * matRY * matRZ * m_matDest * matTrans;

	m_pTransform->Set_WorldMatrix(NewWorld);
	return S_OK;
}

CLaserBullet * CLaserBullet::Create()
{
	CLaserBullet* pIns = new CLaserBullet();
	return pIns;
}
