/*
 * water going into a jar~
 * all the computations done in CPU
 * 
 *
*/

#include <windows.h>
#include <stdbool.h>
#include <wingdi.h>
#include <stdint.h>

const char WINDOW_CLASS_NAME[]  = "Fluid Pixel";
static bool bRunning;
#define internal static;

static int WIDTH = 640;
static int HEIGHT = 480;
static int PIXEL_BYTES = 4;
static int BITS_PER_PIXEL = 32;
static int BYTES_PER_PIXEL = 4;

// unsigned integers
typedef uint8_t u8;     // 1-byte long unsigned integer
typedef uint16_t u16;   // 2-byte long unsigned integer
typedef uint32_t u32;   // 4-byte long unsigned integer
typedef uint64_t u64;   // 8-byte long unsigned integer
// signed integers
typedef int8_t s8;      // 1-byte long signed integer
typedef int16_t s16;    // 2-byte long signed integer
typedef int32_t s32;    // 4-byte long signed integer
typedef int64_t s64;    // 8-byte long signed integer


void* pBits = NULL;

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
    CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT,

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


	int MEM_SIZE = (WIDTH * HEIGHT) * PIXEL_BYTES;
	pBits = VirtualAlloc(
		NULL, //  If this parameter is NULL, the system determines where to allocate the region.
		MEM_SIZE,
		MEM_COMMIT,
		PAGE_READWRITE
	);

	u32 pitch = WIDTH * BYTES_PER_PIXEL;
	u8* row = (u8*)pBits;
	for (int y = 0; y < HEIGHT; ++y) {
		u32* pixel = (u32*)row;
		for (int x = 0; x < WIDTH; ++x) {
			//all red
			*pixel = 0xFFFF0000;
			++pixel;
		}
		row += pitch;
	}

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

	bool bMemReleased = VirtualFree(
		pBits,
		0,
		MEM_RELEASE
	);



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
			GetClientRect(hWnd,&client_rect);
			int win_width = client_rect.right - client_rect.left;
			int win_height = client_rect.bottom - client_rect.top;
			int bitmap_width = ps.rcPaint.right - ps.rcPaint.left;
			int bitmap_height = ps.rcPaint.bottom - ps.rcPaint.top;

			BITMAPINFO bitmap_info = { 0 };
			BITMAPINFOHEADER bitmap_header = { 0 };
			
			bitmap_header.biSize = sizeof(bitmap_info.bmiHeader);
			bitmap_header.biWidth = WIDTH;
			bitmap_header.biHeight = HEIGHT;
			bitmap_header.biPlanes = 1;
			bitmap_header.biBitCount = BITS_PER_PIXEL;
			bitmap_header.biCompression = BI_RGB;
			bitmap_info.bmiHeader = bitmap_header;
			StretchDIBits(
				hdc,
				0,0,win_width,win_height,
				0,0,bitmap_width,bitmap_height,
				pBits,
				&bitmap_info,
				DIB_RGB_COLORS,
				SRCCOPY
			);


			EndPaint(hWnd, &ps); 
		}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
	}

	// This function ensures that every message is processed	
	return (result);
}
