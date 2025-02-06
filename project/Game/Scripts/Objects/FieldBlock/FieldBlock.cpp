#include "PrecompiledHeader.h"
#include "FieldBlock.h"
#include <assert.h>
#include "RandomGenerator/RandomGenerator.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

uint32_t FieldBlock::blockOffsetX = 0;

namespace {
	std::map<std::string, MapChipType> mapChipTable = {
		{"0", MapChipType::kBlank},// 空白
		{"1", MapChipType::kBlock},// 地面ブロック
		{"2", MapChipType::kPlayer},// プレイヤー
		{"3", MapChipType::kEnemy},// 敵
		{"4", MapChipType::kHP},// HPアイテム
		{"5", MapChipType::kPower},// Powerアイテム
		{"6", MapChipType::kSpeed},// Speedアイテム
		{"7", MapChipType::kBoard},// ブロック
	};
}

void FieldBlock::Initialize()
{
	// ステージデータ読み込み
	LoadMapData("Game/Assets/StageData/Stage0");
	GenerateFieldBlock();
	GenerateItems();
}

void FieldBlock::Update()
{
#ifdef _DEBUG
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking;
	ImGui::Begin("FieldBlock", nullptr, flags);
	ImGui::SliderFloat("LaneOffset", &kLaneOffset, 0.0f, 10.0f);
	if (ImGui::Button("Reset"))
	{
		Initialize();
	}
	ImGui::End();
#endif // _DEBUG

	blocks->Update();
}

void FieldBlock::SetPtr(SceneManager* sceneManager, ComponentManager* compManager, InputManager* inputManager)
{
	sceneManager_ = sceneManager;
	compManager_ = compManager;
	input = inputManager;
}

void FieldBlock::LoadMapData(const std::string& directoryPath)
{
	// リセット
	Reset();

	// 指定されたディレクトリ内の全CSVファイルを取得
	uint32_t areaNum = 0;
	for (const auto& entry : fs::directory_iterator(directoryPath)) {
		if (entry.is_regular_file() && entry.path().extension() == ".csv") {
			// ファイルパス取得
			const auto& filePath = entry.path();

			// マップデータ読み込み
			std::ifstream file(filePath);
			assert(file.is_open());

			// マップチップCSV
			std::stringstream mapChipCsv;
			mapChipCsv << file.rdbuf();
			file.close();

			std::string line;
			// CSVからマップチップデータを読み込む
			for (uint32_t i = 0; i < kNumBlockVertical; ++i) {
				if (!std::getline(mapChipCsv, line)) {
					break;
				}

				std::istringstream line_stream(line);
				for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
					std::string word;
					if (!std::getline(line_stream, word, ',')) {
						break;
					}

					if (mapChipTable.contains(word)) {
						// 各レーンのデータを埋める（仮定としてkLeftLaneを利用）
						//MapChipData mapChipData;
						if (areaNum == 0) {
							mapData[0].data[i][j] = mapChipTable[word];
						}
						else if (areaNum == 1) {
							mapData[1].data[i][j] = mapChipTable[word];
						}
						else if (areaNum == 2) {
							mapData[2].data[i][j] = mapChipTable[word];
						}
						mapDataContainer[areaNum].data[i][j] = mapChipTable[word];
					}
				}
			}
		}
		areaNum++;
	}
}

void FieldBlock::GenerateFieldBlock()
{
	// 要素数
	uint32_t numBlockVertical = GetNumBlockVertical();
	uint32_t numBlockHorizontal = GetNumBlockHorizontal();

	for (uint32_t l = 0; l < Lane::kNumLane; ++l) {
		for (uint32_t a = 0;a < maxMapArea;++a) {
			for (uint32_t i = 0; i < numBlockVertical; ++i) {
				for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
					if (GetMapChipTypeByIndex(j, i,static_cast<Lane>(l)) ==MapChipType::kBlock) {
						blockNumInstance++;
					}
					if (GetMapChipTypeByIndex(j, i, static_cast<Lane>(l)) == MapChipType::kBoard&&i==18) {
						boardNumInstance++;
					}
				}
			}
		}
	}

	for (uint32_t i = 0;i < Lane::kNumLane;++i) {
		currentMapAreaLane[i] = Random::get(0, 9);
		mapData[i].data = mapDataContainer[currentMapAreaLane[i]].data;
	}

	// ブロックの生成
	blocks = std::make_unique<Block>();
	blocks->Create("blocks", sceneManager_, compManager_, input, true, blockNumInstance);
	blocks->ChangeVisible(false);// ブロックは非表示

	boards = std::make_unique<Block>();
	boards->Create("boards", sceneManager_, compManager_, input, true, boardNumInstance);
	boards->ChangeVisible(false);// ボードは非表示

	boardsModel = std::make_unique<Board>();
	boardsModel->Create("boardsModel", sceneManager_, compManager_, input, true, 60);
	boardsModel->Initialize();

	std::vector<Vector3> posContainer;

	uint32_t blockIndex = 0;
	uint32_t boardIndex = 0;
	for (uint32_t l = 0; l < Lane::kNumLane; ++l) {
		//for (uint32_t a = 0;a < maxMapArea;++a) {
			//uint32_t blockOffsetX = a * numBlockHorizontal;
			for (uint32_t i = 0; i < numBlockVertical; ++i) {
				for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
					if (GetMapChipTypeByIndex(j, i,static_cast<Lane>(l)) == MapChipType::kBlock) {
						Vector3 pos = GetMapChipPositionByIndex(j, i, static_cast<Lane>(l));
						//pos.x += blockOffsetX;
						blocks->Initialize();
						blocks->SetPosition(pos, blockIndex);
						blocks->SetScale({ 1.0f, 1.0f, 1.0f }, blockIndex);
						blockIndex++;
					}
					if (GetMapChipTypeByIndex(j, i, static_cast<Lane>(l)) == MapChipType::kBoard && i == 18) {
						Vector3 pos = GetMapChipPositionByIndex(j, i, static_cast<Lane>(l));
						//pos.x += blockOffsetX;
						boards->Initialize();
						boards->SetPosition(pos, boardIndex);
						boards->SetScale(boardScale, boardIndex);
						boards->SetLighting(1);

						posContainer.push_back(pos);

						boardIndex++;
					}
				}
			}
		//}
	}
	boardsModel->SetPos(posContainer);
}

void FieldBlock::GenerateItems()
{
	// 要素数
	uint32_t numBlockVertical = GetNumBlockVertical();
	uint32_t numBlockHorizontal = GetNumBlockHorizontal();

	for (uint32_t l = 0; l < Lane::kNumLane; ++l) {
		for (uint32_t i = 0; i < numBlockVertical; ++i) {
			for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
				if (GetMapChipTypeByIndex(j, i, static_cast<Lane>(l)) == MapChipType::kHP) {
					Vector3 pos = GetMapChipPositionByIndex(j, i, static_cast<Lane>(l));
					std::unique_ptr<HealingItem> item = std::make_unique<HealingItem>();
					item->Create("item", sceneManager_, compManager_, input);
					item->Initialize();
					item->SetPosition(pos);
					items->push_back(std::move(item));
				}
				else if (GetMapChipTypeByIndex(j, i, static_cast<Lane>(l)) == MapChipType::kPower) {
					Vector3 pos = GetMapChipPositionByIndex(j, i, static_cast<Lane>(l));
					std::unique_ptr<PowerUpItem> item = std::make_unique<PowerUpItem>();
					item->Create("item", sceneManager_, compManager_, input);
					item->Initialize();
					item->SetPosition(pos);
					items->push_back(std::move(item));
				}
				else if (GetMapChipTypeByIndex(j, i, static_cast<Lane>(l)) == MapChipType::kSpeed) {
					Vector3 pos = GetMapChipPositionByIndex(j, i, static_cast<Lane>(l));
					std::unique_ptr<SpeedUpItem> item = std::make_unique<SpeedUpItem>();
					item->Create("item", sceneManager_, compManager_, input);
					item->Initialize();
					item->SetPosition(pos);
					items->push_back(std::move(item));
				}
			}
		}
	}
}

MapChipType FieldBlock::GetMapChipTypeByIndex(const uint32_t& xIndex, const uint32_t& yIndex, const Lane& laneNum)
{
	int32_t x = xIndex;
	if (xIndex < 0 || (kNumBlockHorizontal*collisionOffset) - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVertical - 1 < yIndex) {
		return MapChipType::kBlank;
	}
	if (xIndex >= kNumBlockHorizontal) {
		x = (kNumBlockHorizontal * static_cast<uint32_t>(collisionOffset)) - xIndex;
		x = std::abs(x);
		x = kNumBlockHorizontal - x;
	}
	if (x >= kNumBlockHorizontal) {
		return MapChipType::kBlank;
	}
	//return mapDataContainer[currentMapArea].data[yIndex][xIndex];
	return mapData[laneNum].data[yIndex][x];
}

Vector3 FieldBlock::GetMapChipPositionByIndex(const uint32_t& xIndex, const uint32_t& yIndex, const Lane& laneNum)
{
	Vector3 result = {};
	switch (laneNum)
	{
	case Lane::kLeftLane:
		// 奥
		result = Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVertical - 1 - yIndex), (kBlockWidth+kLaneOffset));
		break;
	case Lane::kCenterLane:
		// 中央
		result = Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVertical - 1 - yIndex), 0);
		break;
	case Lane::kRightLane:
		// 手前
		result = Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVertical - 1 - yIndex), -(kBlockWidth + kLaneOffset));
		break;
	case Lane::kNumLane:
	default:
		break;
	}
	return result;
}

float FieldBlock::GetMapChipPositionYByIndex(const uint32_t& yIndex)
{
	return kBlockHeight * (kNumBlockVertical - 1 - yIndex);
}

FieldBlock::IndexSet FieldBlock::GetMapChipIndexSetByPosition(const Vector3& position)
{
	IndexSet inDexSet = {};
	inDexSet.xIndex = uint32_t((position.x + kBlockWidth / 2) / kBlockWidth);
	inDexSet.yIndex = uint32_t((position.y + kBlockHeight / 2) / kBlockHeight);
	inDexSet.yIndex = kNumBlockVertical - 1 - inDexSet.yIndex;
	return inDexSet;
}

FieldBlock::Rect FieldBlock::GetRectByIndex(const uint32_t& xIndex, const uint32_t& yIndex, const Lane& laneNum) {
	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex,laneNum);
	// 指定ブロックの中心座標を取得する
	Rect rect;
	rect.left = center.x - kBlockWidth / 2.0f;
	rect.right = center.x + kBlockWidth / 2.0f;
	rect.bottom = center.y - kBlockHeight / 2.0f;
	rect.top = center.y + kBlockHeight / 2.0f;
	return rect;
}

float FieldBlock::GetLaneOffset(const Lane& laneNum)
{
	float result = 0.0f;
	switch (laneNum)
	{
	case Lane::kLeftLane:
		result = (kBlockWidth+kLaneOffset);
		break;
	case Lane::kCenterLane:
		result = 0.0f;
		break;
	case Lane::kRightLane:
		result = -(kBlockWidth + kLaneOffset);
		break;
	case Lane::kNumLane:
	default:
		break;
	}
	return result;
}

std::optional<uint32_t> FieldBlock::GetTransformsIndexByMapChip(const MapChipType type, const uint32_t& xIndex, const uint32_t& yIndex, const Lane& laneNum)
{
	uint32_t numBlockVertical = GetNumBlockVertical();
	uint32_t numBlockHorizontal = GetNumBlockHorizontal();

	uint32_t index = 0;
	uint32_t x = xIndex - numBlockHorizontal * static_cast<int>(collisionOffset - 1.0f);
	for (uint32_t l = 0; l < Lane::kNumLane; ++l) {
		for (uint32_t i = 0; i < numBlockVertical; ++i) {
			for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
				if (GetMapChipTypeByIndex(j, i, static_cast<Lane>(l)) == type && i == 18) {
					if (j == x && i == yIndex && static_cast<Lane>(l) == laneNum) {
						return index;
					}
					index++;
				}
			}
		}
	}
	return std::nullopt; // 見つからなかった場合
}

void FieldBlock::HitEnemyToWall(const MapChipType type, const uint32_t& xIndex, const uint32_t& yIndex, const Lane& laneNum)
{
	if (!blocks && !boards) {
		return;
	}

	auto indexOpt = GetTransformsIndexByMapChip(type, xIndex, yIndex, laneNum);
	if (indexOpt) {
		if(type == MapChipType::kBlock)
		{
			blocks->SetScale({ 0.0f, 0.0f, 0.0f }, *indexOpt);
		}

		if (type == MapChipType::kBoard) {
			boardsModel->SetScale({ 0.0f, 0.0f, 0.0f }, *indexOpt);
		}
	}
}

void FieldBlock::ResetBlockScale()
{
	blocks->ScaleReset();
}

void FieldBlock::ChangeMapArea()
{
	// 要素数
	uint32_t numBlockVertical = GetNumBlockVertical();
	uint32_t numBlockHorizontal = GetNumBlockHorizontal();

	for (uint32_t i = 0;i < Lane::kNumLane;++i) {
		currentMapAreaLane[i] = Random::get(0, 9);
		mapData[i].data = mapDataContainer[currentMapAreaLane[i]].data;
	}
	collisionOffset += 1.0f;

	std::vector<Vector3> posContainer;
	uint32_t blockIndex = 0;
	uint32_t boardIndex = 0;
	blockOffsetX += numBlockHorizontal;
	for (uint32_t l = 0; l < Lane::kNumLane; ++l) {
		for (uint32_t i = 0; i < numBlockVertical; ++i) {
			for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
				if (GetMapChipTypeByIndex(j, i, static_cast<Lane>(l)) == MapChipType::kBlock) {
					Vector3 pos = GetMapChipPositionByIndex(j, i, static_cast<Lane>(l));
					pos.x += blockOffsetX;
					blocks->Initialize();
					blocks->SetPosition(pos, blockIndex);
					blocks->SetScale({ 1.0f, 1.0f, 1.0f }, blockIndex);
					blockIndex++;
				}
				if (GetMapChipTypeByIndex(j, i, static_cast<Lane>(l)) == MapChipType::kBoard && i == 18) {
					Vector3 pos = GetMapChipPositionByIndex(j, i, static_cast<Lane>(l));
					pos.x += blockOffsetX;
					boards->Initialize();
					boards->SetPosition(pos, boardIndex);
					boards->SetScale(boardScale, boardIndex);
					boards->SetLighting(1);
					posContainer.push_back(pos);
					boardIndex++;
				}
			}
		}
	}
	boardsModel->SetPos(posContainer);
}

std::list<std::unique_ptr<BaseItem>> FieldBlock::GenerateNewAreaItems()
{
	std::list<std::unique_ptr<BaseItem>> newItems;

	// 要素数
	uint32_t numBlockVertical = GetNumBlockVertical();
	uint32_t numBlockHorizontal = GetNumBlockHorizontal();

	for (uint32_t l = 0; l < Lane::kNumLane; ++l) {
		for (uint32_t i = 0; i < numBlockVertical; ++i) {
			for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
				if (GetMapChipTypeByIndex(j, i, static_cast<Lane>(l)) == MapChipType::kHP) {
					Vector3 pos = GetMapChipPositionByIndex(j, i, static_cast<Lane>(l));
					pos.x += blockOffsetX;
					std::unique_ptr<HealingItem> item = std::make_unique<HealingItem>();
					item->Create("item", sceneManager_, compManager_, input);
					item->Initialize();
					item->SetPosition(pos);
					newItems.push_back(std::move(item));
				}
				else if (GetMapChipTypeByIndex(j, i, static_cast<Lane>(l)) == MapChipType::kPower) {
					Vector3 pos = GetMapChipPositionByIndex(j, i, static_cast<Lane>(l));
					pos.x += blockOffsetX;
					std::unique_ptr<PowerUpItem> item = std::make_unique<PowerUpItem>();
					item->Create("item", sceneManager_, compManager_, input);
					item->Initialize();
					item->SetPosition(pos);
					newItems.push_back(std::move(item));
				}
				else if (GetMapChipTypeByIndex(j, i, static_cast<Lane>(l)) == MapChipType::kSpeed) {
					Vector3 pos = GetMapChipPositionByIndex(j, i, static_cast<Lane>(l));
					pos.x += blockOffsetX;
					std::unique_ptr<SpeedUpItem> item = std::make_unique<SpeedUpItem>();
					item->Create("item", sceneManager_, compManager_, input);
					item->Initialize();
					item->SetPosition(pos);
					newItems.push_back(std::move(item));
				}
			}
		}
	}
	return newItems; // ムーブされる
}

void FieldBlock::ItemReset()
{
	for (auto&& item : *items) {
		item->Reset();
	}
}

void FieldBlock::Reset()
{
	// マップチップデータをリセット
	for (auto& map : mapData) {
		map.data.clear();
		map.data.resize(kNumBlockVertical);
		for (std::vector<MapChipType>& mapChipDataLine : map.data) {
			mapChipDataLine.resize(kNumBlockHorizontal);
		}
	}
	for (auto& map : mapDataContainer) {
		map.data.clear();
		map.data.resize(kNumBlockVertical);
		for (std::vector<MapChipType>& mapChipDataLine : map.data) {
			mapChipDataLine.resize(kNumBlockHorizontal);
		}
	}
}

void Block::Initialize()
{
	//SetModel("Obstacles");
	SetModel("Cube");
}

void Block::Update()
{
	
}

void Block::SetPosition(const Vector3& position, const uint32_t& index)
{
	transform->transforms[index].translation = position;
}

void Block::SetScale(const Scale& scale, const uint32_t& index)
{
	transform->transforms[index].scale = scale;
}

void Block::ChangeVisible(bool flag)
{
	render->visible = flag;
}

void Block::ScaleReset()
{
	for (Transforms& trans : transform->transforms) {
		trans.scale = { 1.0f,1.0f,1.0f };
	}
}

Vector3 Block::GetPos(const uint32_t& index)
{
	return transform->transforms[index].translation;
}

