#pragma once

#include<d3d12.h>
#include<wrl.h>

class D3DCommand;
class DrawExecution
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(D3DCommand* d3dCommand);

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

private:
	/*借りインスタンス*/
	D3DCommand* d3dCommand_ = nullptr;
};

