
/*********************************************************

	COPYRIGHT NOTICE (C) 2014 Erik Örjehag SGL

	You may:
	
	*	Make profit using this library.
	*	Distribute this library.
	*	Alter the source code.

	You may not:

	*	Leave out proper creadit. If this library
		(or a altered version) is used for non 
		personal use a link to the official website 
		or the GitHub repository is appropritate.

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
public:
	static const Gdiplus::DashStyle SOLID = Gdiplus::DashStyleSolid;
	static const Gdiplus::DashStyle DASHED = Gdiplus::DashStyleDash;
	static const Gdiplus::DashStyle DOTTED = Gdiplus::DashStyleDot;

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
	Gdiplus::DashStyle strokeStyle;
	byte alpha, red, green, blue;
	Gdiplus::Pen *pen;
	Gdiplus::SolidBrush *brush;
	float xPenPos;
	float yPenPos;
	void updateTools();

public:
	// These should ideally be hidden from the user.
	SGL();
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
	void setStrokeStyle(int s);
	void setColor(byte r, byte g, byte b);
	void setAlpha(float a);
	void setFontSize(int s);
	void setWindowSize(int w, int h);
	void setWindowTitle(const WCHAR str[]);

	int getWidth();
	int getHeight();

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
	void drawPixel(float x, float y);

	Gdiplus::Graphics *useGDI();
	Gdiplus::Pen *copyPen();
	Gdiplus::SolidBrush *copyBrush();
};

extern SGL *sgl;
void main();