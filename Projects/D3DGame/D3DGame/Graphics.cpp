#include "Graphics.h"

ID3D11Device* Graphics::d3dDevice{ nullptr };
ID3D11DeviceContext* Graphics::d3dDeviceContext{ nullptr };
D3D11_VIEWPORT Graphics::viewport{ 0 };

Graphics::Graphics()
{
	swapChain = nullptr;
	renderTargetView = nullptr;
	blendState = nullptr;
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	bgColorRGBA[0] = .0f;
	bgColorRGBA[1] = .0f;
	bgColorRGBA[2] = .0f;
	bgColorRGBA[3] = .0f;

	vSync = false;

}

Graphics::~Graphics()
{
	if (blendState)
	{
		blendState->Release();
		blendState = nullptr;
	}
	if (renderTargetView)
	{
		renderTargetView->Release();
		renderTargetView = nullptr;
	}

	if (swapChain)
	{
		swapChain->SetFullscreenState(false, NULL);
		swapChain->Release();
		swapChain = nullptr;
	}

	if (d3dDeviceContext)
	{
		d3dDeviceContext->ClearState();
		d3dDeviceContext->Release();
		d3dDeviceContext = nullptr;
	}
	
	if (d3dDevice)
	{
		d3dDevice->Release();
		d3dDevice = nullptr;
	}

}

bool Graphics::Initialize(Window* window)
{
	uint createDeviceFlags{};

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

	//DC

	//cria o dispositivo gr�fico
	D3D11CreateDevice(
		NULL,                       // adaptador de v�deo (NULL = adaptador padr�o)
		D3D_DRIVER_TYPE_HARDWARE,   // tipo de driver D3D (Hardware Reference ou Software)
		NULL,                       // ponteiro para rasterizador em software
		createDeviceFlags,			// modo de depura��o ou modo normal
		NULL,                       // n�vel de recursos do D3D (NULL = maior suportado)
		0,                          // tamanho do vetor de n�vel de recursos
		D3D11_SDK_VERSION,          // vers�o do SDK do Direct3D
		&d3dDevice,                 // guarda o dispositivo D3D criado
		&featureLevel,              // n�vel de recursos do D3D utilizado
		&d3dDeviceContext           // contexto do dispositivo D3D 
	);

	COLORREF color{ window->GetBackgroundColor()};

	bgColorRGBA[0] = GetRValue(color) / 255.f;
	bgColorRGBA[1] = GetGValue(color) / 255.f;
	bgColorRGBA[2] = GetBValue(color) / 255.f;
	bgColorRGBA[3] = 1.f;

	//SWAP CHAIN
	
	//define a swap chain
	DXGI_SWAP_CHAIN_DESC swapDesc{ 0 }; // descri��o da swap chain

	swapDesc.BufferDesc.Width = window->GetClientWidth();       // largura do backbuffer
	swapDesc.BufferDesc.Height = window->GetClientHeight();     // altura do backbuffer
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;             // taxa de atualiza��o em hz
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;            // denominador da taxa de atualiza��o
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // formato de cores RGBA 32 bits
	swapDesc.SampleDesc.Count = 1;                              // amostras de pixal (antialiasing)
	swapDesc.SampleDesc.Quality = 0;                            // n�vel de qualidade da imagem
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // uso da superf�cie
	swapDesc.BufferCount = 2;                                   // n�mero de buffers (front + back)
	swapDesc.OutputWindow = window->GetHandle();                // identificador da janela
	swapDesc.Windowed = window->GetMode() == WINDOWED;          // modo janela?
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;        // efeito da troca (descarte)
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // tela cheia (backbuffer/desktop)


	// pega o ponteiro para o dispositivo gr�fico
	IDXGIDevice* dxgiDevice{ nullptr };
	if FAILED(d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice))
	{
		return false;
	}

	// pega o ponteiro para adaptador controlado pelo dispositivo gr�fico
	IDXGIAdapter* dxgiAdapter{ nullptr };
	if FAILED(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter))
	{
		return false;
	}

	//pega o ponteiro para a DXGIFactory do adaptador
	IDXGIFactory* dxgiFactory{ nullptr };
	if FAILED(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory))
	{
		return false;
	}

	//cria a swap chain
	if FAILED(dxgiFactory->CreateSwapChain(d3dDevice, &swapDesc, &swapChain))
	{
		return false;
	}

	//previne o alt+enter
	if FAILED(dxgiFactory->MakeWindowAssociation(window->GetHandle(), DXGI_MWA_NO_ALT_ENTER))
	{
		return false;
	}

	//RENDER TARGET
	
	//pega o back buffer da swap chain
	ID3D11Texture2D* backBuffer{ nullptr };

	if FAILED(
		swapChain->GetBuffer(
			0,                          // �ndice do buffer
			__uuidof(ID3D11Texture2D),  // uuid da interface usada para o buffer
			(void**)&backBuffer         // superf�cie do backbuffer
		)
	)
	{
		return false;
	}

	if FAILED(
		d3dDevice->CreateRenderTargetView(
			backBuffer,                 // superf�cie a ser utilizada
			NULL,                       // acessa recursos em mipmap level 0
			&renderTargetView           // render target view
		)
	)
	{
		return false;
	}

	//liga a render-target ao est�gio output-merger
	d3dDeviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);

	//VIEWPORT
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = window->GetClientWidth();
	viewport.Height = window->GetClientHeight();
	viewport.MinDepth = .0f;
	viewport.MaxDepth = 1.0f;

	//liga a viewport ao est�gio de rasteriza��
	d3dDeviceContext->RSSetViewports(1, &viewport);

	//BLEND STATE

	 // Equa��o de mistura de cores (blending):
	// finalColor = SrcColor * SrcBlend <OP> DestColor * DestBlend

	// Combinando superf�cies transparentes (Alpha Blending)
	// finalColor = SrcColor * ScrAlpha + DestColor * (1-SrcAlpha)

	D3D11_BLEND_DESC blendDesc = { 0 };
	blendDesc.AlphaToCoverageEnable = false;                                // destaca a silhueta dos sprites
	blendDesc.IndependentBlendEnable = false;                               // usa mesma mistura para todos os render targets
	blendDesc.RenderTarget[0].BlendEnable = true;                           // habilita o blending
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;             // fator de mistura da fonte 
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;        // destino da mistura RGB � o alpha invertido 
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;                 // opera��o de adi��o na mistura de cores
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;              // fonte da mistura Alpha � o alpha do pixel shader
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;            // destino da mistura Alpha � o alpha invertido
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;            // opera��o de adi��o na mistura de cores
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;                 // componentes de cada pixel que podem ser sobrescritos

	// cria a blend state
	if FAILED(d3dDevice->CreateBlendState(&blendDesc, &blendState))
	{
		return false;
	}

	// liga a blend state ao est�gio Output-Merger
	d3dDeviceContext->OMSetBlendState(blendState, nullptr, 0xffffffff);

	// -------------------------------
	// Libera interfaces DXGI
	// -------------------------------

	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();
	backBuffer->Release();

	// inicializa��o bem sucedida
	return true;

}