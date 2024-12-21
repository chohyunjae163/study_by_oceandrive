/*
 * water going into a jar~
 * all the computations done in CPU
 * 
 *
*/

#include <windows.h>

const char WINDOW_CLASS_NAME[]  = "Fluid Pixel";

//A callback function, which you define in your application, that processes messages sent to a window
LRESULT CALLBACK WndProc(
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
	wc.lpfnWndProc   = WndProc;
	wc.hInstance     = hInstance;
	wc.lpszClassName = WINDOW_CLASS_NAME;

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL,
							 ("Call to RegisterClass failed!"),
							 ("Windows Desktop Guided Tour"),
							 MB_OK);

		return 1;
	}

	HWND hwnd = CreateWindow(
    WINDOW_CLASS_NAME,              // Window class
    "Learn to Program Windows",    // Window text
    WS_OVERLAPPEDWINDOW,            // Window style

    // Size and position
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

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

	return 0;
}

LRESULT CALLBACK WndProc(
	_In_ HWND   hWnd,
	_In_ UINT   message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam) {
	

	// This function ensures that every message is processed	
	return DefWindowProc(hWnd, message, wParam, lParam);
}
