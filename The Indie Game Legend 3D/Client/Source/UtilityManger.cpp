#include "stdafx.h"
#include "UtilityManger.h"
#include "MsgManager.h"
#include "PlayerCamera.h"

#include "Monster.h"
#include "Tile.h"
#include "Obstacle.h"
#include "Floor.h"
#include "Wall.h"
#include "Slider.h"

list<CGameObject*> CUtilityManger::m_RoomMobList;

bool CUtilityManger::CrossHairPicking(_uint _nSceneID, OUT _vector& _vPickingPos/*,const _uint _nRoomID*/)
{
	_vector vCenter = CMsgManager::GetInstance()->GetClientCenterVector();
	RECT tClientRC = CMsgManager::GetInstance()->GetClientRc();
	_uint nWinCX = tClientRC.right - tClientRC.left;
	_uint nWinCY = tClientRC.bottom- tClientRC.top;
	CPlayerCamera* pCamaer = (CPlayerCamera*)CManagement::GetInstance()->FindGameObjectOfType<CPlayerCamera>(_nSceneID);

	_vector vViewPortPt = vZero;
	/* ����Ʈ -> ���� �����̽� */
	vViewPortPt.x = vCenter.x / (nWinCX * 0.5f) - 1.f;
	vViewPortPt.y = 1.f - vCenter.y / (nWinCY * 0.5f);
	vViewPortPt.z = pCamaer->Get_Camera().fNear; //  ����� near

	/* ���� > ��  */
	_matrix matInvProj = pCamaer->Get_Camera().matProj;
	D3DXMatrixInverse(&matInvProj, 0, &matInvProj);
	D3DXVec3TransformCoord(&vViewPortPt, &vViewPortPt, &matInvProj);
	

	//Ray setting
	_vector vRayPivot = vZero;
	_vector vRayDirection = vViewPortPt - vRayPivot;
	D3DXVec3Normalize(&vRayDirection, &vRayDirection);

	/* �� > ���� */
	_matrix matInvView = pCamaer->Get_Camera().matView;
	D3DXMatrixInverse(&matInvView, 0, &matInvView);
	D3DXVec3TransformCoord(&vRayPivot, &vRayPivot, &matInvView);
	D3DXVec3TransformNormal(&vRayDirection, &vRayDirection, &matInvView);
	D3DXVec3Normalize(&vRayDirection, &vRayDirection);

	list<CGameObject*> CollisionList;
	//�浹��ų ��ü �߰� ..
	CollisionList = CManagement::GetInstance()->FindGameObjectsOfBaseType<CMonster>(_nSceneID);
	CollisionList.splice(CollisionList.end(), CManagement::GetInstance()->FindGameObjectsOfBaseType<CObstacle>(_nSceneID));
	CollisionList.splice(CollisionList.end(), CManagement::GetInstance()->FindGameObjectsOfBaseType<CTile>(_nSceneID));
	CollisionList.splice(CollisionList.end(), CManagement::GetInstance()->FindGameObjectsOfBaseType<CFloor>(_nSceneID));
	CollisionList.splice(CollisionList.end(), CManagement::GetInstance()->FindGameObjectsOfBaseType<CWall>(_nSceneID));


	CCollider* pCollider = nullptr;
	_vector vPickPos = vZero;
	_vector vResultPos = vZero;
	bool	bIsPicking = false;
	CGameObject* pGameObject = nullptr;
	float fDis = 0.f;
	float fLowDis = -1.f;

	for (auto& pGameObject : CollisionList)
	{
		if (pGameObject->IsEnable() == false)
		{
			continue;
		}
		pCollider = (CCollider*)pGameObject->GetComponent<CCollider>();
		if (pCollider->IsRayPicking(vPickPos, fDis, vRayPivot, vRayDirection))
		{
			bIsPicking = true;
			if (fLowDis < 0)
			{
				vResultPos = vPickPos;
				fLowDis = fDis;
			}
			else if(fLowDis < fDis)
			{
				vResultPos = vPickPos;
				fLowDis = fDis;
			}
		}
	}
	_vPickingPos = vResultPos;

	return bIsPicking;
}

bool CUtilityManger::AirstrikePicking(_uint _nSceneID, OUT _vector & _vPickingPos)
{
	_vector vCenter = CMsgManager::GetInstance()->GetClientCenterVector();
	RECT tClientRC = CMsgManager::GetInstance()->GetClientRc();
	_uint nWinCX = tClientRC.right - tClientRC.left;
	_uint nWinCY = tClientRC.bottom - tClientRC.top;
	CCollider* pCollider = nullptr;
	bool	bIsPicking = false;
	CGameObject* pGameObject = nullptr;
	float fDis = 0.f;
	CPlayerCamera* pCamaer = (CPlayerCamera*)CManagement::GetInstance()->FindGameObjectOfType<CPlayerCamera>(_nSceneID);

	_vector vViewPortPt = vZero;
	/* ����Ʈ -> ���� �����̽� */
	vViewPortPt.x = vCenter.x / (nWinCX * 0.5f) - 1.f;
	vViewPortPt.y = 1.f - vCenter.y / (nWinCY * 0.5f);
	vViewPortPt.z = pCamaer->Get_Camera().fNear; //  ����� near

	/* ���� > ��  */
	_matrix matInvProj = pCamaer->Get_Camera().matProj;
	D3DXMatrixInverse(&matInvProj, 0, &matInvProj);
	D3DXVec3TransformCoord(&vViewPortPt, &vViewPortPt, &matInvProj);


	//Ray setting
	_vector vRayPivot = vZero;
	_vector vRayDirection = vViewPortPt - vRayPivot;
	D3DXVec3Normalize(&vRayDirection, &vRayDirection);

	/* �� > ���� */
	_matrix matInvView = pCamaer->Get_Camera().matView;
	D3DXMatrixInverse(&matInvView, 0, &matInvView);
	D3DXVec3TransformCoord(&vRayPivot, &vRayPivot, &matInvView);
	D3DXVec3TransformNormal(&vRayDirection, &vRayDirection, &matInvView);
	D3DXVec3Normalize(&vRayDirection, &vRayDirection);

	list<CGameObject*> CollisionList;
	CollisionList = CManagement::GetInstance()->FindGameObjectsOfBaseType<CFloor>(_nSceneID);

	//�浹��ų ��ü �߰� ..
	for (auto& pGameObject : CollisionList)
	{
		if (pGameObject->IsEnable() == false)
			continue;

		pCollider = (CCollider*)pGameObject->GetComponent<CCollider>();
		if (pCollider->IsRayPicking(_vPickingPos, fDis, vRayPivot, vRayDirection))
		{
			bIsPicking = true;
		}
	}
	return bIsPicking;
}

bool CUtilityManger::AutoAim(_uint _nSceneID, OUT CGameObject*&  _pGameObject)
{
	if(m_RoomMobList.empty())
		return false;

	CPlayerCamera* pCamera = (CPlayerCamera*)CManagement::GetInstance()->FindGameObjectOfType<CPlayerCamera>(_nSceneID);
	_matrix matView = pCamera->Get_Camera().matView;
	_matrix matProj = pCamera->Get_Camera().matProj;
	cout << m_RoomMobList.size() << endl;

	//float fAimSize = 0.6f;
	float fAimSize = 1.f;

	float fMinimumZ = 1.f;
	auto Iter = m_RoomMobList.begin();

	//for (auto& pMob : m_RoomMobList)
	for (; Iter != m_RoomMobList.end(); )
	{
		//�����ؾ��ϱ� �ϴµ� ����
		if (((CMonster*)*Iter)->GetDead())
		{
			SafeRelease(*Iter);
			Iter = m_RoomMobList.erase(Iter);
			continue;
		}
		_vector vPos = ((CTransform*)(*Iter)->GetComponent<CTransform>())->Get_WorldPosition();
		D3DXVec3TransformCoord(&vPos, &vPos, &matView);
		D3DXVec3TransformCoord(&vPos, &vPos, &matProj);

		if (fabs(vPos.x) <= fAimSize && fabs(vPos.y) <= fAimSize && (vPos.z >= 0.f && vPos.z <= 1.f))
		{
			if (vPos.z < fMinimumZ)
			{
				_pGameObject = (*Iter);
				fMinimumZ = vPos.z;
			}
		}
		++Iter;
	}

	
	return nullptr != _pGameObject;
}

void CUtilityManger::ObjectCulling(_uint _nSceneID, _uint _nTag)
{
	ReleaseList();
	list<CGameObject*> CullingMobList;
	list<CGameObject*> CullingObjectList;

	//�ø� ����Ʈ : �� ,Ÿ�� , CObstacle
	CullingMobList = CManagement::GetInstance()->FindGameObjectsOfBaseType<CMonster>(_nSceneID);

	CullingObjectList = CManagement::GetInstance()->FindGameObjectsOfBaseType<CTile>(_nSceneID);
	CullingObjectList.splice(CullingObjectList.end(), CManagement::GetInstance()->FindGameObjectsOfBaseType<CObstacle>(_nSceneID));
	CullingObjectList.splice(CullingObjectList.end(), CManagement::GetInstance()->FindGameObjectsOfBaseType<CFloor>(_nSceneID));
	CullingObjectList.splice(CullingObjectList.end(), CManagement::GetInstance()->FindGameObjectsOfBaseType<CWall>(_nSceneID));

	CullingObjectList.splice(CullingObjectList.end(), CManagement::GetInstance()->FindGameObjectsOfBaseType<CSlider>(_nSceneID));

	//Object
	for (auto& pGameObject : CullingObjectList)
	{
		if (pGameObject->GetTag() != _nTag)
			pGameObject->SetEnable(false);
		else
			pGameObject->SetEnable(true);
	}

	//Mob �������� ����ҷ��� ���� ������ �����;��� 
	for (auto& pGameObject : CullingMobList)
	{
		if (pGameObject->GetTag() != _nTag)
			pGameObject->SetEnable(false);
		else
		{
			pGameObject->SetEnable(true);
			pGameObject->OnEnable();
			m_RoomMobList.emplace_back(pGameObject);
			SafeAddRef(pGameObject);
		}
	}
}

void CUtilityManger::ReleaseList()
{
	for (auto& pGameObj : m_RoomMobList)
	{
		SafeRelease(pGameObj);
	}
	CUtilityManger::m_RoomMobList.clear();
}

void CUtilityManger::Release()
{
	ReleaseList();
}
