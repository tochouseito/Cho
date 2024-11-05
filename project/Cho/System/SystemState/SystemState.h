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

   const float GetFrameRate() const { return MaxFramePerSecond; }

   const int32_t GetWindowWidth()const { return windowWidth; }

   const int32_t GetWindowHeight()const { return windowHeight; }

   void SetNowFPS(float value) { nowFPS = value; }

   const float GetNowFrameRate()const { return nowFPS; }

   void SetDeltaTime(float value) { deltaTime = value; }

private:
    int32_t windowWidth = 1280;// ウィンドウ横幅
    int32_t windowHeight = 720;// ウィンドウ縦幅

    float deltaTime = 0.0f;// デルタタイム
    float MaxFramePerSecond = 60.0f;// FPS

    float nowFPS = 0.0f;
};

inline float DeltaTime() {
    return SystemState::GetInstance().GetDeltaTime();
}

inline float MaxFrameRate() {
    return SystemState::GetInstance().GetFrameRate();
}

inline int32_t WindowWidth() {
    return SystemState::GetInstance().GetWindowWidth();
}

inline int32_t WindowHeight() {
    return SystemState::GetInstance().GetWindowHeight();
}

inline float NowFrameRate() {
    return SystemState::GetInstance().GetNowFrameRate();
}