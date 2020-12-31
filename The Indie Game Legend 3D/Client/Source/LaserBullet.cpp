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

	[]() {
		//D3DXMatrixIdentity(&matBill);
		//matView = pCamera->Get_Camera().matView;
		////matBill._11 = matView._11;
		////matBill._13 = matView._13;
		////matBill._31 = matView._31;
		////matBill._33 = matView._33;

		//memcpy(&matBill.m[0][0], &matView.m[0][0], sizeof(_vector));
		//memcpy(&matBill.m[1][0], &matView.m[1][0], sizeof(_vector));
		//memcpy(&matBill.m[2][0], &matView.m[2][0], sizeof(_vector));
		//D3DXMatrixInverse(&matBill, 0, &matBill);

		//D3DXVECTOR3 vPos = pMobTrans->Get_Position();
		//D3DXVECTOR3 vRot = pMobTrans->Get_TransformDesc().vRotation;
		//D3DXMATRIX  matRX, matRY, matRZ, matTrans;

		//D3DXMatrixRotationX(&matRX, D3DXToRadian(vRot.x));
		//D3DXMatrixRotationY(&matRY, D3DXToRadian(vRot.y));
		//D3DXMatrixRotationZ(&matRZ, D3DXToRadian(vRot.z));
		//D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);
		////scale 뺀 mob의 빌보드행렬 구함 
		////matMobWorld =  matRX * matRY * matRZ * matBill * matTrans;
		//matMobWorld = pMobTrans->Get_WorldMatrix();

		//D3DXMatrixIdentity(&m_matDest);
		//memcpy(&m_matDest.m[0][0], &matMobWorld.m[0][0], sizeof(_vector));
		//memcpy(&m_matDest.m[1][0], &matMobWorld.m[1][0], sizeof(_vector));
		//memcpy(&m_matDest.m[2][0], &matMobWorld.m[2][0], sizeof(_vector));
		//D3DXMatrixInverse(&m_matDest, 0, &m_matDest);

	};

	//CBullet::Fire();
	// 망할 자동 에임 때문에 각도 세팅 여기서 다해야함 

	//카메라의 빌보드로 초기각도 세팅
	CPlayerCamera* pCamera = (CPlayerCamera*)FindGameObjectOfType<CPlayerCamera>();
	if (pCamera == nullptr)
		return E_FAIL;
	CAMERA_DESC CameraDesc = pCamera->Get_Camera();
	// 불렛 스폰지점으로 위치 얻어오기 
	CBulletSpawn* pSpawn = (CBulletSpawn*)FindGameObjectOfType<CBulletSpawn>(); 
	auto  pSpawnTrans = ((CTransform*)pSpawn->GetComponent<CTransform>());
	_vector vBulletPos = pSpawnTrans->Get_WorldPosition();

	_vector PickPos;
	CGameObject* pGameObj = nullptr;

	m_pTransform->Set_Position(vBulletPos);
	m_bAutoEnable = false;
	
	if (CMsgManager::GetInstance()->GetAutoAimEnable() &&CUtilityManger::AutoAim(m_nSceneID, pGameObj))
	{
		//Ui추가
		m_bAutoEnable = true;
		auto  pMobTrans = ((CTransform*)pGameObj->GetComponent<CTransform>());
		_matrix matBill,matView ,matMobWorld;
		m_vDiraction = pMobTrans->Get_WorldPosition() - pSpawnTrans->Get_WorldPosition();
		D3DXVec3Normalize(&m_vDiraction, &m_vDiraction);
		//x/z 로 y축 각도 
		_vector vWorldRight = _vector(1, 0, 0);
		_vector vWorldLook = _vector(0, 0, 1);

		_vector	vAngleY = _vector(m_vDiraction.x, 0, m_vDiraction.z);
		_vector	vAngleZ = _vector(m_vDiraction.x, m_vDiraction.y, m_vDiraction.z);

		float fDot = D3DXVec3Dot(&vWorldRight, &vAngleY);
		float fAngleY,fAngleZ;
		if (vAngleY.z < 0)
			fAngleY = 2 * PI - acosf(fDot);
		else
			fAngleY = acosf(fDot);

		fDot = D3DXVec3Dot(&vWorldLook, &vAngleZ);

		//if (vAngleZ.y  < 0)
		//	m_fAxisAngle = 2 * PI- acosf(fDot);
		//else
			m_fAxisAngle = acosf(fDot);

		cout << D3DXToDegree(fAngleY) << endl;
		cout << D3DXToDegree(m_fAxisAngle) << endl;

		cout << m_vDiraction.x << ", " << m_vDiraction.y << ", " << m_vDiraction.z << endl;
		m_pTransform->Add_Position(m_vDiraction * 1);
		m_pTransform->Set_Rotation(_vector(0, D3DXToDegree(-fAngleY), D3DXToDegree(0)));

	}
	else
	{
		//오토에임 미사용시 빌보드상태의 각도에서 크로스헤어 방향으로 각도 추가함 
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
	//벽추가
	if (L"Obstacle" == _pGameObject->GetName() || L"Floor" == _pGameObject->GetName())
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

	//if (m_bAutoEnable)
	//{
	//	_vector vBulletRight; 
	//	D3DXVec3Cross(&vBulletRight, &_vector(0, 1, 0), &m_vDiraction);
	//	D3DXMatrixRotationAxis(&matAxis, &_vector(0, 0, 1), m_fAxisAngle);
	//	NewWorld = matScale * matRX * matRY * matRZ * matAxis * matTrans;
	//}
	//else
		NewWorld = matScale * matRX * matRY * matRZ * m_matDest * matTrans;

	m_pTransform->Set_WorldMatrix(NewWorld);
	return S_OK;
}

CLaserBullet * CLaserBullet::Create()
{
	CLaserBullet* pIns = new CLaserBullet();
	return pIns;
}
