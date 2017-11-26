#define _WIN32_WINNT 0x0500
#include <iostream>
#include <windows.h>
#include <gdiplus.h>
#include <memory>
#include <sstream>

#pragma comment (lib,"Gdiplus.lib")

using namespace Gdiplus;
using namespace std;

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT num = 0;          // number of image encoders
    UINT size = 0;         // size of the image encoder array in bytes

    ImageCodecInfo* pImageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);
    if(size == 0)
    {
        return -1;  // Failure
    }

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if(pImageCodecInfo == NULL)
    {
        return -1;  // Failure
    }

    GetImageEncoders(num, size, pImageCodecInfo);

    for(UINT j = 0; j < num; ++j)
    {
        if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }
    }

    free(pImageCodecInfo);
    return -1;  // Failure
}

void BitmapToJpg(HBITMAP hbmpImage, int width, int height , const char * Pic_Name)
{
    Bitmap *p_bmp = Bitmap::FromHBITMAP(hbmpImage, NULL);
    //Bitmap *p_bmp = new Bitmap(width, height, PixelFormat32bppARGB);

    CLSID pngClsid;
    int result = GetEncoderClsid(L"image/jpeg", &pngClsid);
    if(result != -1)
        std::cout << "Encoder succeeded" << std::endl;
    else
        std::cout << "Encoder failed" << std::endl;

        //converting const char 8 to wchar*
    const WCHAR *pwcsName;
        // required size
    int nChars = MultiByteToWideChar(CP_ACP, 0, Pic_Name, -1, NULL, 0);
        // allocate it
    pwcsName = new WCHAR[nChars];
    MultiByteToWideChar(CP_ACP, 0, Pic_Name, -1, (LPWSTR)pwcsName, nChars);

    p_bmp->Save(pwcsName, &pngClsid, NULL);
    delete p_bmp;
    delete[] pwcsName;
}

bool ScreenCapture(int x, int y, int width, int height, const char *filename)
{
    HDC hDc = CreateCompatibleDC(0);
    HBITMAP hBmp = CreateCompatibleBitmap(GetDC(0), width, height);
    SelectObject(hDc, hBmp);
    BitBlt(hDc, 0, 0, width, height, GetDC(0), x, y, SRCCOPY);
    BitmapToJpg(hBmp, width, height,filename);
    DeleteObject(hBmp);
    return true;
}

int main() {
    // Initialize GDI+.
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    ShowWindow( GetConsoleWindow(), SW_HIDE );

    int x1 = 0;
    int y1 = 0;
    int x2 = GetSystemMetrics(SM_CXSCREEN);
    int y2 = GetSystemMetrics(SM_CYSCREEN);
    //system("pause");
    int Pic_No=0;
    string Pic_Name;
    stringstream sstm;

    while(1) {
    if (GetAsyncKeyState(VK_LBUTTON)){
        Pic_Name="screen";
        sstm << Pic_Name << Pic_No << ".jpg";
        Pic_Name = sstm.str();
        Pic_No++;
        ScreenCapture(x1, y1, x2 - x1, y2 - y1, Pic_Name.c_str());
        Pic_Name="";              //Empty string
        sstm.str(std::string());  //Empty sstream
        }
    }
    //Shutdown GDI+
    GdiplusShutdown(gdiplusToken);

    return 0;
}
