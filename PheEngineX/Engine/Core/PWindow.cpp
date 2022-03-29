#include "pch.h"
#include "PWindow.h"
#include "PInput.h"
#include "PEngine.h"

using namespace Phe;

PWindow* PWindow::Create(const std::string& Name, UINT32 Width, UINT32 Height)
{
	PWindow* window = nullptr;
#ifdef PlatformWin32
	window = new PWindowWin32(Width, Height, nullptr, Name);
#elif defined (PlatformIOS)
	window = new PWindowIOS();
#elif defined (PlatformAndroid)
	window = new PWindowAndroid();
#endif
	return window;
}

void PWindow::DestroyWindow(PWindow* Window)
{
	ReleasePtr(Window);
}

PWindow::PWindow(UINT32 Width, UINT32 Height) : PWidth(Width), PHeight(Height)
{
	PInput = Input::Create();
}

PWindow::~PWindow()
{

}


#ifdef PlatformWin32
static PWindowWin32* PWin32 = nullptr;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!PWin32)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	switch (message)
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			//			PEngine::GetSingleton().Pause();
		}
		else
		{
			//			PEngine::GetSingleton().Resume();
		}
		return 0;


	case WM_SIZE:
		//		PEngine::GetSingleton().SetScreenSize(LOWORD(lParam), HIWORD(lParam));
		if (wParam == SIZE_MINIMIZED)
		{
			//			PEngine::GetSingleton().Pause();
			//			PEngine::GetSingleton().Pminimized = true;
			//			PEngine::GetSingleton().Pmaximized = false;
		}
		else if (wParam == SIZE_MAXIMIZED)
		{
			//			PEngine::GetSingleton().Resume();
			//			PEngine::GetSingleton().Pminimized = false;
			//			PEngine::GetSingleton().Pmaximized = true;
			//			PEngine::GetSingleton().OnResize();
		}
		else if (wParam == SIZE_RESTORED)
		{
			// 			if (PEngine::GetSingleton().Pminimized)
			// 			{
			// 				PEngine::GetSingleton().Resume();
			// 				PEngine::GetSingleton().Pminimized = false;
			// 				PEngine::GetSingleton().OnResize();
			// 			}
			// 
			// 			else if (PEngine::GetSingleton().Pmaximized)
			// 			{
			// 				PEngine::GetSingleton().Resume();
			// 				PEngine::GetSingleton().Pmaximized = false;
			// 				PEngine::GetSingleton().OnResize();
			// 			}
			// 			else if (PEngine::GetSingleton().Presizing)
			// 			{
			// 
			// 			}
			// 			else
			// 			{
			// 				PEngine::GetSingleton().OnResize();
			// 			}
		}
		return 0;

	case WM_ENTERSIZEMOVE:
		// 		PEngine::GetSingleton().Pause();
		// 		PEngine::GetSingleton().Presizing = true;
		return 0;

	case WM_EXITSIZEMOVE:
		// 		PEngine::GetSingleton().Resume();
		// 		PEngine::GetSingleton().Presizing = false;
		// 		PEngine::GetSingleton().OnResize();
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_MENUCHAR:
		return MAKELRESULT(0, MNC_CLOSE);

	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;
	case WM_MOUSEMOVE:
		PEngine::GetSingleton().GetWindow()->GetInput()->OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONDOWN:
		PEngine::GetSingleton().GetWindow()->GetInput()->OnMouseDown(KeyCode::Mouse0);
		return 0;

	case WM_LBUTTONUP:
		PEngine::GetSingleton().GetWindow()->GetInput()->OnMouseUp(KeyCode::Mouse0);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

PWindowWin32::PWindowWin32(UINT32 Width, UINT32 Height, void* Handle, const std::string& Name) : PWindow(Width, Height), HWnd((HWND)Handle)
{
	//	PheEngine = std::make_unique<PEngine>();
	GenerateWindow(Name);
	//	PheEngine->Initialize(PWidth, PHeight, HWnd);
	PInput = dynamic_cast<PInputWin32*>(PInput);
	PWin32 = this;
}

PWindowWin32::~PWindowWin32()
{
	PInput = nullptr;
	PWin32 = nullptr;
}

bool PWindowWin32::Run()
{
	//	PEngine::GetSingleton().PTimer.Reset();
	MSG msg;
	bool quit = false;
	PInput->Update();
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);

		if (msg.hwnd == HWnd)
			WndProc(HWnd, msg.message, msg.wParam, msg.lParam);
		else
			DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
			quit = true;
	}
	return !quit;

	return false;
}

void PWindowWin32::GenerateWindow(const std::string& Name)
{
	HINSTANCE hInstance = GetModuleHandle(0);

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"MainWnd";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, LONG(PWidth), LONG(PHeight) };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	HWnd = CreateWindow(L"MainWnd", L"PheEngine",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, hInstance, 0);
	if (!HWnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
	}

	if (HWnd)
	{
		ShowWindow(HWnd, SW_SHOW);
		UpdateWindow(HWnd);
		SetActiveWindow(HWnd);
		SetForegroundWindow(HWnd);
	}
}
#endif