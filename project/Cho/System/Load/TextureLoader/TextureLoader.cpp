#include "TextureLoader.h"

#include"D3D12/ResourceViewManager/ResourceViewManager.h"
#include"D3D12/D3DCommand/D3DCommand.h"

// Utility
#include"ConvertString/ConvertString.h"

void TextureLoader::Initialize(D3DDevice* d3dDevice, D3DCommand* d3dCommand, ResourceViewManager* rvManager)
{
	d3dDevice_ = d3dDevice;
	d3dCommand_ = d3dCommand;
	rvManager_ = rvManager;
}

void TextureLoader::Load()
{
	// 後でフォルダの中のもの全て別のスレッドで読み込む
	const std::string& filePath = "Cho/Resources/Texture/monsterBall.png";
	// 読み込み済みテクスチャを検索
	if (textureData.contains(filePath)) {
		return;
	}

	// テクスチャデータを追加して書き込む
	Texture& texData = textureData[filePath];

	// 新たなハンドルを取得
	texData.rvIndex = rvManager_->GetNewHandle();

	HRESULT hr;

	// テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	if (filePathW.ends_with(L".dds")) {/*.ddsで終わっていたらddsとみなす。より安全な方法はいくらでもあるがいまのとここれ*/
		hr = DirectX::LoadFromDDSFile(filePathW.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
	} else {
		hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	}
	assert(SUCCEEDED(hr));

	// ミップマップの生成	
	DirectX::ScratchImage mipImages{};

	/*圧縮フォーマットかどうか調べる*/
	if (DirectX::IsCompressed(image.GetMetadata().format)) {
		/*圧縮フォーマットならそのまま使うのでmoveする*/
		mipImages = std::move(image);
	} else {
		// テクスチャが小さい場合はミップマップ生成をスキップ
		if (image.GetMetadata().width > 1 && image.GetMetadata().height > 1) {
			hr = DirectX::GenerateMipMaps(
				image.GetImages(),
				image.GetImageCount(),
				image.GetMetadata(),
				DirectX::TEX_FILTER_SRGB,
				0,
				mipImages
			);
		} else {
			// 小さいテクスチャの場合はミップマップを生成せず、そのまま移動
			mipImages = std::move(image);
		}
	}
	assert(SUCCEEDED(hr));

	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();

	// リソース作成
	d3dCommand_->Reset();

	rvManager_->CreateTextureResource(texData.rvIndex, metadata);
	rvManager_->UploadTextureDataEx(texData.rvIndex, mipImages);

	d3dCommand_->Close();
	d3dCommand_->Signal();

	texData.metadata = metadata;

	rvManager_->CreateSRVForTexture2D(texData.rvIndex, metadata.format, UINT(metadata.mipLevels));
}