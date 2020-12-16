#pragma once
#ifndef __RECT_TRANSFORM_H__

#include "Transform.h"
BEGIN(Engine)
class CRectTransform final : public CTransform
{
	typedef struct tagPivot
	{
		float fX;
		float fY;
	}PIVOT, *LPPIVOT;
private:
	float m_fWidth;
	float m_fHeight;
	PIVOT m_tPivot;
private:
	explicit CRectTransform(CGameObject* const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);
	explicit CRectTransform(const CRectTransform& _rOther);
	virtual ~CRectTransform() = default;
};
END
#define __RECT_TRANSFORM_H__
#endif // !__RECT_TRANSFORM_H__
