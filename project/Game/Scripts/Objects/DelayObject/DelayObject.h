#pragma once
#include <iostream>
#include <deque>
#include <utility>
#include "Vector3.h"

class DelayObject
{
public:
    virtual ~DelayObject() {};
    void DelayUpdate(const Vector3& newPosition);

    Vector3 getDelayedPosition() const;

    void SetDelayTime(float delayTime) { delayTime_ = delayTime; }

    void DelayReset();

private:
    float delayTime_ = 0.0f;                               // 遅延させる時間（秒）
    float currentTime_ = 0.0f;                             // 現在の経過時間（秒）
    std::deque<std::pair<float, Vector3>> history;         // 過去の時刻と位置を保存
};

