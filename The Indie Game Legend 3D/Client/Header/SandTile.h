#pragma once
#ifndef __SANDTILE_H__
#include "Tile.h"
USING(Engine)
class CSandTile : public CTile
{
private:
	float	m_fTextureIndex = 0.f;
	float	m_fAnimateSpeed = 0.f;
	float	m_fAnimateOneCycleTime = 0.f; // 한번 순회하는데 몇초로 설정할거냐
	_uint   m_nMaxFrame = 0;
private:
	HRESULT Animate(const float _fDeltaTime);
	private:
	explicit CSandTile();
	explicit CSandTile(const CSandTile& _rOther);
	virtual ~CSandTile() = default;

	// CTile을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;
public:
	static CSandTile* Create();
private:
	virtual void Free();
};
#define __SANDTILE_H__
#endif
