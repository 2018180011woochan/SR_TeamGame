#include "..\Header\RectTransform.h"

USING(Engine)

CRectTransform::CRectTransform(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice)
	:CTransform(_pGameObject, _pDevice)
{
}

CRectTransform::CRectTransform(const CRectTransform & _rOther)
	: CTransform(_rOther)
{
}
