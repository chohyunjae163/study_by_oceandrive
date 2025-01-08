/*
 * water going into a jar~
 * all the computations done in CPU
 * 
 *
*/

#include <windows.h>
#include <windowsx.h>
#include <stdbool.h>
#include <stdio.h>
#include <wingdi.h>
#include <stdint.h>
#include <assert.h>

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
	int Pitch;
	int BytesPerPixel;
} win32_offscreen_buffer;

global_variable win32_offscreen_buffer GlobalBackBuffer;

typedef struct {
	u32 X;
	u32 Y;
	u32 Radius;
} Hole;

typedef struct
{
	u32 X;
	u32 Y;
	u32 Color;
	bool IsLive;
} Sand;

//gameplay data
#define NUM_SAND 1000
Sand Sands[NUM_SAND];
static u32 StartX = 320;
static u32 StartY = 240;
static u32 SandColor = 0xFFFFFF00;
static u32 NumSandLive = 0;

void* pBits = NULL;

//A callback function, which you define in your application, that processes messages sent to a window
LRESULT CALLBACK MainWindowCallback(
	_In_ HWND   hWnd, //
	_In_ UINT   message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam);

internal void
Win32ResetBuffer(win32_offscreen_buffer *Buffer)
{
	if (Buffer->Memory) // Same as writing (BitmapMemory != 0) or (BitmapMemory != NULL)
	{
		VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
		// Optionally, you can check if the result of VirtualFree is not zero.
		// Print out an error message if it is.
	}
    Buffer->Width = WIDTH;
    Buffer->Height = HEIGHT;
    Buffer->BytesPerPixel = 4;
	Buffer->Pitch = Buffer->Width * Buffer->BytesPerPixel;
    
    Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
    Buffer->Info.bmiHeader.biWidth = Buffer->Width;
    Buffer->Info.bmiHeader.biHeight = -Buffer->Height; // negative value: top-down
    Buffer->Info.bmiHeader.biPlanes = 1;
    Buffer->Info.bmiHeader.biBitCount = 32;
    Buffer->Info.bmiHeader.biCompression = BI_RGB;
    
    int BitmapMemorySize = Buffer->BytesPerPixel * (Buffer->Width * Buffer->Height);
    
	Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE); 
}

internal void
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
	
	RECT rect;
	rect.left   = 50;
	rect.top    = 50;
	rect.right  = rect.left + WIDTH;
	rect.bottom = rect.top + HEIGHT;
	AdjustWindowRectEx(
		&rect,
		WS_OVERLAPPEDWINDOW,
		0,
		0);
	int AdjustedWidth =  rect.right - rect.left;
	int AdjustedHeight = rect.bottom - rect.top;
	HWND hWnd = CreateWindowA(
    WINDOW_CLASS_NAME,              // Window class
    "Learn to Program Windows",    // Window text
    WS_OVERLAPPEDWINDOW,            // Window style

    // Size and position
    rect.left, rect.top, rect.right - rect.left, AdjustedHeight,

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

	const u32 MonitorRefreshHz = 60;
	const u32 GameUpdateHZ = MonitorRefreshHz / 2;
	f32 TargetSecondsPerFrame = 1.0f / (f32)GameUpdateHZ;

	MSG message;
	LARGE_INTEGER LastCounter;
	QueryPerformanceCounter(&LastCounter);
	bRunning = true;

	//sand init
	
	{
		for (u32 i = 0; i < NUM_SAND; ++i) {
			Sands[i].X = StartX;
			Sands[i].Y = StartY;
			Sands[i].Color = SandColor;
			Sands[i].IsLive = false;
		} 
	}

	Sands[0].IsLive = true;

	Hole HoleInGround;
	HoleInGround.X = 320;
	HoleInGround.Y = 330;
	HoleInGround.Radius = 15;

	u32 SandIndex = 0;
	//map
	u32 Map[480][640];
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			Map[y][x]  = 0xFF000000;
		}
	}
	
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

			//sand update
			const u32 SimNum = HEIGHT;

			if (SandIndex < NUM_SAND) {
				for (int i = 0; i < NUM_SAND; ++i) {
					if (Sands[i].IsLive) {
						u32 X = Sands[i].X;
						u32 Y = Sands[i].Y;
						if (Map[Y + 1][X] == 0xFF000000) {
							Sands[i].Y += 1;
						} else if (Map[Y + 1][X + 1] == 0xFF000000) {
							Sands[i].Y += 1;
							Sands[i].X += 1;
						} else if (Map[Y + 1][X - 1] == 0xFF000000) {
							Sands[i].Y += 1;
							Sands[i].X -= 1;
						}
						else if (Map[Y + 1][X] == 0xFF0000FF) {
						  //do nothing
						}
					}
				}

			}
			for (int y = 0; y < HEIGHT; ++y) {
				for (int x = 0; x < WIDTH; ++x) {
					if (y > HEIGHT - 150) {
						HoleInGround.X;
						HoleInGround.Y;
						u32 DistFromHole = (x - HoleInGround.X) * (x - HoleInGround.X) + (y - HoleInGround.Y) * (y - HoleInGround.Y);
						if (DistFromHole > HoleInGround.Radius * HoleInGround.Radius) {
							Map[y][x] = 0xFF0000FF;
						} else {
							Map[y][x]  = 0xFF000000;
						}
						
					}
					else {
						Map[y][x]  = 0xFF000000;
					}
			
				}
			}
			for (u32 i = 0; i < NUM_SAND; ++i) {
				if (!Sands[i].IsLive) {
					continue;
				}
				u32 X = Sands[i].X;
				u32 Y = Sands[i].Y;
				Map[Y][X] = Sands[i].Color;
			}
			
			//draw mouse point
			Map[StartY][StartX] = 0xFFFFFFFF;

			u8*row = (u8*)GlobalBackBuffer.Memory;
			for (int y = 0; y < HEIGHT; ++y) {
				u32* pixel = (u32*)row;
				for (int x = 0; x < WIDTH; ++x) {
					*pixel = Map[y][x];
					++pixel;
				}
				row += GlobalBackBuffer.Pitch;
			}
		}

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
		RECT ClientRect;
		GetClientRect(hWnd, &ClientRect);
		Win32DisplayBufferInWindow(&GlobalBackBuffer,DeviceContext,ClientRect);
		ReleaseDC(hWnd, DeviceContext);   

		f32 MSPerFrame = 1000.0f*(f32)CounterElapsed / (f32)PerfCountFrequency;
		f32 FPS = (f32)(PerfCountFrequency / (f32)CounterElapsed);
		char Buffer[256];
		sprintf(Buffer, "%.02f ms/f / %.02f f/s\n", MSPerFrame, FPS);
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
	POINT MousePoint;
	switch (message) {
		case WM_SIZE:
		{
			Win32ResetBuffer(&GlobalBackBuffer);
			break;
		}
		case WM_GETMINMAXINFO:
		{
			MINMAXINFO *minmax = (MINMAXINFO *)lParam;
			minmax->ptMinTrackSize.x = WIDTH;
			minmax->ptMinTrackSize.y = HEIGHT;
			break;
		}
		case WM_MOUSEMOVE:
		{
			MousePoint.x = GET_X_LPARAM(lParam);
			MousePoint.y = GET_Y_LPARAM(lParam);
			RECT ClientRect;
			GetClientRect(hWnd,&ClientRect);
			int WindowWidth = ClientRect.right - ClientRect.left;
			int WindowHeight = ClientRect.bottom - ClientRect.top;
			int ClientToBufferWidthRatio = WindowWidth / WIDTH;
			int ClientToBufferHeightRatio = WindowHeight / HEIGHT;
			int PointXInBuffer = MousePoint.x / ClientToBufferWidthRatio; 
			int PointYInBuffer = MousePoint.y / ClientToBufferHeightRatio;
			StartX = PointXInBuffer;
			StartY = PointYInBuffer;			
			break;
		}
		case WM_LBUTTONDOWN:
		{
			Sands[++NumSandLive].IsLive = true;
			Sands[NumSandLive].X = StartX;
			Sands[NumSandLive].Y = StartY;
			break;
		}
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
