/*
 * water going into a jar~
 * all the computations done in CPU
 * 
 *
*/

#include <windows.h>
#include <stdbool.h>
#include <stdio.h>
#include <wingdi.h>
#include <stdint.h>

const char WINDOW_CLASS_NAME[]  = "Fluid Pixel";
static bool bRunning;
#define internal static;
#define global_variable static;

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

typedef float f32;
typedef double f64;

typedef struct 
{
	BITMAPINFO Info;
	void *Memory;
	int Width;
	int Height;
	int BytesPerPixel;
} win32_offscreen_buffer;

global_variable win32_offscreen_buffer GlobalBackBuffer;



void* pBits = NULL;

//A callback function, which you define in your application, that processes messages sent to a window
LRESULT CALLBACK MainWindowCallback(
	_In_ HWND   hWnd, //
	_In_ UINT   message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam);

void
Win32DisplayBufferInWindow(win32_offscreen_buffer *Buffer,
													 HDC DeviceContext, RECT ClientRect) {
	int WindowWidth = ClientRect.right - ClientRect.left;
	int WindowHeight = ClientRect.bottom - ClientRect.top;

	StretchDIBits(DeviceContext,
								0, 0, WindowWidth, WindowHeight,
								0, 0, Buffer->Width, Buffer->Height,
								Buffer->Memory,
								&Buffer->Info,
								DIB_RGB_COLORS, SRCCOPY);
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow ) {
	
	
	LARGE_INTEGER PerfCountFrequencyResult;
	//QueryPerformanceFrequency tells us how many increments 
	//does the clock go through in one second. 
	QueryPerformanceFrequency(&PerfCountFrequencyResult);
	s64 PerfCountFrequency = PerfCountFrequencyResult.QuadPart;

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

	HWND hWnd = CreateWindowA(
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

	if (hWnd == NULL)
	{
    return 0;
	}

	ShowWindow(hWnd, nCmdShow);


	int MEM_SIZE = (WIDTH * HEIGHT) * PIXEL_BYTES;

	RECT ClientRect;
	GetClientRect(hWnd,&ClientRect);
	int Width = ClientRect.right - ClientRect.left;
	int Height = ClientRect.bottom - ClientRect.top;

	BITMAPINFO bitmap_info = { 0 };
	BITMAPINFOHEADER bitmap_header = { 0 };
			
	bitmap_header.biSize = sizeof(bitmap_info.bmiHeader);
	bitmap_header.biWidth = WIDTH;
	bitmap_header.biHeight = HEIGHT;
	bitmap_header.biPlanes = 1;
	bitmap_header.biBitCount = BITS_PER_PIXEL;
	bitmap_header.biCompression = BI_RGB;
	bitmap_info.bmiHeader = bitmap_header;

	pBits = VirtualAlloc(
		NULL, //  If this parameter is NULL, the system determines where to allocate the region.
		MEM_SIZE,
		MEM_RESERVE | MEM_COMMIT,
		PAGE_READWRITE
	);

	GlobalBackBuffer.Info = bitmap_info;
	GlobalBackBuffer.Memory = pBits;
	GlobalBackBuffer.Width = Width;
	GlobalBackBuffer.Height = Height;
	GlobalBackBuffer.BytesPerPixel = 4;

	u32 pitch = WIDTH * BYTES_PER_PIXEL;
	u8* row = (u8*)pBits;
	for (int y = 0; y < HEIGHT; ++y) {
		u32* pixel = (u32*)row;
		for (int x = 0; x < WIDTH; ++x) {
			*pixel = 0x00000000;
			
			++pixel;
		}
		row += pitch;
	}

	
	const u32 MonitorRefreshHz = 60;
	const u32 GameUpdateHZ = MonitorRefreshHz / 2;
	f32 TargetSecondsPerFrame = 1.0f;// / (f32)GameUpdateHZ;

	MSG message;
	LARGE_INTEGER LastCounter;
	QueryPerformanceCounter(&LastCounter);
	bRunning = true;
	u32 R = 0xFF0000FF;
	u32 G = 0xFF00FF00;
	u32 B = 0xFFFF0000;
	u32 Colors[3];
	Colors[0] = R;
	Colors[1] = G;
	Colors[2] = B;
	u32 ColorIndex = 0;
	while (bRunning) {
		while (PeekMessageA(&message, 0, 0, 0, PM_REMOVE)) {
			if (!bRunning) {
				break;
			}
			TranslateMessage(&message);
			DispatchMessageA(&message);
		}

		//update 
		{
			u8*row = (u8*)pBits;
			for (int y = 0; y < HEIGHT; ++y) {
				u32* pixel = (u32*)row;
				for (int x = 0; x < WIDTH; ++x) {
					*pixel = Colors[ColorIndex];
			
					++pixel;
				}
				row += pitch;
			}
		}
		ColorIndex++;

		LARGE_INTEGER EndCounter;
		QueryPerformanceCounter(&EndCounter);
		s64 CounterElapsed = EndCounter.QuadPart - LastCounter.QuadPart;

		f32 SecondsElapsedForWork = (f32)CounterElapsed / (f32) PerfCountFrequency;
		f32 SecondsElapsedForFrame = SecondsElapsedForWork;

		// wait for target seconds
		while (SecondsElapsedForFrame < TargetSecondsPerFrame) {
			QueryPerformanceCounter(&EndCounter);
			CounterElapsed = EndCounter.QuadPart - LastCounter.QuadPart;
			SecondsElapsedForFrame =  (f32)CounterElapsed / (f32)PerfCountFrequency;
		}



		//render
		HDC DeviceContext = GetDC(hWnd);
		Win32DisplayBufferInWindow( &GlobalBackBuffer,DeviceContext,ClientRect);
		ReleaseDC(hWnd, DeviceContext);   

		f32 MSPerFrame = 1000.0f*(f32)CounterElapsed / (f32)PerfCountFrequency;
		f32 FPS = (f32)(PerfCountFrequency / (f32)CounterElapsed);
		char Buffer[256];
		sprintf(Buffer, "%.02fms/f / %.02ff/s\n", MSPerFrame, FPS);
		OutputDebugStringA(Buffer);
		LastCounter = EndCounter;


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
			HDC DeviceContext = BeginPaint(
				hWnd, 
				&ps
			);

			RECT ClientRect;
			GetClientRect(hWnd,&ClientRect);
			Win32DisplayBufferInWindow( &GlobalBackBuffer,DeviceContext,ClientRect);


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
