#pragma once
#ifndef __CARDINAL_POINT_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CCardinalPoint final : public CGameObject
{
private:
	CTexturePool*	m_pTexturePool;
	TSTRING			m_sTextureKey;
	Image*			m_pImage;

	float			m_fDegree;
private:
	explicit CCardinalPoint();
	explicit CCardinalPoint(const CCardinalPoint& _rOther);
	virtual ~CCardinalPoint() = default;
	// CBase을(를) 통해 상속됨
	virtual void			Free() override;
public:
	static CCardinalPoint* Create();
	// CGameObject을(를) 통해 상속됨
	virtual CGameObject * Clone() override;
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
private:
	void SetOffset();
};
#define __CARDINAL_POINT_H__
#endif // !__CARDINAL_POINT_H__
