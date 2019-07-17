////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <atlconv.h>

#include "textureclass.h"


TextureClass::TextureClass()
{
	m_texture = 0;
}


TextureClass::TextureClass(const TextureClass& other)
{
}


TextureClass::~TextureClass()
{
}


bool TextureClass::InitializeModel(ID3D11Device* device, WCHAR* filename)
{
	USES_CONVERSION;
	HRESULT result;

	// Load the texture in.
	//result = CreateDDSTextureFromFile(device,filename,nullptr,&m_texture);
	result = CreateWICTextureFromFile(device, filename, nullptr, &m_texture);
	if(FAILED(result))
	{
		cout << "failed" << endl;
		return false;
	}
	
	cout << "success" << endl;

	return true;
}

bool TextureClass::Initialize(ID3D11Device* device, WCHAR* filename)
{
	HRESULT result;
	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL);

	if (FAILED(result))
	{
		return false;
	}



	return true;
}


void TextureClass::Shutdown()
{
	// Release the texture resource.
	if(m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}


ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}