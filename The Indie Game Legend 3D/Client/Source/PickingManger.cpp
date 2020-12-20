#include "stdafx.h"
#include "PickingManger.h"
#include "MsgManager.h"
#include "PlayerCamera.h"
void CPickingManger::CrossHairPicking(_uint _nSceneID)
{
	_vector vCenter = CMsgManager::GetInstance()->Get_ClientCenterVector();
	RECT tClientRC = CMsgManager::GetInstance()->GetClientRc();
	_uint nWinCX = tClientRC.right - tClientRC.left;
	_uint nWinCY = tClientRC.bottom- tClientRC.top;

	_vector vPickPt = vZero;
	/* 뷰포트 -> 투영 스페이스 */
	vPickPt.x = vCenter.x / (nWinCX * 0.5f) - 1.f;
	vPickPt.y = 1.f - vCenter.y / (nWinCY * 0.5f);
	vPickPt.z = 1.f; // 우리가 픽킹한 지점은 근평면이다.

	CPlayerCamera* pCamaer = (CPlayerCamera*)CManagement::GetInstance()->FindGameObjectOfType<CPlayerCamera>(_nSceneID);
	_matrix matInvProj = pCamaer->Get_Camera().matProj;
	D3DXMatrixInverse(&matInvProj, 0, &matInvProj);
	D3DXVec3TransformCoord(&vPickPt, &vPickPt, &matInvProj);

	//Ray setting
	_vector vRayPivot = vZero;
	_vector vRayDirection = vPickPt - vRayPivot;
	D3DXVec3Normalize(&vRayDirection, &vRayDirection);

	//list<CGameObject*> list
}
