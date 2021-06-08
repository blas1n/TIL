#include "SoundManager.h"
#include <cstdio>
#include <mmsystem.h>
#include <dsound.h>
#include <vector>

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

namespace
{
	struct WaveHeader final
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};
}

bool SoundManager::Initialize(HWND hWnd)
{
	bool result = InitializeDirectSound(hWnd);
	if (!result) return false;

	result = LoadWaveFile(&secondaryBuffer1, TEXT("Asset/sound01.wav"));
	if (!result) return false;

	return PlayWaveFile();
}

void SoundManager::Release() noexcept
{
	ReleaseWaveFile(&secondaryBuffer1);
	ReleaseDirectSound();
}

bool SoundManager::InitializeDirectSound(HWND hWnd)
{
	HRESULT result = DirectSoundCreate8(nullptr, &directSound, nullptr);
	if (FAILED(result)) return false;

	result = directSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
	if (FAILED(result)) return false;

	DSBUFFERDESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(DSBUFFERDESC));

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	result = directSound->CreateSoundBuffer(&bufferDesc, &primaryBuffer, nullptr);
	if (FAILED(result)) return false;

	WAVEFORMATEX waveFormat;
	memset(&waveFormat, 0, sizeof(WAVEFORMATEX));

	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	
	result = primaryBuffer->SetFormat(&waveFormat);
	if (FAILED(result)) return false;

	return true;
}

void SoundManager::ReleaseDirectSound() noexcept
{
	if (primaryBuffer)
	{
		primaryBuffer->Release();
		primaryBuffer = nullptr;
	}

	if (directSound)
	{
		directSound->Release();
		directSound = nullptr;
	}
}

bool SoundManager::LoadWaveFile(IDirectSoundBuffer8** secondaryBuffer, const std::filesystem::path& soundPath)
{
	FILE* file;
	int error = fopen_s(&file, soundPath.string().c_str(), "rb");
	if (error) return false;

	WaveHeader waveHeader;
	size_t count = fread(&waveHeader, sizeof(WaveHeader), 1, file);
	if (count != 1) return false;

	if (strncmp(waveHeader.chunkId, "RIFF", 4) || strncmp(waveHeader.format, "WAVE", 4) ||
		strncmp(waveHeader.subChunkId, "fmt ", 4) || waveHeader.audioFormat != WAVE_FORMAT_PCM ||
		waveHeader.numChannels != 2 || waveHeader.sampleRate != 44100 || waveHeader.bitsPerSample != 16)
	{
		return false;
	}

	WAVEFORMATEX waveFormat;
	memset(&waveFormat, 0, sizeof(WAVEFORMATEX));

	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;

	DSBUFFERDESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(DSBUFFERDESC));

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveHeader.dataSize;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;
	
	IDirectSoundBuffer* tempBuffer;
	HRESULT result = directSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, nullptr);
	if (FAILED(result)) return false;

	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, reinterpret_cast<void**>(secondaryBuffer));
	if (FAILED(result)) return false;

	tempBuffer->Release();

	fseek(file, sizeof(WaveHeader), SEEK_SET);

	std::vector<unsigned char> waveData(waveHeader.dataSize);
	
	count = fread(waveData.data(), 1, waveHeader.dataSize, file);
	if (count != waveHeader.dataSize) return false;
	
	error = fclose(file);
	if (error) return false;

	unsigned char* bufferPtr = nullptr;
	unsigned long bufferSize = 0;
	result = (*secondaryBuffer)->Lock(0, waveHeader.dataSize,
		reinterpret_cast<void**>(&bufferPtr), &bufferSize, nullptr, nullptr , 0);

	if (FAILED(result)) return false;

	memcpy(bufferPtr, waveData.data(), waveHeader.dataSize);

	result = (*secondaryBuffer)->Unlock(bufferPtr, bufferSize, nullptr, 0);
	if (FAILED(result)) return false;

	return true;
}

void SoundManager::ReleaseWaveFile(IDirectSoundBuffer8** secondaryBuffer) noexcept
{
	if (*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = nullptr;
	}
}

bool SoundManager::PlayWaveFile()
{
	HRESULT result = secondaryBuffer1->SetCurrentPosition(0);
	if (FAILED(result)) return false;

	result = secondaryBuffer1->SetVolume(DSBVOLUME_MAX);
	if (FAILED(result)) return false;

	result = secondaryBuffer1->Play(0, 0, 0);
	return SUCCEEDED(result);
}
