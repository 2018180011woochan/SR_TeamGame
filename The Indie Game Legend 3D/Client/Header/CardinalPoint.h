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
	// CBase��(��) ���� ��ӵ�
	virtual void			Free() override;
public:
	static CCardinalPoint* Create();
	// CGameObject��(��) ���� ��ӵ�
	virtual CGameObject * Clone() override;
public:
	// CGameObject��(��) ���� ��ӵ�
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
