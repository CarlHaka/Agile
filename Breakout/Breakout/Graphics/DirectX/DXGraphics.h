#pragma once
#ifndef DXGRAPHICS_H
#define DXGRAPHICS_H
#include <windows.h>
#include <D3D11.h>
#include <d3dCompiler.h>

#include "../IGraphics.h"
#include "DXWindow.h"
#include "DXDeferred.h"
#include "../ICamera.h"

class DXGraphics : public IGraphics
{

private:
	DXWindow				*m_window;


	IDXGISwapChain			*m_swapChain = NULL;
	ID3D11RenderTargetView	*m_renderTargetView = NULL;

	ID3D11Texture2D			*m_depthStencil = NULL;
	ID3D11DepthStencilView	*m_depthStencilView = NULL;
	ID3D11Device			*m_device = NULL;
	ID3D11DeviceContext		*m_deviceContext = NULL;

	DXDeferred				*m_DXDeferred = NULL;


	HRESULT					InitDirect3D(DisplayMode _displayMode);

	int m_width, m_height;



public:

	DXGraphics(void);
	~DXGraphics(void);

	bool  InitWindow(int _x, int _y, int _width, int _height, DisplayMode _displayMode);
	bool  Init3D(DisplayMode _displayMode);
	DXWindow* GetWindow(){ return m_window; }
	void	Render(ICamera* _camera);
	void LoadModel(std::string _path);

};



#endif