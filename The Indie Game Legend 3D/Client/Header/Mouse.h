#pragma once
#ifndef __MOUSE_H__
#include "GameObject.h"
USING(Engine)
class CMouse final : public CGameObject
{
private:
	_vector		m_vMouseDir;

private:
	explicit CMouse();
	explicit CMouse(const CMouse& _rOther);
	virtual ~CMouse() = default;

private:
	void MoveToCentorCursor();
	void UpdateMouseDir();
public:
	/*Getter*/
	_vector Get_MouseDir();
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;

public:
	static CMouse* Create();
private:
	virtual void Free() override;
};
#define __MOUSE_H__
#endif
