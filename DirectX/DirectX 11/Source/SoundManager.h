#pragma once

#include "stdafx.h"
#include <filesystem>

class SoundManager final
{
public:
	bool Initialize(HWND hWnd);
	void Release() noexcept;

private:
	bool InitializeDirectSound(HWND hWnd);
	void ReleaseDirectSound() noexcept;

	bool LoadWaveFile(struct IDirectSoundBuffer8** buffer, const std::filesystem::path& soundPath);
	void ReleaseWaveFile(IDirectSoundBuffer8** buffer) noexcept;

	bool PlayWaveFile();

private:
	struct IDirectSound8* directSound;
	struct IDirectSoundBuffer* primaryBuffer;
	IDirectSoundBuffer8* secondaryBuffer1;
};
