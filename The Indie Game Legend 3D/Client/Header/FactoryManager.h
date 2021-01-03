#pragma once
#ifndef __FACTORY_MANGER_H__
#include "Base.h"
USING(Engine)
class CFactoryManager : public CBase
{
	DECLARE_SINGLETON(CFactoryManager)
private:
	vector<_vector>		m_vecSaveDataPosition;
	vector<TSTRING>		m_vecSaveDataType;
	vector<_int>		m_vecSaveDataRoomID;
public:
	/*������ ���� �а� ������ ����*/
	HRESULT LoadDataFile(const TSTRING& _sFileName);
	/*������ �����͸� ��ȸ�ϸ鼭 ����*/
	HRESULT LoadScene( CScene* const _pScene);

	HRESULT LoadCollider(CScene* _pScene, const TSTRING& _sFileName);
	HRESULT LoadInterationObj(CScene* _pScene, const TSTRING& _sFileName);
private:
	void ClearData();
	HRESULT AddObject(CScene* const _pScene, _uint _nIndex);
private:
	explicit CFactoryManager();
	virtual ~CFactoryManager() = default;

	virtual void Free() override;
};
#define __FACTORY_MANGER_H__
#endif
