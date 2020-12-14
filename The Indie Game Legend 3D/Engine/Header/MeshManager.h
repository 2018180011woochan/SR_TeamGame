#pragma once
#ifndef __MESH_MANAGER_H__
#include "Base.h"
#include "Mesh.h"
BEGIN(Engine)
class CMeshManager final : public CBase
{
	DECLARE_SINGLETON(CMeshManager)
private:
	typedef unordered_map<TSTRING, CMesh*>	MESH;
	MESH	m_Mesh;
private:
	explicit CMeshManager();
	virtual ~CMeshManager() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;

public:
	CMesh* Clone(const TSTRING& _sMeshID);
};
END
#define __MESH_MANAGER_H__
#endif // !__MESH_MANAGER_H__
