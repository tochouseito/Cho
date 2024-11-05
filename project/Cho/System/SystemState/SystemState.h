#pragma once

#include<cstdint>

class SystemState {
public:
    static SystemState& GetInstance() {
        static SystemState instance;
        return instance;
    }
private:
    SystemState() {}
    SystemState(const SystemState&) = delete;
    SystemState& operator=(const SystemState&) = delete;

public:

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize();

   const float GetDeltaTime() const { return deltaTime; }

   const float GetFrameRate() const { return framePerSecond; }

private:
    int32_t windowSizeWidth = 1280;// ウィンドウ横幅
    int32_t windowSizeHeight = 720;// ウィンドウ縦幅

    float deltaTime = 0.0f;// デルタタイム
    float framePerSecond = 60.0f;// FPS

};

inline float DeltaTime() {
    return SystemState::GetInstance().GetDeltaTime();
}

inline float NowFrameRate() {
    return SystemState::GetInstance().GetFrameRate();
}

