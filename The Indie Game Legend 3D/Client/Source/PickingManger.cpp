#include "stdafx.h"
#include "PickingManger.h"
#include "MsgManager.h"
#include "PlayerCamera.h"

#include "Monster.h"
bool CPickingManger::CrossHairPicking(_uint _nSceneID, OUT _vector& _vPickingPos/*,const _uint _nRoomID*/)
{
	_vector vCenter = CMsgManager::GetInstance()->Get_ClientCenterVector();
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
	//merge�� ��ĥ�� ���� ���ĵ� �Ѵٰ� �ϴµ� �Ҿ��� 
	CollisionList = CManagement::GetInstance()->FindGameObjectsOfBaseType<CMonster>(_nSceneID);
	CCollider* pCollider = nullptr;

	_vector vPickPos = vZero;
	_vector vResultPos = vZero;
	bool	bIsPicking = false;
	CGameObject* pGameObject = nullptr;
	float fDis = 0.f;
	float fLowDis = -1.f;
	for (auto& pGameObject : CollisionList)
	{
		pCollider = (CCollider*)pGameObject->GetComponent<CCollider>();
		if (pCollider->IsRayPicking(vPickPos, fDis, vRayPivot, vRayDirection))
		{
			bIsPicking = true;
			if (fLowDis < 0)
			{
				vResultPos = vPickPos;
				fLowDis = fDis;
				pGameObject = pGameObject;
			}
			else if(fLowDis < fDis)
			{
				pGameObject = pGameObject;
				vResultPos = vPickPos;
				fLowDis = fDis;
			}
		}
	}
	_vPickingPos = vResultPos;

	return bIsPicking;
}

void CPickingManger::ObjectCulling(_uint _nSceneID, _uint _nTag)
{
	list<CGameObject*> CullingList;
	//�ø� ����Ʈ �߰��Ұ�
	CullingList = CManagement::GetInstance()->FindGameObjectsOfBaseType<CMonster>(_nSceneID);

	for (auto& pGameObject : CullingList)
	{
		if (pGameObject->GetTage() != _nTag)
			pGameObject->SetEnable(false);
		else
			pGameObject->SetEnable(true);
	}
}
