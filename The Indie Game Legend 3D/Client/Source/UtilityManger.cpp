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

#include "SlideBlock.h"
#include "CameraManager.h"
#include "InteractionObj.h"

list<CGameObject*> CUtilityManger::m_RoomMobList;

bool CUtilityManger::CrossHairPicking(_uint _nSceneID, OUT _vector& _vPickingPos/*,const _uint _nRoomID*/)
{
	_vector vCenter = CMsgManager::GetInstance()->GetClientCenterVector();
	RECT tClientRC = CMsgManager::GetInstance()->GetClientRc();
	_uint nWinCX = tClientRC.right - tClientRC.left;
	_uint nWinCY = tClientRC.bottom- tClientRC.top;
	CPlayerCamera* pCamaer = (CPlayerCamera*)CManagement::GetInstance()->FindGameObjectOfType<CPlayerCamera>(_nSceneID);

	_vector vViewPortPt = vZero;
	/* 뷰포트 -> 투영 스페이스 */
	vViewPortPt.x = vCenter.x / (nWinCX * 0.5f) - 1.f;
	vViewPortPt.y = 1.f - vCenter.y / (nWinCY * 0.5f);
	vViewPortPt.z = pCamaer->Get_Camera().fNear; //  근평면 near

	/* 투영 > 뷰  */
	_matrix matInvProj = pCamaer->Get_Camera().matProj;
	D3DXMatrixInverse(&matInvProj, 0, &matInvProj);
	D3DXVec3TransformCoord(&vViewPortPt, &vViewPortPt, &matInvProj);
	

	//Ray setting
	_vector vRayPivot = vZero;
	_vector vRayDirection = vViewPortPt - vRayPivot;
	D3DXVec3Normalize(&vRayDirection, &vRayDirection);

	/* 뷰 > 월드 */
	_matrix matInvView = pCamaer->Get_Camera().matView;
	D3DXMatrixInverse(&matInvView, 0, &matInvView);
	D3DXVec3TransformCoord(&vRayPivot, &vRayPivot, &matInvView);
	D3DXVec3TransformNormal(&vRayDirection, &vRayDirection, &matInvView);
	D3DXVec3Normalize(&vRayDirection, &vRayDirection);

	list<CGameObject*> CollisionList;
	//충돌시킬 물체 추가 ..
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
			continue;
		pCollider = (CCollider*)pGameObject->GetComponent<CCollider>();
		if(pCollider == nullptr)
			continue;
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
	/* 뷰포트 -> 투영 스페이스 */
	vViewPortPt.x = vCenter.x / (nWinCX * 0.5f) - 1.f;
	vViewPortPt.y = 1.f - vCenter.y / (nWinCY * 0.5f);
	vViewPortPt.z = pCamaer->Get_Camera().fNear; //  근평면 near

	/* 투영 > 뷰  */
	_matrix matInvProj = pCamaer->Get_Camera().matProj;
	D3DXMatrixInverse(&matInvProj, 0, &matInvProj);
	D3DXVec3TransformCoord(&vViewPortPt, &vViewPortPt, &matInvProj);


	//Ray setting
	_vector vRayPivot = vZero;
	_vector vRayDirection = vViewPortPt - vRayPivot;
	D3DXVec3Normalize(&vRayDirection, &vRayDirection);

	/* 뷰 > 월드 */
	_matrix matInvView = pCamaer->Get_Camera().matView;
	D3DXMatrixInverse(&matInvView, 0, &matInvView);
	D3DXVec3TransformCoord(&vRayPivot, &vRayPivot, &matInvView);
	D3DXVec3TransformNormal(&vRayDirection, &vRayDirection, &matInvView);
	D3DXVec3Normalize(&vRayDirection, &vRayDirection);

	list<CGameObject*> CollisionList;
	CollisionList = CManagement::GetInstance()->FindGameObjectsOfBaseType<CFloor>(_nSceneID);

	//충돌시킬 물체 추가 ..
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

bool CUtilityManger::MouseBlockPicking(_uint _nSceneID)
{
	POINT tMousePos;
	GetCursorPos(&tMousePos);
	ScreenToClient(g_hWnd, &tMousePos);

	RECT tClientRC = CMsgManager::GetInstance()->GetClientRc();
	_uint nWinCX = tClientRC.right - tClientRC.left;
	_uint nWinCY = tClientRC.bottom - tClientRC.top;
	CCollider* pCollider = nullptr;
	bool	bIsPicking = false;
	CGameObject* pGameObject = nullptr;
	float fDis = 0.f;
	//CCameraManager::GetInstance()->
	CAMERA_DESC* pCameraDecs = nullptr;
	if (FAILED(CCameraManager::GetInstance()->GetCurCameraDesc(pCameraDecs)))
		return false;
	_vector vPosition = vZero;
	_vector vViewPortPt = vZero;
	/* 뷰포트 -> 투영 스페이스 */
	vViewPortPt.x = tMousePos.x / (nWinCX * 0.5f) - 1.f;
	vViewPortPt.y = 1.f - tMousePos.y / (nWinCY * 0.5f);
	vViewPortPt.z = pCameraDecs->fNear; //  근평면 near

	/* 투영 > 뷰  */
	_matrix matInvProj = pCameraDecs->matProj;
	D3DXMatrixInverse(&matInvProj, 0, &matInvProj);
	D3DXVec3TransformCoord(&vViewPortPt, &vViewPortPt, &matInvProj);


	//Ray setting
	_vector vRayPivot = vZero;
	_vector vRayDirection = vViewPortPt - vRayPivot;
	D3DXVec3Normalize(&vRayDirection, &vRayDirection);

	/* 뷰 > 월드 */
	_matrix matInvView = pCameraDecs->matView;
	D3DXMatrixInverse(&matInvView, 0, &matInvView);
	D3DXVec3TransformCoord(&vRayPivot, &vRayPivot, &matInvView);
	D3DXVec3TransformNormal(&vRayDirection, &vRayDirection, &matInvView);
	D3DXVec3Normalize(&vRayDirection, &vRayDirection);

	list<CGameObject*> CollisionList;
	CollisionList = CManagement::GetInstance()->FindGameObjectsOfBaseType<CSlideBlock>(_nSceneID);

	for (auto& pGameObject : CollisionList)
	{
		if (pGameObject->IsEnable() == false)
			continue;

		//((CSlideBlock*)pGameObject)->SetPicking(false);
		pCollider = (CCollider*)pGameObject->GetComponent<CCollider>();
		if (pCollider->IsRayPicking(vPosition, fDis, vRayPivot, vRayDirection))
		{
			((CSlideBlock*)pGameObject)->SetPicking(true);
			bIsPicking = true;
			break;
		}
	}

	return bIsPicking;
}

bool CUtilityManger::AutoAim(_uint _nSceneID, OUT list<CGameObject*>& _listGameObj)
{
	if (m_RoomMobList.empty())
		return false;

	CPlayerCamera* pCamera = (CPlayerCamera*)CManagement::GetInstance()->FindGameObjectOfType<CPlayerCamera>(_nSceneID);
	_matrix matView = pCamera->Get_Camera().matView;
	_matrix matProj = pCamera->Get_Camera().matProj;

	float fAimSize = 1.f; // 절두체 전 범위 이긴 한데 
	float fCamerNear = pCamera->Get_Camera().fNear;
	auto IterMob = m_RoomMobList.begin();

	//엔진에서 삭제처리를 막아놨기에 여기서 삭제처리를 해야한다 
	for (; IterMob != m_RoomMobList.end(); )
	{
		if (((CMonster*)*IterMob)->GetDead() || (*IterMob)->IsDelete())
		{
			SafeRelease(*IterMob);
			IterMob = m_RoomMobList.erase(IterMob);
			continue;
		}

		_vector vPos = ((CTransform*)(*IterMob)->GetComponent<CTransform>())->Get_WorldPosition();
		_vector vProjPos;
		D3DXVec3TransformCoord(&vProjPos, &vPos, &matView);
		D3DXVec3TransformCoord(&vProjPos, &vProjPos, &matProj);

		if (fabs(vProjPos.x) <= fAimSize && fabs(vProjPos.y) 
			<= fAimSize && (vProjPos.z >= 0.f && vProjPos.z <= 1.f)
			&& vPos.z >fCamerNear)
		{
			_listGameObj.emplace_back(*IterMob);
		}
		++IterMob;
	}

	return !_listGameObj.empty();
}

bool CUtilityManger::AutoAim(_uint _nSceneID, OUT CGameObject*&  _pGameObject)
{
	if(m_RoomMobList.empty())
		return false;

	CPlayerCamera* pCamera = (CPlayerCamera*)CManagement::GetInstance()->FindGameObjectOfType<CPlayerCamera>(_nSceneID);
	_matrix matView = pCamera->Get_Camera().matView;
	_matrix matProj = pCamera->Get_Camera().matProj;

	float fAimSize = 1.f;
	float fMinimumZ = pCamera->Get_Camera().fNear;

	auto Iter = m_RoomMobList.begin();

	//for (auto& pMob : m_RoomMobList)
	for (; Iter != m_RoomMobList.end(); )
	{
		//삭제해야하긴 하는데 귀찮
		if (((CMonster*)*Iter)->GetDead() || (*Iter)->IsDelete() )
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

	//컬링 리스트 : 몹 ,타일 , CObstacle CWall CSlider
	CullingMobList = CManagement::GetInstance()->FindGameObjectsOfBaseType<CMonster>(_nSceneID);

	CullingObjectList = CManagement::GetInstance()->FindGameObjectsOfBaseType<CTile>(_nSceneID);
	CullingObjectList.splice(CullingObjectList.end(), CManagement::GetInstance()->FindGameObjectsOfBaseType<CObstacle>(_nSceneID));
	CullingObjectList.splice(CullingObjectList.end(), CManagement::GetInstance()->FindGameObjectsOfBaseType<CFloor>(_nSceneID));
	CullingObjectList.splice(CullingObjectList.end(), CManagement::GetInstance()->FindGameObjectsOfBaseType<CWall>(_nSceneID));
	CullingObjectList.splice(CullingObjectList.end(), CManagement::GetInstance()->FindGameObjectsOfBaseType<CSlider>(_nSceneID));
	CullingObjectList.splice(CullingObjectList.end(), CManagement::GetInstance()->FindGameObjectsOfBaseType<CInteractionObj>(_nSceneID));

	//Object
	for (auto& pGameObject : CullingObjectList)
	{
		if (pGameObject->GetTag() != _nTag)
			pGameObject->SetEnable(false);
		else
			pGameObject->SetEnable(true);
	}

	//Mob 유도판정 계산할려면 따로 돌려서 가져와야함 
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
