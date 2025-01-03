#include "PrecompiledHeader.h"
#include "AudioManager.h"
#include <thread>
#include <chrono>
#include <assert.h>

void AudioManager::Initialize()
{
	HRESULT result;
	/*xAudioエンジンのインスタンスを生成*/
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));
	/*マスターボイスを生成*/
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result));
}

AudioManager::SoundData AudioManager::SoundLordWave(const char* filename)
{
	/*ファイルオープン*/
	/*ファイル入力ストリームのインスタンス*/
	std::ifstream file;
	/*.wavファイルをバイナリモードで開く*/
	file.open(filename, std::ios_base::binary);
	/*ファイルオープン失敗を検出する*/
	assert(file.is_open());

	/*.wavデータ読み込み*/
	/*RIFFヘッダーの読み込み*/
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	/*ファイルがRIFFかチェック*/
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	/*タイプがWAVEかチェック*/
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}
	/*Formatチャンクの読み込み*/
	FormatChunk format = {};
	/*チャンクヘッダーの確認*/
	// fmt チャンクを探す
	while (true) {
		file.read(reinterpret_cast<char*>(&format.chunk), sizeof(ChunkHeader));
		if (file.eof()) {
			assert(0); // チャンクが見つからなかった場合
		}
		if (strncmp(format.chunk.id, "fmt ", 4) == 0) {
			break; // fmt チャンクを見つけた
		}
		// 他のチャンクの場合、サイズ分だけ読み飛ばす
		file.seekg(format.chunk.size, std::ios_base::cur);
	}
	/*チャンク本体の読み込み*/
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);
	/*Dataチャンクの読み込み*/
	ChunkHeader data;
	/*dataを読み込み、data チャンクを見つけるまでループする*/
	while (true) {
		file.read(reinterpret_cast<char*>(&data), sizeof(data));
		if (file.eof()) {
			//throw std::runtime_error("No data chunk found in WAV file.");
			assert(0);
		}
		if (strncmp(data.id, "data", 4) == 0) {
			break;
		}
		/*目的のチャンク (data チャンク) を見つけるまで、サイズ分だけ読み飛ばす*/
		file.seekg(data.size, std::ios_base::cur);
	}
	/*Dataチャンクのデータ部（波形データ）の読み込み*/
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	/*ファイルクローズ*/
	/*Waveファイルを閉じる*/
	file.close();
	/*読み込んだ音声データをreturn*/
	/*returnするための音声データ*/
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	return soundData;
}

void AudioManager::SoundUnLord(SoundData* soundData)
{
	/*バッファのメモリを解放*/
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
	soundData->pSourceVoice->DestroyVoice();
	soundData->pSourceVoice = nullptr;
}

void AudioManager::SoundPlayWave(IXAudio2* xaudio2, SoundData& soundData, bool loop)
{
	HRESULT result;
	if (!soundData.pSourceVoice) {
		result = xaudio2->CreateSourceVoice(&soundData.pSourceVoice, &soundData.wfex);
		assert(SUCCEEDED(result));
	}

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	if (loop) {
		buf.LoopBegin = 0;
		buf.LoopLength = 0; // 無限ループ設定
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	soundData.pSourceVoice->FlushSourceBuffers();
	result = soundData.pSourceVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(result));

	result = soundData.pSourceVoice->Start();
	assert(SUCCEEDED(result));
}

void AudioManager::SoundStop(SoundData& soundData)
{
	if (soundData.pSourceVoice) {
		soundData.pSourceVoice->Stop(0);
		soundData.pSourceVoice->FlushSourceBuffers();
	}
}

void AudioManager::SoundStopFadeOut(SoundData& soundData, float duration)
{
	if (!soundData.pSourceVoice) {
		return;
	}

	// 現在のボリュームを取得
	float currentVolume = soundData.currentVolume;

	// フェードアウトのステップ設定
	const int steps = 50; // フェードアウトを50ステップに分割
	const float stepDuration = duration / steps; // 1ステップの時間
	const float volumeDecrement = currentVolume / steps; // 1ステップで減少するボリューム

	for (int i = 0; i < steps; ++i) {
		currentVolume -= volumeDecrement; // ボリュームを減らす
		if (currentVolume < 0.0f) {
			currentVolume = 0.0f;
		}

		// ボリュームを設定
		SetVolume(soundData, currentVolume);

		// スリープして次のステップに進む
		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(stepDuration * 1000)));
	}

	// 音声を停止
	SoundStop(soundData);
}

void AudioManager::Finalize()
{
	/*XAudio2解放*/
	xAudio2.Reset();
}

void AudioManager::SetVolume(SoundData& soundData, float volume)
{
	if (soundData.pSourceVoice) {
		// ボリュームを設定する
		HRESULT result = soundData.pSourceVoice->SetVolume(volume);
		assert(SUCCEEDED(result)); // ボリューム設定が失敗した場合にアサート

		// 現在のボリューム値を保存
		soundData.currentVolume = volume;
	}
}
