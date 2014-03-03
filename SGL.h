
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

#pragma once;
#include <windows.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI UserThread(LPVOID lpParameter);

class SGL
{
private:
	int width, height;
	HDC screenDC;
	HDC bufferDC;
	PAINTSTRUCT ps;
	HANDLE mxUseGraphics;
	Gdiplus::Graphics *graphics;
	Gdiplus::GraphicsState gs[100];
	unsigned gslen;
	int fontSize;
	float strokeWidth;
	byte alpha, red, green, blue;
	Gdiplus::Pen *pen;
	Gdiplus::SolidBrush *brush;
	float xPenPos;
	float yPenPos;
	void updateTools();

public:
	SGL();

	// These should be hidden from the user.
	HANDLE hUserThread;
	HANDLE evCanDraw;
	HWND window;
	void resize();
	void cleanUp();
	// ------------------------------------

	// API
	void waitTillCanUseGraphics();
	void stopUsingGraphics();
	void render();

	void setStrokeWidth(float w);
	void setColor(byte r, byte g, byte b);
	void setAlpha(float a);
	void setFontSize(int s);
	void setWindowSize(int w, int h);

	void save();
	void restore();
	void translate(float x, float y);
	void rotate(float angle);
	void scale(float x, float y);

	void clear(byte r, byte g, byte b);

	void fillRect(float x, float y, float w, float h);
	void fillTriangle(float x, float y, float w, float h);
	void fillCircle(float x, float y, float r);
	void fillEllipse(float x, float y, float w, float h);
	void fillPolygon(Gdiplus::Point &points, int count);

	void strokeRect(float x, float y, float w, float h);
	void strokeTriangle(float x, float y, float w, float h);
	void strokeCircle(float x, float y, float r);
	void strokeEllipse(float x, float y, float w, float h);
	void strokePolygon(Gdiplus::Point &points, int count);

	void drawLine(float startx, float starty, float endx, float endy);
	void moveTo(float x, float y);
	void lineTo(float x, float y);

	void drawString(const WCHAR str[], float x, float y);
	void drawImage(Gdiplus::Image *img, float x, float y);

	Gdiplus::Graphics *useGDI();
	Gdiplus::Pen *copyPen();
	Gdiplus::SolidBrush *copyBrush();
};

extern SGL *sgl;
void main();