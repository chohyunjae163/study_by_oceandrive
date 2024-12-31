/*
 * water going into a jar~
 * all the computations done in CPU
 * 
 *
*/

#include <windows.h>
#include <stdbool.h>

const char WINDOW_CLASS_NAME[]  = "Fluid Pixel";
static bool bRunning;
#define internal static;

//A callback function, which you define in your application, that processes messages sent to a window
LRESULT CALLBACK MainWindowCallback(
	_In_ HWND   hWnd, //
	_In_ UINT   message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam);

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow ) {
	
	//register a new window class

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc   = MainWindowCallback;
	wc.hInstance     = hInstance;
	wc.lpszClassName = WINDOW_CLASS_NAME;

	if (!RegisterClass(&wc))
	{
		MessageBoxA(NULL,
							 ("Call to RegisterClass failed!"),
							 ("Windows Desktop Guided Tour"),
							 MB_OK);

		return 1;
	}

	HWND hwnd = CreateWindowA(
    WINDOW_CLASS_NAME,              // Window class
    "Learn to Program Windows",    // Window text
    WS_OVERLAPPEDWINDOW,            // Window style

    // Size and position
    CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,

    NULL,       // Parent window    
    NULL,       // Menu
    hInstance,  // Instance handle
    NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
    return 0;
	}

	ShowWindow(hwnd, nCmdShow);


	MSG message;
	bRunning = true;
	while (bRunning) {
		while (PeekMessageA(&message, 0, 0, 0, PM_REMOVE)) {
			if (!bRunning) {
				break;
			}
			TranslateMessage(&message);
			DispatchMessageA(&message);
		}
	}

	return 0;
}


LRESULT CALLBACK MainWindowCallback(
	_In_ HWND   hWnd,
	_In_ UINT   message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam) {
	
	LRESULT result = 0;
	switch (message) {
		case WM_DESTROY:
			bRunning = false;
			break;
		case WM_CLOSE:
			bRunning = false;
			break;
		case WM_PAINT:
		{
			PAINTSTRUCT ps; 
			HDC hdc = BeginPaint(
				hWnd,
				&ps
			);

			RECT client_rect;
			BOOL GetClientRect(
				hWnd,
				&client_rect
			);
			int win_width = client_rect.right - client_rect.left;
			int win_height = client_rect.bottom - client_rect.top;
			int bitmap_width = ps.rcPaint.right - ps.rcPaint.left;
			int bitmap_height = ps.rcPaint.bottom - ps.rcPaint.top;

			void* pBits;
			BITMAPINFO bitmap_info = { NULL };

			int StretchDIBits(
				hdc,
				0,
				0,
				win_width,
				win_height,
				0,
				0,
				bitmap_width,
				bitmap_height,
				pBits,
				&bitmap_info,
				DIB_RGB_COLORS,
				SRCCOPY
			);


			EndPaint(hwnd, &ps); 
		}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
	}

	// This function ensures that every message is processed	
	return (result);
}
