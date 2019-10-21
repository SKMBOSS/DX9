#include <Windows.h>
#include <cmath>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("BounceBall");

struct CUSTOMVECTOR
{
	float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;
};

struct CUSTOMMATIRX
{
	float a11 = 0.0f, a12 = 0.0f, a13 = 0.0f, a14 = 0.0f;
	float a21 = 0.0f, a22 = 0.0f, a23 = 0.0f, a24 = 0.0f;
	float a31 = 0.0f, a32 = 0.0f, a33 = 0.0f, a34 = 0.0f;
	float a41 = 0.0f, a42 = 0.0f, a43 = 0.0f, a44 = 0.0f;
};

CUSTOMMATIRX MATIRX_PRODUCT_MATIRX(CUSTOMMATIRX mat1, CUSTOMMATIRX mat2)
{
	CUSTOMMATIRX result;

	result.a11 = mat1.a11 * mat2.a11 + mat1.a12 * mat2.a21 + mat1.a13 * mat2.a31 + mat1.a14 * mat2.a41;
	result.a12 = mat1.a11 * mat2.a12 + mat1.a12 * mat2.a22 + mat1.a13 * mat2.a32 + mat1.a14 * mat2.a42;
	result.a13 = mat1.a11 * mat2.a13 + mat1.a12 * mat2.a23 + mat1.a13 * mat2.a33 + mat1.a14 * mat2.a43;
	result.a14 = mat1.a11 * mat2.a14 + mat1.a12 * mat2.a24 + mat1.a13 * mat2.a34 + mat1.a14 * mat2.a44;

	result.a21 = mat1.a21 * mat2.a11 + mat1.a22 * mat2.a21 + mat1.a23 * mat2.a31 + mat1.a24 * mat2.a41;
	result.a22 = mat1.a21 * mat2.a12 + mat1.a22 * mat2.a22 + mat1.a23 * mat2.a32 + mat1.a24 * mat2.a42;
	result.a23 = mat1.a21 * mat2.a13 + mat1.a22 * mat2.a23 + mat1.a23 * mat2.a33 + mat1.a24 * mat2.a43;
	result.a24 = mat1.a21 * mat2.a14 + mat1.a22 * mat2.a24 + mat1.a23 * mat2.a34 + mat1.a24 * mat2.a44;

	result.a31 = mat1.a31 * mat2.a11 + mat1.a32 * mat2.a21 + mat1.a33 * mat2.a31 + mat1.a34 * mat2.a41;
	result.a32 = mat1.a31 * mat2.a12 + mat1.a32 * mat2.a22 + mat1.a33 * mat2.a32 + mat1.a34 * mat2.a42;
	result.a33 = mat1.a31 * mat2.a13 + mat1.a32 * mat2.a23 + mat1.a33 * mat2.a33 + mat1.a34 * mat2.a43;
	result.a34 = mat1.a31 * mat2.a14 + mat1.a32 * mat2.a24 + mat1.a33 * mat2.a34 + mat1.a34 * mat2.a44;

	result.a41 = mat1.a41 * mat2.a11 + mat1.a42 * mat2.a21 + mat1.a43 * mat2.a31 + mat1.a44 * mat2.a41;
	result.a42 = mat1.a41 * mat2.a12 + mat1.a42 * mat2.a22 + mat1.a43 * mat2.a32 + mat1.a44 * mat2.a42;
	result.a43 = mat1.a41 * mat2.a13 + mat1.a42 * mat2.a23 + mat1.a43 * mat2.a33 + mat1.a44 * mat2.a43;
	result.a44 = mat1.a41 * mat2.a14 + mat1.a42 * mat2.a24 + mat1.a43 * mat2.a34 + mat1.a44 * mat2.a44;
	return result;
}

CUSTOMVECTOR MATRIX_PRODUCT_VECTOR(CUSTOMMATIRX mat, CUSTOMVECTOR vec)
{
	CUSTOMVECTOR result;

	result.x = mat.a11 *vec.x + mat.a12 * vec.y + mat.a13 * vec.z + mat.a14 * vec.w;
	result.y = mat.a21 *vec.x + mat.a22 * vec.y + mat.a23 * vec.z + mat.a24 * vec.w;
	result.z = mat.a31 *vec.x + mat.a32 * vec.y + mat.a33 * vec.z + mat.a34 * vec.w;
	result.w = mat.a41 *vec.x + mat.a42 * vec.y + mat.a43 * vec.z + mat.a44 * vec.w;

	return result;
}

CUSTOMVECTOR MY_TRANS_VECTOR(CUSTOMVECTOR vec, float dX, float dY, float dZ)
{
	CUSTOMMATIRX transMatrix;

	transMatrix.a11 = 1;
	transMatrix.a22 = 1;
	transMatrix.a33 = 1;
	transMatrix.a44 = 1;

	transMatrix.a14 = dX;
	transMatrix.a24 = dY;
	transMatrix.a34 = dZ;

	

	return MATRIX_PRODUCT_VECTOR(transMatrix, vec);
}

CUSTOMVECTOR MY_ROTARIONX(CUSTOMVECTOR vec, float radian)
{
	CUSTOMMATIRX rotationX_Matrix;
	rotationX_Matrix.a11 = 1;
	rotationX_Matrix.a22 = cosf(radian);
	rotationX_Matrix.a33 = cosf(radian);
	rotationX_Matrix.a44 = 1;

	rotationX_Matrix.a23 = -1 * sinf(radian);
	rotationX_Matrix.a32 = sinf(radian);

	return MATRIX_PRODUCT_VECTOR(rotationX_Matrix, vec);
}

CUSTOMVECTOR MY_ROTARIONY(CUSTOMVECTOR vec, float radian)
{
	CUSTOMMATIRX rotationY_Matrix;
	rotationY_Matrix.a11 = cosf(radian);
	rotationY_Matrix.a22 = 1;
	rotationY_Matrix.a33 = cosf(radian);
	rotationY_Matrix.a44 = 1;

	rotationY_Matrix.a13 = sinf(radian);
	rotationY_Matrix.a31 = -1 * sinf(radian);

	return MATRIX_PRODUCT_VECTOR(rotationY_Matrix, vec);
}


CUSTOMVECTOR MY_ROTARIONZ(CUSTOMVECTOR vec, float radian)
{
	CUSTOMMATIRX rotationZ_Matrix;
	rotationZ_Matrix.a11 = cosf(radian);
	rotationZ_Matrix.a22 = cosf(radian);
	rotationZ_Matrix.a33 = 1;
	rotationZ_Matrix.a44 = 1;

	rotationZ_Matrix.a12 = -1 * sinf(radian);
	rotationZ_Matrix.a21 = sinf(radian);

	return MATRIX_PRODUCT_VECTOR(rotationZ_Matrix, vec);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;

}

CUSTOMVECTOR vec1 = { 0.0f, 0.0f, 0.5f, 1.0f };
CUSTOMVECTOR vec2 = { 100.0f, 100.0f, 0.5f, 1.0f };
CUSTOMVECTOR vec3 = { -100.0f, 100.0f, 0.5f, 1.0f };

float fangle = 0.0f;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	
	switch (iMessage)
	{
	case WM_CREATE: //윈도우를 생성할 때 호출되는 메세지(초기화를 하기에 좋은 위치)
		SetTimer(hWnd, 1, 10, NULL); //타이머를 등록 1번타이머를 등록, 1000 = 밀리세컨드 = 1초, NULL = 함수포인터를 넣는데 통상 널
		return 0;
	case WM_TIMER:
		InvalidateRect(hWnd, NULL, TRUE); //화면을 갱신하기위해
		vec1 = MY_ROTARIONY(vec1, 0.1f);
		vec2 = MY_ROTARIONY(vec2, 0.1f);
		vec3 = MY_ROTARIONY(vec3, 0.1f);

		return 0;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		CUSTOMVECTOR tVec1 = MY_TRANS_VECTOR(vec1, 650.1f, 100.0f, 0.0f);
		CUSTOMVECTOR tVec2 = MY_TRANS_VECTOR(vec2, 650.1f, 100.0f, 0.0f);
		CUSTOMVECTOR tVec3 = MY_TRANS_VECTOR(vec3, 650.1f, 100.0f, 0.0f);

		MoveToEx(hdc, tVec1.x, tVec1.y, NULL);
		LineTo(hdc, tVec2.x, tVec2.y);
		LineTo(hdc, tVec3.x, tVec3.y);
		LineTo(hdc, tVec1.x, tVec1.y);

		EndPaint(hWnd, &ps);
	}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}