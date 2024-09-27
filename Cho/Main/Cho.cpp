#include "Cho.h"

// 静的メンバー変数の定義
bool Cho::endRequest_ = false;
std::unique_ptr <WinApp> Cho::win = nullptr;

// System
#include"WinApp/WinApp.h"

void Cho::Initialize()
{
	// ウィンドウの作成
	win = std::make_unique<WinApp>();
	win->CreateGameWindow();
}

void Cho::Finalize()
{
	// ウィンドウの破棄
	win->TerminateWindow();
}

void Cho::Operation()
{
	/*初期化*/
	Initialize();
	/*メインループ*/
	while (true) {
		/*ウィンドウ終了リクエスト*/
		if (IsEndRequest()) {
			break;
		}
		/*毎フレーム更新*/
		Update();
		/*描画前処理*/
		PreDraw();
		/*描画*/
		Draw();
		/*描画後処理*/
		PostDraw();
	}
	/*終了処理*/
	Finalize();
}

void Cho::Update()
{
	if (win->ProcessMessage()) {
		endRequest_ = true;
	}
}

void Cho::PreDraw()
{
}

void Cho::Draw()
{
}

void Cho::PostDraw()
{
}

bool Cho::IsEndRequest()
{
	if (endRequest_) {
		return true;
	}
	return false;
}
