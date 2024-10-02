#pragma once

// System
class WinApp;
class ResourceLeakChecker;
class Framelate;
class DXGIFactory;
class D3DDevice;
class D3DFence;
class D3DCommand;
class D3DSwapChain;
class ResourceViewManager;
class RTVManager;
class DSVManager;
class DrawExecution;

// Scene
class SceneManager;

// 汎用機能
class ImGuiManager;

// Editor
class EditorManager;

// C++
#include<memory>

class Cho
{
public:// メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	static void Finalize();

	/// <summary>
	/// 稼働
	/// </summary>
	static void Operation();

	/// <summary>
	/// 更新
	/// </summary>
	static void Update();

	/// <summary>
	/// 描画前処理
	/// </summary>
	static void PreDraw();

	/// <summary>
	/// 描画
	/// </summary>
	static void Draw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 終了リクエスト
	/// </summary>
	/// <returns></returns>
	static bool IsEndRequest();

private:// メンバ変数

	/*ウィンドウズアプリケーション*/
	static std::unique_ptr <WinApp> win;

	/*フレーム*/
	static std::unique_ptr<Framelate> framelate;

	/*DirectX*/
	static std::unique_ptr<ResourceLeakChecker> resourceLeakChecker;
	static std::unique_ptr<DXGIFactory> dxgiFactory;
	static std::unique_ptr<D3DDevice> d3dDevice;
	static std::unique_ptr<D3DFence> d3dFence;
	static std::unique_ptr<D3DCommand> d3dCommand;
	static std::unique_ptr<D3DSwapChain> d3dSwapChain;
	static std::unique_ptr<ResourceViewManager> resourceViewManager;
	static std::unique_ptr<RTVManager> rtvManager;
	static std::unique_ptr<DSVManager> dsvManager;
	static std::unique_ptr<DrawExecution> drawExecution;

	/*Scene*/
	static std::unique_ptr<SceneManager> sceneManager;

	/*汎用機能*/
	static std::unique_ptr<ImGuiManager> imguiManager;

	/*Editor*/
	static std::unique_ptr<EditorManager> editorManager;

	/*ループ終了*/
	static bool endRequest_;
};

