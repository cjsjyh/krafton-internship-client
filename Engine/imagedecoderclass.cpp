#include "stdafx.h"

#include "imagedecoderclass.h"

imagedecoderclass::imagedecoderclass()
{
	g_ipD2DFactory = nullptr;
	g_ipRT = nullptr;
	g_ipWICFactory = nullptr;
	g_ipConvertedSrcBmp = nullptr;
	g_ipD2DBitmap = nullptr;
}

void imagedecoderclass::GetImageSize(WCHAR* fname, int& width, int& height)
{
	HRESULT hr = E_FAIL;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_ipD2DFactory);
	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&g_ipWICFactory));

	IWICBitmapDecoder* ipDecoderPtr = nullptr;
	hr = g_ipWICFactory->CreateDecoderFromFilename(fname, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &ipDecoderPtr);

	IWICBitmapFrameDecode* ipFramePtr = nullptr;
	hr = ipDecoderPtr->GetFrame(0, &ipFramePtr);

	UINT uiWidth, uiHeight;
	ipFramePtr->GetSize(&uiWidth, &uiHeight);
	
	width = (int)uiWidth;
	height = (int)uiHeight;
}