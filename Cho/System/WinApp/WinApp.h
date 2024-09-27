#pragma once
/*-------------------------------------------------------------------------------------------*/
/*                                                                                           */
/*                                      ウィンドウズアプリケーションクラス					　　　*/
/*                                                                                           */
/*-------------------------------------------------------------------------------------------*/

#include <Windows.h>
#include<cstdint>

class WinApp
{
public:// 静的メンバ変数
	// クライアント領域サイズ
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;

public:// 静的メンバ関数

	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	/// <param name="hwnd"></param>
	/// <param name="msg"></param>
	/// <param name="wparam"></param>
	/// <param name="lparam"></param>
	/// <returns></returns>
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg,
		WPARAM wparam, LPARAM lparam);

public:// メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~WinApp() = default;

	/// <summary>
	/// ゲームウィンドウの作成
	/// </summary>
	void CreateGameWindow();

	/// <summary>
	/// メッセージ処理
	/// </summary>
	bool ProcessMessage();

	/// <summary>
	/// ウィンドウの破棄
	/// </summary>
	void TerminateWindow();

	HWND GetHwnd() const { return hwnd_; }

	HINSTANCE GetHInstance() const { return wc_.hInstance; }

private:// メンバ変数
	HWND hwnd_ = nullptr;

	WNDCLASS wc_{}; // ウィンドウクラス
};

