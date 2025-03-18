#include <windows.h>

struct program{
	int running;
	int paused;
	int screen_dimensions[2];
	//
	int score;
};
struct program program = { 0 };
void programInit(){
	program.running = 1;
	program.paused = 0;
	program.screen_dimensions[0] = 400;
	program.screen_dimensions[1] = 600;
	return;
}

LRESULT CALLBACK proc(HWND, UINT, WPARAM, LPARAM);

WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmdline, int showcmd){
	WNDCLASS window_class = { 0 };
	window_class.lpfnWndProc = proc;
	window_class.hInstance = hInstance;
	window_class.lpszClassName = "tetris";
	RegisterClass(&window_class);
	
	HWND window = CreateWindow(
		"tetris",
		"tetris",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,400,600,
		NULL,NULL,hInstance,NULL
	);
	ShowWindow(window, showcmd);
	UpdateWindow(window);

	//init
	programInit();
	
	MSG msg;
	while(program.running){
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if (msg.message == WM_QUIT)
				program.running = 0;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

LRESULT CALLBACK proc(HWND window, UINT message, WPARAM wp, LPARAM lp){
	static HBITMAP bitBuffer = NULL;
	static HDC hdcBuffer = NULL;
	switch(message){
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_SIZE:{
			RECT screen;
			GetClientRect(window, &screen);
			program.screen_dimensions[0] = screen.right;
			program.screen_dimensions[1] = screen.bottom;
			break;
		}
		case WM_KEYDOWN:
			if (wp == VK_ESCAPE)
				DestroyWindow(window);
			break;
		case WM_PAINT:{
			PAINTSTRUCT paint_struct;
			HDC hdc = BeginPaint(window, &paint_struct);
			//select our buffer
			hdcBuffer = CreateCompatibleDC(hdc);
			bitBuffer = CreateCompatibleBitmap(hdc, program.screen_dimensions[0], program.screen_dimensions[1]);
			SelectObject(hdcBuffer, bitBuffer);
			
			RECT background;
			GetClientRect(window, &background);
			HBRUSH color = CreateSolidBrush(RGB(0,0,0));
			FillRect(hdcBuffer, &background, color);
			DeleteObject(color);
			
			if (program.paused)
				DrawText()
			
			BitBlt(hdc, 0, 0, program.screen_dimensions[0], program.screen_dimensions[1], hdcBuffer, 0, 0, SRCCOPY);
			EndPaint(window, &paint_struct);
		}
		default:
			return DefWindowProc(window, message, wp, lp);
	}
	return 0;
}
