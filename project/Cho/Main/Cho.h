#pragma once

// System
class WinApp;
class ResourceLeakChecker;
class Framelate;
class DXGIFactory;
class D3DDevice;

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

	/*ループ終了*/
	static bool endRequest_;
};

