#pragma once
#ifndef __WORM_BODY_H__
#include "WormPart.h"
USING(Engine)
class CWormBody final : public CWormPart
{
public:
	enum TYPE { BODY, CONNECTOR, TAIL };
private:
	TYPE	m_eType;
private:
	explicit CWormBody();
	explicit CWormBody(const CWormBody& _rOther);
	virtual ~CWormBody() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CWormBody* Create();
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

public:
	void SetTextureKey();
};
#define __WORM_BODY_H__
#endif // !__WORM_BODY_H__
