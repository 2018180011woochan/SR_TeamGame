#pragma once
#ifndef __TRANSFORM_H__
#include "Component.h"
BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
private:
	TRANSFORM_DESC	m_tTransformDesc;
public:
	/*Getter*/
	const TRANSFORM_DESC& Get_TransformDesc() const;
	const _vector Get_Right() const;
	const _vector Get_Look() const;
	const _vector Get_Up() const;
	const _vector Get_Position() const;
	const _vector Get_WorldPosition() const;

	const _vector Get_Parent() const;
	const _matrix Get_WorldMatrix() const;
	/*Setter*/
	void Set_Position(const _vector& _rPosition);
	void Add_Position(const _vector& _rPosition);
	void Set_Rotation(const _vector& _rRotaticon);
	void Add_Rotation(const _vector& _rRotaticon);
	void Add_RotationX(const float& _rRotationX);
	void Add_RotationY(const float& _rRotationY);
	void Add_RotationZ(const float& _rRotationZ);

	void Set_RotationZ(const float& _rRotationZ);

	void Set_Revolution(const _vector& _rRevolution);
	void Add_Revolution(const _vector& _rRevolution);
	void Add_RevolutionX(const float& _rRevolutionX);
	void Add_RevolutionY(const float& _rRevolutionY);
	void Add_RevolutionZ(const float& _rRevolutionZ);

	void Set_Parent(const _vector& _rParent);
	void Add_parent(const _vector& _rParent);

	void Set_WorldMatrix(const _matrix & matWorld);
	void Set_Scale(const _vector & _rScale);

public:
	void UpdateTransform();
	HRESULT UpdateWorld();
public:
	explicit CTransform(CGameObject* const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);
	explicit CTransform(const CTransform& _rOther);
	virtual ~CTransform() = default;

	// CComponent을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	static CTransform* Create(CGameObject* const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);
	virtual void Free() override;
};
END
#define __TRANSFORM_H__
#endif
