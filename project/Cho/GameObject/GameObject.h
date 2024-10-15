// C++
#include<string>

class GameObject {
public:
    std::string name;
   // Transform transform;  // 位置、回転、スケールを持つ
    bool isActive;

    GameObject() : name("New GameObject"), isActive(true) {}

    virtual void Update(float deltaTime) {
        // ゲームオブジェクトの状態を更新
    }

    virtual void Render() {
        // レンダリング処理
    }

    virtual ~GameObject() {}
};