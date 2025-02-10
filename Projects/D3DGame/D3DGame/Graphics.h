#ifndef _PROGJOGOS_GRAPHICS_H_
#define _PROGJOGOS_GRAPHICS_H_

#include <dxgi.h>
#include <d3d11.h>
#include "Window.h"
#include "Types.h"

class Graphics
{
private:
	IDXGISwapChain				* swapChain;			// swap chain
	ID3D11RenderTargetView		* renderTargetView;		// render target view do backbuffer
	ID3D11BlendState            * blendState;			// configuração da mistura de cores
	D3D_FEATURE_LEVEL		      featureLevel;			// nível de recursos D3D suportados pelo hardware
	float						  bgColorRGBA[4];		// cor de fundo do backbuffer
	bool						  vSync;				// vertical sync

public:
	Graphics();
	~Graphics();

	static ID3D11Device			* d3dDevice;			// dispositivo gráfico
	static ID3D11DeviceContext	* d3dDeviceContext;     // contexto do dispositivo gráfico
	static D3D11_VIEWPORT		  viewport;				// viewport	

	void SetVSync(bool);
	void Clear();										// limpa o backbuffer com a cor de fundo
	void Present();										// apresenta o desenho na tela
	bool Initialize(Window*);							// inicializa o Direct3D
};

inline void Graphics::SetVSync(bool vSync)
{ this->vSync = vSync; }

inline void Graphics::Clear()
{ d3dDeviceContext->ClearRenderTargetView(renderTargetView, bgColorRGBA); }

inline void Graphics::Present()
{
	swapChain->Present(vSync, NULL);
	d3dDeviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
}

#endif //_PROGJOGOS_GRAPHICS_H_