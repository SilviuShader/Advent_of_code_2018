//
// Game.h
//

#pragma once

#include "StepTimer.h"


#define HEIGHT_DECREASE_MULTIPLIER 0.4f

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:

	struct TreeNode
	{
		TreeNode() : 
			startPos(0),
			endPos(0),
			nodeValue(-1),
			metadata(std::vector<int>()),
			children(std::vector<TreeNode>())
		{
		}

		int startPos;
		int endPos;

		int nodeValue;

		std::vector<int>      metadata;
		std::vector<TreeNode> children;
	};

    Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

private:

	void ReadData();
	void CreateTree(TreeNode&);
	int  Solve1(TreeNode&);
	int  Solve2(TreeNode&);
	void Solve();

    void Update(DX::StepTimer const& timer);
	void RenderNode(TreeNode&, float);
    void Render();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();

    void OnDeviceLost();

    // Device resources.
    HWND                                                                   m_window;
    int                                                                    m_outputWidth;
    int                                                                    m_outputHeight;

    D3D_FEATURE_LEVEL                                                      m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device1>                                  m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>                           m_d3dContext;

    Microsoft::WRL::ComPtr<IDXGISwapChain1>                                m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>                         m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>                         m_depthStencilView;

    // Rendering loop timer.
    DX::StepTimer                                                          m_timer;

	std::ifstream                                                          m_fin;
	std::ofstream                                                          m_fout;
	std::vector<int>                                                       m_inputData;

	TreeNode                                                               m_tree;

	std::unique_ptr<DirectX::CommonStates>                                 m_states;
	std::unique_ptr<DirectX::BasicEffect>                                  m_effect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>                              m_inputLayout;

	DirectX::SimpleMath::Matrix                                            m_world;
	DirectX::SimpleMath::Matrix                                            m_view;
	DirectX::SimpleMath::Matrix                                            m_proj;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState>                          m_raster;
	std::unique_ptr<DirectX::Mouse>                                        m_mouse;

	float                                                                  m_maxHeight;
	float                                                                  m_scale;
};