#pragma once
class GameContext
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	void Update();

	void SetFrameValue(float framerate,float deltaTime );
private:
	/*Context*/

	// デルタタイム
	float deltaTime_;
	// フレームレート
	float frameRate_;
};

