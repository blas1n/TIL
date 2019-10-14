#include "SoundEvent.h"
#include <fmod_studio.hpp>
#include "AudioSystem.h"

SoundEvent::SoundEvent()
	: system(nullptr), id(0) {}

SoundEvent::SoundEvent(AudioSystem* inSystem, unsigned int inId)
	: system(inSystem), id(inId) {}

bool SoundEvent::IsValid() {
	return GetEvent() != nullptr;
}

void SoundEvent::Restart() {
	const auto event = GetEvent();
	if (event) event->start();
}

void SoundEvent::Stop(bool allowFadeOut /*= true*/) {
	const auto event = GetEvent();
	if (!event) return;

	const auto mode = allowFadeOut ?
		FMOD_STUDIO_STOP_ALLOWFADEOUT :
		FMOD_STUDIO_STOP_IMMEDIATE;

	event->stop(mode);
}

bool SoundEvent::GetPaused() const {
	bool ret = false;
	const auto event = GetEvent();

	if (event)
		event->getPaused(&ret);

	return ret;
}

void SoundEvent::SetPaused(bool pause) {
	const auto event = GetEvent();
	if (event)
		event->setPaused(pause);
}

float SoundEvent::GetVolume() const {
	float ret = 0.0f;
	const auto event = GetEvent();

	if (event)
		event->getVolume(&ret);

	return ret;
}

void SoundEvent::SetVolume(float volume) {
	const auto event = GetEvent();
	if (event)
		event->setVolume(volume);
}

float SoundEvent::GetPitch() const {
	float ret = 0.0f;
	const auto event = GetEvent();

	if (event)
		event->getPitch(&ret);

	return ret;
}

void SoundEvent::SetPitch(float pitch) {
	const auto event = GetEvent();
	if (event)
		event->setPitch(pitch);
}

float SoundEvent::GetParameter(const std::string& name) {
	float ret = 0.0f;
	const auto event = GetEvent();

	if (event)
		event->getParameterValue(name.c_str(), &ret);

	return ret;
}

void SoundEvent::SetParameter(const std::string& name, float value) {
	const auto event = GetEvent();
	if (event)
		event->setParameterValue(name.c_str(), value);
}

FMOD::Studio::EventInstance* SoundEvent::GetEvent() const {
	return system ? system->GetEventInstance(id) : nullptr;
}

bool SoundEvent::Is3D() const {
	bool ret = false;
	const auto event = GetEvent();
	if (event) {
		FMOD::Studio::EventDescription* ed = nullptr;
		event->getDescription(&ed);

		if (ed) ed->is3D(&ret);
	}

	return ret;
}

void SoundEvent::Set3DAttributes(const Matrix4& worldTransform) {
	const static auto VecToFMOD = [](const auto& in) {
		return FMOD_VECTOR{ in.y, in.z, in.x };
	};

	const auto event = GetEvent();
	if (!event) return;

	FMOD_3D_ATTRIBUTES attr;
	attr.position = VecToFMOD(worldTransform.GetTranslation());
	attr.forward = VecToFMOD(worldTransform.GetXAxis());
	attr.up = VecToFMOD(worldTransform.GetZAxis());
	attr.velocity = { 0.0f, 0.0f, 0.0f };
	event->set3DAttributes(&attr);
}