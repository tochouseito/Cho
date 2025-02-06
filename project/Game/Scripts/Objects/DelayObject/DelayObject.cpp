#include "PrecompiledHeader.h"
#include "DelayObject.h"

void DelayObject::DelayUpdate(const Vector3& newPosition)
{
    currentTime_ += DeltaTime();

    // 現在の位置と時刻を履歴に追加
    history.push_back({ currentTime_, newPosition });

    // 過去の遅れたデータが保持する必要がない場合は削除
    while (!history.empty() && history.front().first < currentTime_ - delayTime_) {
        history.pop_front();
    }
}

Vector3 DelayObject::getDelayedPosition() const
{
    if (history.empty()) {
        return { 0.0f, 0.0f, 0.0f }; // デフォルト値（履歴が空の場合）
    }

    // 遅延時間を満たす最も近い位置を検索
    for (auto it = history.rbegin(); it != history.rend(); ++it) {
        if (it->first <= currentTime_ - delayTime_) {
            return it->second;
        }
    }

    // 遅延時間に満たない場合は最も古い位置を返す
    return history.front().second;
}

void DelayObject::DelayReset()
{
    history.clear();
}
