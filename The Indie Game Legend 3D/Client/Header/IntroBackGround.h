#pragma once
#ifndef __INTRO_BACKGROUND_H__
#include "GameObject.h"
USING(Engine)
class CIntroBackGround final : public CGameObject
{
private:
	Image* m_pImage;

private:
	explicit CIntroBackGround();
	explicit CIntroBackGround(const CIntroBackGround& _rOther);
	virtual ~CIntroBackGround() = default;
	virtual void Free() override;
public:
	static CIntroBackGround* Create();
	virtual CGameObject*	 Clone() override;
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;


};
#define __INTRO_BACKGROUND_H__
#endif // !__INTRO_BACKGROUND_H__
