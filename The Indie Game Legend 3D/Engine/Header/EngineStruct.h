#pragma once
#ifndef __ENGINE_STRUCT_H__

typedef struct tagCameraDesc
{
	_matrix	matView;
	_vector vEye;
	_vector vAt;
	_vector vUp;

	_matrix matProj;
	float	fFovY;	// Degree
	float	fAspect;
	float	fNear;
	float	fFar;
}CAMERA_DESC;

typedef struct tagVertexTexture
{
	D3DXVECTOR3 vPosition;
	D3DXVECTOR2 vUV; 
}VTX_TEXTURE;

const _uint FVF_VTX_TEXTURE = D3DFVF_XYZ | D3DFVF_TEX1 ;

typedef struct tagVertexCubeTexture
{
	D3DXVECTOR3 vPosition;
	D3DXVECTOR3 vUV;
}VTXCUBE_TEXTURE;

const _uint FVF_VTXCUBE_TEXTURE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

typedef struct tagIndex16
{
	WORD _1, _2, _3;
}INDEX16, *LPINDEX16;;

typedef struct tagIndex32
{
	DWORD _1, _2, _3;
}INDEX32;

typedef struct tagTransformDesc
{
	_vector	vPosition;
	_vector	vScale;
	_vector	vRotation; // For.Degree
	_vector vRevolution;
	_vector vParent;
	_matrix matWorld;
}TRANSFORM_DESC;

typedef struct tagVertexRect
{
	D3DXVECTOR3 Position;
	D3DXVECTOR2 UV;
	static const DWORD FVF;
}VERTEXRECT, *LPVERTEXRECT;
typedef struct tagPivot
{
	float fX;
	float fY;
}PIVOT, *LPPIVOT;

#define __ENGINE_STRUCT_H__
#endif // !__ENGINE_STRUCT_H__
