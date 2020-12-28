#pragma once
#ifndef __MANAGEMENT_H__
#include "Base.h"
#include "GraphicDevice.h"
#include "GameObjectManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "TimeManager.h"
#include "EngineComponent.h"
#include "FrameManager.h"
BEGIN(Engine)
class ENGINE_DLL CManagement final : public CBase
{
	DECLARE_SINGLETON(CManagement)
private:

	CGraphicDevice*			m_pGraphicDevice;
	CGameObjectManager*		m_pGameObjectManager;
	CSceneManager*			m_pSceneManager;
	CRenderer*				m_pRenderer;
	CTimeManager*			m_pTimeManager;
	CFrameManager*			m_pFrameManager;
	HWND					m_hWnd;
private:
	explicit CManagement();
	virtual ~CManagement() = default;
public:
	HRESULT Initialize(const HWND _hWnd, const UINT _nWinCX, const UINT _nWinCY, const EDisplayMode _eDisplayMode,const float _fSetFrame);
	UINT	Update();
	HRESULT	Render(HWND _hWnd);
	float	Running();

	// CBase을(를) 통해 상속됨
	virtual void Free() override;
	void ReleaseEngine();
#pragma region GAMEOBJECT_MANAGER
	HRESULT				AwakeGameObject(const size_t _nSceneID);
	HRESULT				StartGameObject(const size_t _nSceneID);
	UINT				UpdateGameObject(const size_t _nSceneID, const float _fDeltaTime);
	UINT				LateUpdateGameObject(const size_t _nSceneID, const float _fDeltaTime);
	HRESULT				AddInRenderer(const ERenderID _eRenderID, CGameObject* const _pGameObject);
	HRESULT				AddPrototype(CGameObject* const _pGameObject);
	template<typename T>
	CGameObject*		AddGameObject(const UINT _nSceneID);
	CGameObject*		AddGameObject(const UINT _nSceneID, const TSTRING& _sTypeName);
	template<typename T>
	CGameObject*		Instantiate(const size_t _nSceneID);
	template<typename T>
	CGameObject*		FindGameObjectOfType(const size_t _nSceneID);
	template<typename T>
	list<CGameObject*>	FindGameObjectsOfType(const size_t _nSceneID);
	template<typename T>
	list<CGameObject*>	FindGameObjectsOfBaseType(const size_t _nSceneID);
#pragma endregion

#pragma region GRAPHICDEVICE
	LPDIRECT3DDEVICE9	GetDevice();
#pragma endregion

#pragma region SCENE_MANAGER
	HRESULT SetUpCurrentScene(CScene* const _pCurrentScene);
#pragma endregion
};
END
#include "Management.hpp"
#define __MANAGEMENT_H__
#endif // !__MANAGEMENT_H__

