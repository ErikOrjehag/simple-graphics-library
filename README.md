#Simple Graphics Library
[![Gitter](https://badges.gitter.im/Join Chat.svg)](https://gitter.im/ErikOrjehag/simple-graphics-library?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

SGL is a small graphics wrapper for [GDI+](http://msdn.microsoft.com/en-us/library/windows/desktop/ms533798(v=vs.85).aspx) designed to help windows C++ developers to draw stuff with very little effort. Please visit the [official website](http://orjehag.se/SGL) for more information. A stable version can be found on our website, the developer version can be downloaded from this repository.

## To do

[![ranom image](http://www.captchacreator.com/captcha/captchac_code_google.php)](http://google.se)

Are you eager to get involved in this project? Fantastic! Here are some stuff that needs fixing.

* Spell check the website.
* **Fix crash when window is closed and the user has manually resized the window.**
* Hide some public variables and methods (Marked with a comment in the header file).
* Minimize flickering caused by manual resize of the window.
* Make copyBrush() and copyPen() not return pointers.
* ~~setPenStyle(SOLID, DASHED or DOTTED).~~
* ~~drawPixel(float, flaot).~~
* ~~setWindowTitle(string).~~
* ~~getWidth(), getHeight().~~
* showConsoleWindow(bool).
* Somehow open a window without SubSystem set to WINDOWS.
* **Implement an input API.**
* Code clean up by someone who's actually experienced in C++ (because I'm not).

We don't need a pile of extra GDI+ wrapper methods. Let's focus on creating a solid easy to use API.
