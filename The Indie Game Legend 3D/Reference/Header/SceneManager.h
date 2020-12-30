#pragma once
#ifndef __SCENEMANAGER_H__
#include "Base.h"

BEGIN(Engine)
class CScene;
class CSceneManager final : public CBase
{
	DECLARE_SINGLETON(CSceneManager)
private:
	CScene* m_pCurrentScene = nullptr;
private:
	explicit CSceneManager();
	virtual ~CSceneManager() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;

public:
	HRESULT SetUpCurrentScene(CScene* const _pCurrentScene);
	UINT	Update(const float _fDeltaTime);
	UINT	LateUpdate(const float _fDeltaTime);

	bool	IsCurrentSceneActive();
};
END
#define __SCENEMANAGER_H__
#endif // !__SCENEMANAGER_H__
