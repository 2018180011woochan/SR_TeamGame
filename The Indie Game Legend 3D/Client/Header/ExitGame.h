#pragma once
#ifndef __EXIT_GAME_H__
#include "GameObject.h"
USING(Engine)
class CExitGame final : public CGameObject
{
private:
	CText* m_pText;
private:
	explicit CExitGame();
	explicit CExitGame(const CExitGame& _rOther);
	virtual ~CExitGame() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CExitGame* Create();
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
};
#define __EXIT_GAME_H__
#endif // !__START_GAME_H__
