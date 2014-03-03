
/*********************************************************

	COPYRIGHT NOTICE

	You may:
	
	*	Alter this source if you give proper credit.
		Feel free to fork the official GitHub 
		repository.

	*	Make profit using this library if you give 
		proper credit.

	*	Distribute this library if you give proper 
		credit.

	You may not:

	*	Leave out proper creadit. If the library
		is used for non personal use a link to the
		official website or the GitHub repository 
		is appropritate.

	Copyright SGL 2014, Erik Örjehag

**********************************************************/

#ifndef UNICODE
#define UNICODE
#endif

#include "SGL.h"

SGL *sgl = NULL;

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	const WCHAR CLASS_NAME[]  = L"winSGL";
    
    WNDCLASS            wndClass;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
   
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
   
	wndClass.style          = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc    = WindowProc;
	wndClass.cbClsExtra     = 0;
	wndClass.cbWndExtra     = 0;
	wndClass.hInstance      = hInstance;
	wndClass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName   = NULL;
	wndClass.lpszClassName  = CLASS_NAME;
   
	RegisterClass(&wndClass);

	sgl = new SGL();
	
	sgl->window = CreateWindowEx (
		0,						// Optional window styles
		CLASS_NAME,				// Window class
		L"SGL App",				// Window text
		WS_OVERLAPPEDWINDOW,	// Window style
		CW_USEDEFAULT,			// Position x
		CW_USEDEFAULT,			// Position y
		CW_USEDEFAULT,			// Width
		CW_USEDEFAULT,			// Height
		NULL,					// Paranet window
		NULL,					// Menu
		hInstance,				// Instance handle
		NULL					// Additional application data
	);

	if(sgl->window == NULL) return 0;
	ShowWindow(sgl->window, nCmdShow);
	SetEvent(sgl->evCanDraw);

	MSG msg = {};

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);
	return msg.wParam;
}

DWORD WINAPI UserThread(LPVOID lpParameter)
{	
	WaitForSingleObject(sgl->evCanDraw, INFINITE);
	main();
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		sgl->hUserThread = CreateThread(0, 0, UserThread, 0, 0, 0);
		return 0;

	case WM_DESTROY:
		sgl->cleanUp();
		return 0;

	case WM_SIZE:
		sgl->resize();
		return 0;

	case WM_PAINT:
		return 0;

	case WM_ERASEBKGND:
		return 0;

	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

SGL::SGL()
{
	gslen = 0;
	evCanDraw = CreateEvent(0, TRUE, FALSE, L"evCanDraw");
	mxUseGraphics = CreateMutex(0, FALSE, L"mxUseGraphics");

	fontSize = 24;
	strokeWidth = 1; alpha = 255;
	red = 255; green = 255; blue = 255;
	updateTools();
}

void SGL::resize()
{
	using namespace Gdiplus;

	waitTillCanUseGraphics();

	RECT client;
	GetClientRect(window, &client);
	width = client.right - client.left;
	height = client.bottom - client.top;

	screenDC = BeginPaint(window, &ps);
	bufferDC = CreateCompatibleDC(screenDC);
	HBITMAP bmp = CreateCompatibleBitmap(screenDC, width, height);
	SelectObject(bufferDC, bmp); // Leak?

	if(graphics) delete graphics;
	graphics = new Graphics(bufferDC);
	graphics->SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);
	graphics->Clear(Color::White);

	stopUsingGraphics();
}

void SGL::updateTools()
{
	using namespace Gdiplus;

	waitTillCanUseGraphics();

	if(pen) delete pen;
	if(brush) delete brush;

	pen = new Pen(Color(alpha, red, green, blue), strokeWidth);
	brush = new SolidBrush(Color(alpha, red, green, blue));

	stopUsingGraphics();
}

void SGL::cleanUp()
{
	EndPaint(window, &ps);
	if(graphics) delete graphics;
	if(brush) delete brush;
	if(pen) delete pen;
	ResetEvent(evCanDraw);
	TerminateThread(hUserThread, 0);
    PostQuitMessage(0);
}

//		   ###    ########  #### 
//		  ## ##   ##     ##  ##  
//		 ##   ##  ##     ##  ##  
//		##     ## ########   ##  
//		######### ##         ##  
//		##     ## ##         ##  
//		##     ## ##        #### 

void SGL::waitTillCanUseGraphics()
{
	WaitForSingleObject(mxUseGraphics, INFINITE);
}

void SGL::stopUsingGraphics()
{
	ReleaseMutex(mxUseGraphics);
}

void SGL::render()
{
	waitTillCanUseGraphics();
	BitBlt(screenDC, 0, 0, width, height, bufferDC, 0,0, SRCCOPY);
	stopUsingGraphics();
}

void SGL::setStrokeWidth(float w)
{
	strokeWidth = w;
	updateTools();
}

void SGL::setColor(byte r, byte g, byte b)
{
	red = r;
	green = g;
	blue = b;
	updateTools();
}

void SGL::setAlpha(float a)
{
	alpha = 255 * a;
	updateTools();
}

void SGL::setFontSize(int s)
{
	fontSize = s;
}

void SGL::setWindowSize(int w, int h)
{
	SetWindowPos(window, 0, 0, 0, w, h, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
}

void SGL::save()
{
	waitTillCanUseGraphics();
	gs[gslen] = graphics->Save();
	gslen++;
	stopUsingGraphics();
}

void SGL::restore()
{
	waitTillCanUseGraphics();
	graphics->Restore(gs[gslen-1]);
	gslen--;
	stopUsingGraphics();
}

void SGL::translate(float x, float y)
{
	waitTillCanUseGraphics();
	graphics->TranslateTransform(x, y);
	stopUsingGraphics();
}

void SGL::rotate(float angle)
{
	waitTillCanUseGraphics();
	graphics->RotateTransform(angle);
	stopUsingGraphics();
}

void SGL::scale(float x, float y)
{
	waitTillCanUseGraphics();
	graphics->ScaleTransform(x, y);
	stopUsingGraphics();
}

void SGL::clear(byte r, byte g, byte b)
{
	waitTillCanUseGraphics();
	graphics->Clear(Gdiplus::Color(r, g, b));
	stopUsingGraphics();
}

void SGL::fillRect(float x, float y, float w, float h)
{
	waitTillCanUseGraphics();
	graphics->FillRectangle(brush, x, y, w, h);
	stopUsingGraphics();
}

void SGL::fillTriangle(float x, float y, float w, float h)
{
	using namespace Gdiplus;

	Point points[] = {
		Point(x + w / 2, y),
		Point(x, y + h),
		Point(x + w, y + h),
	};

	fillPolygon(*points, 3);
}

void SGL::fillCircle(float x, float y, float r)
{
	waitTillCanUseGraphics();
	graphics->FillEllipse(brush, x, y, r, r);
	stopUsingGraphics();
}

void SGL::fillEllipse(float x, float y, float w, float h)
{
	waitTillCanUseGraphics();
	graphics->FillEllipse(brush, x, y, w, h);
	stopUsingGraphics();
}

void SGL::fillPolygon(Gdiplus::Point &points, int count)
{
	waitTillCanUseGraphics();
	graphics->FillPolygon(brush, &points, count);
	stopUsingGraphics();
}

void SGL::strokeRect(float x, float y, float w, float h)
{
	waitTillCanUseGraphics();
	graphics->DrawRectangle(pen, x, y, w, h);
	stopUsingGraphics();
}

void SGL::strokeTriangle(float x, float y, float w, float h)
{
	using namespace Gdiplus;

	Point points[] = {
		Point(x + w / 2, y),
		Point(x, y + h),
		Point(x + w, y + h),
	};

	strokePolygon(*points, 3);
}

void SGL::strokeCircle(float x, float y, float r)
{
	waitTillCanUseGraphics();
	graphics->DrawEllipse(pen, x, y, r, r);
	stopUsingGraphics();
}

void SGL::strokeEllipse(float x, float y, float w, float h)
{
	waitTillCanUseGraphics();
	graphics->DrawEllipse(pen, x, y, w, h);
	stopUsingGraphics();
}

void SGL::strokePolygon(Gdiplus::Point &points, int count)
{
	waitTillCanUseGraphics();
	graphics->DrawPolygon(pen, &points, count);
	stopUsingGraphics();
}

void SGL::drawLine(float startx, float starty, float endx, float endy)
{
	waitTillCanUseGraphics();
	graphics->DrawLine(pen, startx, starty, endx, endy);
	stopUsingGraphics();
}

void SGL::moveTo(float x, float y)
{
	xPenPos = x;
	yPenPos = y;
}

void SGL::lineTo(float x, float y)
{
	waitTillCanUseGraphics();
	graphics->DrawLine(pen, xPenPos, yPenPos, x, y);
	moveTo(x, y);
	stopUsingGraphics();
}

void SGL::drawString(const WCHAR str[], float x, float y)
{
	using namespace Gdiplus;

	waitTillCanUseGraphics();

	FontFamily  fontFamily(L"Verdana");
	Font        font(&fontFamily, fontSize, FontStyleRegular, UnitPixel);
	PointF      pos(x, y);

	graphics->DrawString(str, -1, &font, pos, brush);

	stopUsingGraphics();
}

void SGL::drawImage(Gdiplus::Image *img, float x, float y) {
	waitTillCanUseGraphics();
	graphics->DrawImage(img, x, y, img->GetWidth(), img->GetHeight());
	stopUsingGraphics();
}

Gdiplus::Graphics *SGL::useGDI()
{
	return graphics;
}

Gdiplus::Pen *SGL::copyPen()
{
	using namespace Gdiplus;

	return new Pen(Color(alpha, red, green, blue), strokeWidth);
}

Gdiplus::SolidBrush *SGL::copyBrush()
{
	using namespace Gdiplus;

	return new SolidBrush(Color(alpha, red, green, blue));
}