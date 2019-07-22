#pragma once
class imagedecoderclass
{
public:
	imagedecoderclass();
	void GetImageSize(WCHAR*, int& width, int& height);

private:
	ID2D1Factory* g_ipD2DFactory;
	ID2D1HwndRenderTarget* g_ipRT;
	IWICImagingFactory* g_ipWICFactory;
	IWICFormatConverter* g_ipConvertedSrcBmp;
	ID2D1Bitmap* g_ipD2DBitmap;
};

