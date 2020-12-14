#include "..\Header\MeshManager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CMeshManager)

CMeshManager::CMeshManager()
{
}

void CMeshManager::Free()
{
	for (auto rPair : m_Mesh)
	{
		SafeRelease(rPair.second);
	}
	m_Mesh.clear();
}

CMesh * CMeshManager::Clone(const TSTRING & _sMeshID)
{
	auto iter = m_Mesh.find(_sMeshID);

	if (m_Mesh.end() == iter)
	{
		CMesh* pMesh = CMesh::Create(_sMeshID);

		if (nullptr == pMesh)
		{
			PrintLog(TEXT("Warning"), TEXT("Failed to load mesh from file."));
			return nullptr;
		}
		iter = (m_Mesh.emplace(_sMeshID, pMesh)).first;
	}

	return iter->second->Clone();
}
