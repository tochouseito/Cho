#pragma once

#include<d3d12.h>
#include <imgui.h>
#include <wrl.h>

class WinApp;
class D3DDevice;
class ResourceViewManager;
class ImGuiManager
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* win,D3DDevice* d3dDevice,ResourceViewManager* RVManager);

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGui受付開始
	/// </summary>
	void Begin();

	/// <summary>
	/// ImGui受付終了
	/// </summary>
	void End();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* commandList);
};

