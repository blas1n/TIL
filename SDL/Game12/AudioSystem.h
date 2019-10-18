#pragma once

#include <unordered_map>
#include <string>
#include "SoundEvent.h"
#include "Math.h"

namespace FMOD {
	class System;
	
	namespace Studio {
		class System;
		class Bank;
		class EventDescription;
		class EventInstance;
		class Bus;
	}
}

class AudioSystem {
public:
	AudioSystem(class Game* inGame);
	
	bool Initialize();
	void Update(float deltaTime);
	void Shutdown();

	SoundEvent PlayEvent(const std::string& name);

	void LoadBank(const std::string& name);
	void UnloadBank(const std::string& name);
	void UnloadAllBanks();

	void SetListener(const Matrix4& viewMatrix);

	bool GetBusPaused(const std::string& name) const;
	void SetBusPaused(const std::string& name, bool pause);

	float GetBusVolume(const std::string& name) const;
	void SetBusVolume(const std::string& name, float volume);

protected:
	friend class SoundEvent;
	FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);

private:
	constexpr static int MaxPathLength = 512;
	static unsigned int nextId;

	class Game* game;
	class FMOD::Studio::System* system;
	class FMOD::System* lowLevelSystem;

	std::unordered_map<std::string, FMOD::Studio::Bus*> buses;
	std::unordered_map<std::string, FMOD::Studio::Bank*> banks;
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> events;
	std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> eventInstances;
};