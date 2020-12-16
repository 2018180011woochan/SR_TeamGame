#pragma once
#ifndef __GIZMO_H__
#include "GameObject.h"
BEGIN(Engine)
class ENGINE_DLL CGizmo  : public CGameObject
{

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
public:
	explicit CGizmo();
	explicit CGizmo(const CGizmo& _rOther);
	virtual ~CGizmo() = default;

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() PURE;
	virtual HRESULT Awake() PURE;
	virtual HRESULT Start() PURE;
	virtual UINT Update(const float _fDeltaTime) PURE;
	virtual UINT LateUpdate(const float _fDeltaTime) PURE;
	virtual HRESULT Render() PURE;
	virtual CGameObject * Clone() PURE;
	virtual void Free() override;

};
END
#define __GIZMO_H__
#endif
