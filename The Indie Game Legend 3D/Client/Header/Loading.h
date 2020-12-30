#pragma once
#ifndef __LOADING_H__
#include "Base.h"
#include "Scene.h"
USING(Engine)
class CLoading final : public CBase
{
private:
	LPDIRECT3DDEVICE9 m_pDevice;
	HANDLE m_hThread;
	bool   m_bSceneReady;
	CScene* m_pLoadedScene;
private:
	explicit CLoading(LPDIRECT3DDEVICE9 const _pDevice);
	virtual ~CLoading() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CLoading* Create(LPDIRECT3DDEVICE9 const _pDevice);

public:
	LPDIRECT3DDEVICE9 GetDevice();
	bool IsSceneReady();
	void SetActiveScene(const bool _bActiveScene);
private:
	static unsigned __stdcall LoadingThread(void* pParam);

public:
	HRESULT OnLoading(CScene* const _pScene);
};
#define __LOADING_H__
#endif // !__LOADING_H__
