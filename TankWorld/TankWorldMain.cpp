#include <d3d9.h>
#include <d3dx9.h>
#include "ZFLog.h"
#include "ZCamera.h"
#include "SAFE_DELETE.h"
#include <vector>
#include <chrono>
using namespace std;

#define MAINBODY

#define WINDOW_W		500
#define WINDOW_H		500
#define WINDOW_TITLE	"HeightMap-TList"
#define BMP_HEIGHTMAP	"map128.bmp"

chrono::system_clock::time_point m_LastTime;
float		m_fElapseTime;

struct CUSTOMVERTEX
{
	D3DXVECTOR3	p;
	D3DXVECTOR3	n;
	D3DXVECTOR3	t;
};

struct CUSTOMVERTEX2
{
	FLOAT x, y, z;
	FLOAT u, v;
};

struct CUSTOMVERTEX3
{
	FLOAT x, y, z;
};

HWND						g_hWnd = NULL;
LPDIRECT3D9					g_pD3D = NULL;
LPDIRECT3DDEVICE9			g_pd3dDevice = NULL;

LPDIRECT3DVERTEXBUFFER9		g_pVB = NULL;
LPDIRECT3DVERTEXBUFFER9		g_pVB2 = NULL;
LPDIRECT3DVERTEXBUFFER9		g_pVB3 = NULL;

LPDIRECT3DINDEXBUFFER9		g_pIB = NULL;
LPDIRECT3DINDEXBUFFER9		g_pIB2 = NULL;
LPDIRECT3DINDEXBUFFER9		g_pIB3 = NULL;

LPDIRECT3DTEXTURE9			g_pTexHeight = NULL;
LPDIRECT3DTEXTURE9			g_pTexDiffuse = NULL;
LPDIRECT3DTEXTURE9			g_pTexDiffuse2 = NULL;

D3DXMATRIXA16				g_matAni;
D3DXMATRIXA16				g_matAni2;
D3DXMATRIXA16				g_matTankBox;
D3DXMATRIXA16				g_matTankBoxRotate;

DWORD						g_cxHeight = 0;
DWORD						g_czHeight = 0;
DWORD						g_dwMouseX = 0;
DWORD						g_dwMouseY = 0;

CUSTOMVERTEX3 CheckVertex = { 0,100.0f,-50.0f };
CUSTOMVERTEX3 CheckVertex2 = { 0,80.0f,-55.0f };
vector<CUSTOMVERTEX3> mapVertex;

void GetHegiht()
{
	int x = CheckVertex.x + 64;
	int z = -1*CheckVertex.z + 64;

	int c = 

	CheckVertex.y = (mapVertex[z*g_cxHeight + x].y + mapVertex[z*g_cxHeight + x - 1].y + mapVertex[z*g_cxHeight + x + 1].y)/3.0f;
}

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)
#define D3DFVF_CUSTOMVERTEX2 (D3DFVF_XYZ | D3DFVF_TEX1)

struct MYINDEX
{
	WORD _0, _1, _2;
};

HRESULT InitD3D(HWND hWnd)
{
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
		return E_FAIL;

	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	return S_OK;
}

HRESULT InitTexture()
{
	if (FAILED(D3DXCreateTextureFromFileEx(g_pd3dDevice, BMP_HEIGHTMAP, D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL,
		&g_pTexHeight))) //D3DFMT_X8R8G8B8 이거설명 중 jpg png(손실없는압축?)
		return E_FAIL;

	if (FAILED(D3DXCreateTextureFromFile(g_pd3dDevice, "tile2.tga", &g_pTexDiffuse)))
		return E_FAIL;

	if (FAILED(D3DXCreateTextureFromFile(g_pd3dDevice, "test.bmp", &g_pTexDiffuse2)))
		return E_FAIL;

	return S_OK;
}

HRESULT InitVB()
{
	CUSTOMVERTEX3 vertices3[] =
	{
		{-0.5f, 0.5f, 0.5f},
		{  0.5f, 0.5f, 0.5f},
		{  0.5f, 0.5f, -0.5f},
		{ -0.5f, 0.5f, -0.5f},

		{ -0.5f, -0.5f, 0.5f},
		{ 0.5f, -0.5f, 0.5f },
		{ 0.5f, -0.5f, -0.5f},
		{ -0.5f, -0.5f, -0.5f },
	};

	if (FAILED(g_pd3dDevice->CreateVertexBuffer(8 * sizeof(CUSTOMVERTEX3), 0, D3DFVF_CUSTOMVERTEX2,
		D3DPOOL_DEFAULT, &g_pVB3, NULL)))
		return E_FAIL;

	VOID* pVertices3;
	if (FAILED(g_pVB3->Lock(0, sizeof(vertices3), (void**)&pVertices3, 0)))
		return E_FAIL;
	memcpy(pVertices3, vertices3, sizeof(vertices3));
	g_pVB3->Unlock();


	CUSTOMVERTEX2 vertices[] =
	{
		{ -5, 50,0, 0,0,},
		{  5,20,0, 1,1, },
		{ -5,20,0, 0,1, },

		{ -5, 50,0, 0,0,},
		{  5, 50,0, 1,0, },
		{  5,20,0, 1,1, },
	};

	if (FAILED(g_pd3dDevice->CreateVertexBuffer(6 * sizeof(CUSTOMVERTEX2), 0, D3DFVF_CUSTOMVERTEX2, D3DPOOL_DEFAULT,
		&g_pVB2, NULL)))
		return E_FAIL;

	VOID* pVertices2;
	if (FAILED(g_pVB2->Lock(0, sizeof(vertices), (void**)&pVertices2, 0)))
		return E_FAIL;
	memcpy(pVertices2, vertices, sizeof(vertices));
	g_pVB2->Unlock();

	D3DSURFACE_DESC ddsd;
	D3DLOCKED_RECT	d3drc;

	g_pTexHeight->GetLevelDesc(0, &ddsd); //GetLevelDesc Desc가 들어가는건 이옵션에 대한 설명을 기술함->이함수는 실제로 디바이스의 정보를 알 수 있음 
	g_cxHeight = ddsd.Width;
	g_czHeight = ddsd.Height;
	ZFLog::GetInstance()->Log("Texturel Size:[%d,%d]", g_cxHeight, g_czHeight);
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(ddsd.Width*ddsd.Height * sizeof(CUSTOMVERTEX), 0,
		D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL)))
		return E_FAIL;

	g_pTexHeight->LockRect(0, &d3drc, NULL, D3DLOCK_READONLY);
	VOID* pVertices;
	if (FAILED(g_pVB->Lock(0, g_cxHeight * g_czHeight * sizeof(CUSTOMVERTEX), (void**)&pVertices, 0)))
		return E_FAIL;

	CUSTOMVERTEX v;

	CUSTOMVERTEX* pV = (CUSTOMVERTEX*)pVertices;
	for (DWORD z = 0; z < g_czHeight; z++)
	{
		for (DWORD x = 0; x < g_cxHeight; x++)
		{
			v.p.x = (float)x - g_cxHeight / 2.0f;
			v.p.z = -((float)z - g_czHeight / 2.0f);
			v.p.y = ((float)(*((LPDWORD)d3drc.pBits + x + z * (d3drc.Pitch / 4)) & 0x0000ff)) / 10.0f; // Pitch 로 접근해야함 실제로 데이터적으로 정의된값 height넣으면 큰일남
			v.n.x = v.p.x;
			v.n.y = v.p.y;
			v.n.z = v.p.z;
			D3DXVec3Normalize(&v.n, &v.n);
			v.t.x = (float)x / (g_cxHeight - 1);
			v.t.y = (float)z / (g_czHeight - 1);
			*pV++ = v;
			
			CUSTOMVERTEX3 saveVertex;
			saveVertex.x = v.p.x;
			saveVertex.y = v.p.y;
			saveVertex.z = v.p.z;
			mapVertex.push_back(saveVertex);


			
		}
	}
	/*for(auto iter = mapVertex.begin(); iter != mapVertex.end(); iter++)
		ZFLog::GetInstance()->Log("[%f ,%f ,%f]", (*iter).x, (*iter).y, (*iter).z);*/

	g_pVB->Unlock();
	g_pTexHeight->UnlockRect(0);

	return S_OK;
}

HRESULT InitIB()
{
	MYINDEX indices3[] =
	{
		{ 0,1,2 },{ 0,2,3 },
		{ 4,6,5 },{ 4,7,6 },
		{ 0,3,7 },{ 0,7,4 },
		{ 1,5,6 },{ 1,6,2 },
		{ 3,2,6 },{ 3,6,7 },
		{ 0,4,5 },{ 0,5,1 },
	};

	if (FAILED(g_pd3dDevice->CreateIndexBuffer(12 * sizeof(MYINDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT,
		&g_pIB3, NULL)))
		return E_FAIL;

	VOID* pIndeces3;
	if (FAILED(g_pIB3->Lock(0, sizeof(indices3), (void**)&pIndeces3, 0)))
		return E_FAIL;
	memcpy(pIndeces3, indices3, sizeof(indices3));
	g_pIB3->Unlock();


	MYINDEX indeices[] =
	{
		{3,2,6},{3,6,7},//앞면
	};

	if (FAILED(g_pd3dDevice->CreateIndexBuffer(2 * sizeof(MYINDEX), 0,
		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB2, NULL)))
	{
		return E_FAIL;
	}


	void* pIndices;
	if (FAILED(g_pIB2->Lock(0, sizeof(indeices), (void**)&pIndices, 0)))
		return E_FAIL;
	memcpy(pIndices, indeices, sizeof(indeices));
	g_pIB2->Unlock();


	if (FAILED(g_pd3dDevice->CreateIndexBuffer((g_cxHeight - 1) * (g_czHeight - 1) * 2 * sizeof(MYINDEX), 0,
		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB, NULL)))
		return E_FAIL;

	MYINDEX		i;
	MYINDEX*	pI;
	if (FAILED(g_pIB->Lock(0, (g_cxHeight - 1) * (g_czHeight - 1) * 2 * sizeof(MYINDEX), (void**)&pI, 0)))
		return E_FAIL;

	for (DWORD z = 0; z < g_czHeight - 1; z++)
	{
		for (DWORD x = 0; x < g_cxHeight - 1; x++)
		{
			i._0 = (z*g_cxHeight + x);
			i._1 = (z*g_cxHeight + x + 1);
			i._2 = ((z + 1)*g_cxHeight + x);
			*pI++ = i;

			i._0 = ((z + 1)*g_cxHeight + x);
			i._1 = (z*g_cxHeight + x + 1);
			i._2 = ((z + 1)*g_cxHeight + x + 1);
			*pI++ = i;
		}
	}

	g_pIB->Unlock();
	return S_OK;
}

void SetupCamera()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);


	/*D3DXVECTOR3 vEyePt(0.0f, 12.0f, -30.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 10.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);*/

	D3DXVECTOR3 vEyePt(0.0f, 100.0f, -(float)g_czHeight);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);

	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 1000.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	ZCamera::GetInstance()->SetView(&vEyePt, &vLookatPt, &vUpVec);
}

HRESULT InitGeometry()
{
	if (FAILED(InitTexture()))
		return E_FAIL;

	if (FAILED(InitVB()))
		return E_FAIL;

	if (FAILED(InitIB()))
		return E_FAIL;

	SetupCamera();
	// 최초의 마우스 위치 보관
	POINT	pt;
	GetCursorPos(&pt);
	g_dwMouseX = pt.x;
	g_dwMouseY = pt.y;

	return S_OK;
}

void ProcessMouse()
{
	POINT pt;
	float fDelta = 0.001f;
	GetCursorPos(&pt);
	int dx = pt.x - g_dwMouseX;
	int dy = pt.y - g_dwMouseY;

	ZCamera::GetInstance()->RotateLocalX(dy * fDelta);
	ZCamera::GetInstance()->RotateLocalY(dx * fDelta);
	D3DXMATRIXA16* pmatView = ZCamera::GetInstance()->GetViewMatrix();


	D3DXMatrixRotationY(&g_matTankBoxRotate, dy * fDelta);


	g_pd3dDevice->SetTransform(D3DTS_VIEW, pmatView);

	RECT	rc;
	GetClientRect(g_hWnd, &rc);
	pt.x = (rc.right - rc.left) / 2;
	pt.y = (rc.bottom - rc.top) / 2;
	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
	g_dwMouseX = pt.x;
	g_dwMouseY = pt.y;
}

void ProcessKey()
{
	std::chrono::duration<float> sec = std::chrono::system_clock::now() - m_LastTime;
	m_fElapseTime = sec.count();
	m_LastTime = std::chrono::system_clock::now();

	if (GetAsyncKeyState('W'))
	{
		ZCamera::GetInstance()->MoveLocalZ(m_fElapseTime * 5.0f);

		float x = CheckVertex2.x;
		float z = CheckVertex2.z;

		CheckVertex2.x = ZCamera::GetInstance()->GetEye()->x;
		CheckVertex2.z = ZCamera::GetInstance()->GetEye()->z;

		CheckVertex.x += CheckVertex2.x - x;
		CheckVertex.z += CheckVertex2.z - z;

	}

	if (GetAsyncKeyState('S'))
	{
		ZCamera::GetInstance()->MoveLocalZ(m_fElapseTime * -5.0f);

		float x = CheckVertex2.x;
		float z = CheckVertex2.z;

		CheckVertex2.x = ZCamera::GetInstance()->GetEye()->x;
		CheckVertex2.z = ZCamera::GetInstance()->GetEye()->z;

		CheckVertex.x += CheckVertex2.x - x;
		CheckVertex.z += CheckVertex2.z - z;

	}

	if (GetAsyncKeyState(VK_ESCAPE))
		PostMessage(g_hWnd, WM_DESTROY, 0, 0);

	if (GetAsyncKeyState(VK_LBUTTON))
		g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	if (GetAsyncKeyState(VK_RBUTTON))
		g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);


	//wasd
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		CheckVertex.z += m_fElapseTime * 3;
		ZFLog::GetInstance()->Log("TANK POSITION:[%f,%f,%f]", CheckVertex.x, CheckVertex.y, CheckVertex.z);
	}
	else if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		CheckVertex.z -= m_fElapseTime * 3;
		ZFLog::GetInstance()->Log("TANK POSITION:[%f,%f,%f]", CheckVertex.x, CheckVertex.y, CheckVertex.z);
	}
	else if (GetKeyState(VK_UP) & 0x8000)
	{
		CheckVertex.x += m_fElapseTime * 3;
		ZFLog::GetInstance()->Log("TANK POSITION:[%f,%f,%f]", CheckVertex.x, CheckVertex.y, CheckVertex.z);
	}
	else if (GetKeyState(VK_DOWN) & 0x8000)
	{
		CheckVertex.x -= m_fElapseTime * 3;
		ZFLog::GetInstance()->Log("TANK POSITION:[%f,%f,%f]", CheckVertex.x, CheckVertex.y, CheckVertex.z);
	}
}

void ProcessInputs(void)
{
	ProcessMouse();
	ProcessKey();
}

VOID SetupLights()
{
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	g_pd3dDevice->SetMaterial(&mtrl);

	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 0.0f;
	//vecDir = D3DXVECTOR3(1, 1, 1);
	vecDir = D3DXVECTOR3(cosf(GetTickCount() / 350.0f), 1.0f, sinf(GetTickCount() / 350.0f));
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
	light.Range = 1000.0f;

	g_pd3dDevice->SetLight(0, &light);
	g_pd3dDevice->LightEnable(0, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00909090);
}

void LogFPS(void)
{
	static DWORD nTick = 0;
	static DWORD nFPS = 0;

	if (GetTickCount() - nTick > 1000)
	{
		nTick = GetTickCount();
		ZFLog::GetInstance()->Log("FPS : %d", nFPS);
		nFPS = 0;

		return;
	}

	nFPS++;
}

VOID Animate()
{
	/// 0 ~ 2PI 까지(0~360도) 값을 변화시킴 Fixed Point기법 사용
	DWORD d = GetTickCount() % ((int)((D3DX_PI * 2) * 1000));
	/// Y축 회전행렬
	//D3DXMatrixRotationY( &g_matAni, d / 1000.0f );
	D3DXMatrixIdentity(&g_matAni);

	D3DXMatrixIdentity(&g_matAni2);
	g_matAni2._11 = ZCamera::GetInstance()->GetBillMatrix()->_11;
	g_matAni2._13 = ZCamera::GetInstance()->GetBillMatrix()->_13;
	g_matAni2._31 = ZCamera::GetInstance()->GetBillMatrix()->_31;
	g_matAni2._33 = ZCamera::GetInstance()->GetBillMatrix()->_33;

	//D3DXMatrixIdentity(&g_matTankBox);


	GetHegiht();
	CheckVertex2.y = CheckVertex.y -3.0f;
	D3DXMatrixTranslation(&g_matTankBox, CheckVertex.x, CheckVertex.y + 1.0f, CheckVertex.z);
	D3DXVECTOR3 cPos = { CheckVertex2.x, CheckVertex2.y + 5.0f, CheckVertex2.z};
	ZCamera::GetInstance()->MoveTo(&cPos);

	LogFPS();

	SetupLights();
	ProcessInputs();
}

void CleanUp()
{
	SAFE_RELEASE(g_pTexHeight);
	SAFE_RELEASE(g_pTexDiffuse);
	SAFE_RELEASE(g_pIB);
	SAFE_RELEASE(g_pVB);
	SAFE_RELEASE(g_pd3dDevice);
	SAFE_RELEASE(g_pD3D);
}

void DrawMesh(D3DXMATRIXA16* pMat)
{
	g_pd3dDevice->SetTransform(D3DTS_WORLD, pMat);
	g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pd3dDevice->SetIndices(g_pIB);
	g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, g_cxHeight * g_czHeight, 0,
		(g_cxHeight - 1) * (g_czHeight - 1) * 2);
}

void DrawMesh2(D3DXMATRIXA16* pMat)
{
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &g_matAni2);
	g_pd3dDevice->SetStreamSource(0, g_pVB2, 0, sizeof(CUSTOMVERTEX2)); // 정점세팅
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX2);
	g_pd3dDevice->SetIndices(g_pIB2);
	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
}

void DrawMesh3(D3DXMATRIXA16* pMat)
{
	g_pd3dDevice->SetTransform(D3DTS_WORLD, pMat);
	g_pd3dDevice->SetStreamSource(0, g_pVB3, 0, sizeof(CUSTOMVERTEX3));
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX2);
	g_pd3dDevice->SetIndices(g_pIB3);
	g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
}

void Render()
{
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	//g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	Animate();

	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		g_pd3dDevice->SetTexture(0, g_pTexDiffuse2);
		g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		DrawMesh2(&g_matAni2);
		
		g_pd3dDevice->SetTexture(0, g_pTexDiffuse);
		g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		DrawMesh(&g_matAni);

		g_pd3dDevice->SetTexture(0, NULL);
		D3DXMATRIXA16 matWorld = g_matTankBoxRotate * g_matTankBox;
		DrawMesh3(&matWorld);
		g_pd3dDevice->EndScene();
	}

	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{

	switch (iMessage)
	{
	case WM_DESTROY:
		CleanUp();
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostMessage(hWnd, WM_DESTROY, 0, 0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
char g_szClassName[256] = "BasicFrame";

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	MSG Message;
	WNDCLASS WndClass;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = g_szClassName;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	g_hWnd = CreateWindow(g_szClassName, g_szClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);

	if (FAILED(InitD3D(g_hWnd)))
		return -1;

	if (FAILED(InitGeometry()))
		return -1;

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	while (true)
	{
		/// 메시지큐에 메시지가 있으면 메시지 처리
		if (PeekMessage(&Message, NULL, 0U, 0U, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
				break;

			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else
		{
			Render();
		}
	}

	ZFLog::Release();

	UnregisterClass(g_szClassName, hInstance);

	return (int)Message.wParam;
}