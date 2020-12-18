#pragma once
#ifndef __ELECTRICTILE_H__
#include "Tile.h"
USING(Engine)
class CElectricTile final : public CTile
{
private:
	float	m_fTextureIndex = 0.f;
	float	m_fAnimateSpeed = 0.f;
	float	m_fAnimateOneCycleTime = 0.f; // �ѹ� ��ȸ�ϴµ� ���ʷ� �����Ұų�

	_uint   m_nMaxFrame= 0;

private:
	explicit CElectricTile();
	explicit CElectricTile(const CElectricTile& _rOther);
	virtual ~CElectricTile() = default;
private:
	HRESULT Animate(const float _fDeltaTime);
public:
	// CTile��(��) ���� ��ӵ�
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;
public:
	static  CElectricTile* Create();
private:
	virtual void Free();
};
#define __ELECTRICTILE_H__
#endif // 
