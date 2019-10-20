#pragma once

#include <string>
#include "Math.h"

namespace FMOD {
	namespace Studio {
		class EventInstance;
	}
}

class SoundEvent {
public:
	SoundEvent();

	bool IsValid();
	void Restart();
	void Stop(bool allowFadeOut = true);

	bool GetPaused() const;
	void SetPaused(bool pause);

	float GetVolume() const;
	void SetVolume(float volume);

	float GetPitch() const;
	void SetPitch(float pitch);

	float GetParameter(const std::string& name);
	void SetParameter(const std::string& name, float value);

	bool Is3D() const;
	void Set3DAttributes(const Matrix4& worldTransform);

protected:
	friend class AudioSystem;
	SoundEvent(class AudioSystem* inSystem, unsigned int inId);

private:
	FMOD::Studio::EventInstance* GetEvent() const;

	class AudioSystem* system;
	unsigned int id;
};

