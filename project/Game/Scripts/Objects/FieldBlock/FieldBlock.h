#pragma once
#include "Game/Scripts/Objects/BaseObject/BaseObject.h"

// Item
#include "Game/Scripts/Objects/Item/BaseItem.h"
#include "Game/Scripts/Objects/Item/HealingItem.h"
#include "Game/Scripts/Objects/Item/PowerUpItem.h"
#include "Game/Scripts/Objects/Item/SpeedUpItem.h"

#include "Game/Scripts/Objects/Board/Board.h"

//C++
#include <vector>
#include <array>
#include <map>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>

enum class MapChipType {
	kBlank, // 空白
	kBlock, // ブロック
	kPlayer,// プレイヤー初期位置
	kEnemy,	// エネミー初期位置
	kHP,		// 回復アイテム
	kPower,		// 攻撃アップアイテム
	kSpeed,		// スピードアップアイテム
	kBoard,	// ボード
};

enum Lane {
	kLeftLane = 0,	// 左レーン
	kCenterLane,	// 中央レーン
	kRightLane,		// 右レーン
	kNumLane		// レーン数
};

class Block : public BaseObject
{
public:
	// BaseObject を介して継承されました
	void Initialize() override;
	void Update() override;
	void SetPosition(const Vector3& position,const uint32_t& index);
	void SetScale(const Scale& scale, const uint32_t& index);
	void SetLighting(int flag) { material->enableLighting = flag; }
	void ChangeVisible(bool flag);

	void ScaleReset();
	Vector3 GetPos(const uint32_t& index);
};

class FieldBlock : public BaseObject
{
public:
	struct IndexSet {
		uint32_t xIndex;
		uint32_t yIndex;
	};
	struct MapChipData {
		std::vector<std::vector<MapChipType>> data;
	};
	struct Rect {
		float left;   // 左端
		float right;  // 右端
		float bottom; // 下端
		float top;    // 上端
	};
public:
	// BaseObject を介して継承されました
	void Initialize() override;
	void Update() override;

	void SetPtr(SceneManager* sceneManager, ComponentManager* compManager, InputManager* inputManager);

	// マップデータ読み込み
	void LoadMapData(const std::string& directoryPath);

	// フィールドブロック生成
	void GenerateFieldBlock();

	void GenerateItems();

	// マップチップの種類を取得
	MapChipType GetMapChipTypeByIndex(const uint32_t& xIndex, const uint32_t& yIndex, const Lane& laneNum);
	// マップチップの位置を取得
	Vector3 GetMapChipPositionByIndex(const uint32_t& xIndex, const uint32_t& yIndex, const Lane& laneNum);
	// マップチップの位置を取得
	float GetMapChipPositionYByIndex(const uint32_t& yIndex);
	// ブロックの個数を取得
	uint32_t GetNumBlockVertical() {return kNumBlockVertical;}
	// ブロックの個数を取得
	uint32_t GetNumBlockHorizontal() {return kNumBlockHorizontal;}
	// マップチップのインデックスを取得
	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);
	// マップチップの矩形を取得
	Rect GetRectByIndex(const uint32_t& xIndex, const uint32_t& yIndex, const Lane& laneNum);
	// レーンのオフセットを取得
	float GetLaneOffset(const Lane& laneNum);


	// マップチップのインデックスからtransformsのインデックスを取得
	std::optional<uint32_t> GetTransformsIndexByMapChip(const MapChipType type,const uint32_t& xIndex, const uint32_t& yIndex, const Lane& laneNum);
	// 敵が壁に当たった時（とりあえずすぐに消す）
	void HitEnemyToWall(const MapChipType type, const uint32_t& xIndex, const uint32_t& yIndex, const Lane& laneNum);

	void ResetBlockScale();

	// エリア切り替え
	void ChangeMapArea();
	std::list<std::unique_ptr<BaseItem>> GenerateNewAreaItems();


	void SetItems(std::shared_ptr<std::list<std::unique_ptr<BaseItem>>> itemList) { items = itemList; }

	void ItemReset();

private:
	// リセット
	void Reset();
private:
	// マップデータ
	static const uint32_t maxMapArea = 10;
	std::array< MapChipData, maxMapArea> mapDataContainer;
	std::array<MapChipData, Lane::kNumLane> mapData;

	// ブロック
	std::unique_ptr<Block> blocks;
	std::unique_ptr<Block> boards;

	// 全マップの合計数
	uint32_t blockNumInstance = 0;
	uint32_t boardNumInstance = 0;

	// ボード表示用オブジェクト
	std::unique_ptr<Board> boardsModel;
	Scale boardScale = { 0.2f,0.2f,0.2f };

	// アイテム
	std::shared_ptr<std::list<std::unique_ptr<BaseItem>>> items;

	// 現在のマップエリア
	std::array<uint32_t, Lane::kNumLane> currentMapAreaLane;
	//uint32_t currentMapArea = 0;
	float collisionOffset = 1.0f;

	// 現在のステージ
	uint32_t currentStage = 0;

	// 1ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;
	// フィールドオフセット
	static inline float kLaneOffset = 5.0f;
	// ブロックの個数
	static inline const uint32_t kNumBlockVertical = 20;
	static inline const uint32_t kNumBlockHorizontal = 40;

	static uint32_t blockOffsetX;
};

