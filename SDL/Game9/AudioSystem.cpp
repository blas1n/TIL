#include "AudioSystem.h"
#include <SDL/SDL_log.h>
#include <fmod_studio.hpp>
#include <fmod_errors.h>
#include <vector>

using namespace FMOD;

unsigned int AudioSystem::nextId = 0;


AudioSystem::AudioSystem(Game* inGame)
	: game(inGame),
	system(nullptr),
	lowLevelSystem(nullptr) {}

bool AudioSystem::Initialize() {
	Debug_Initialize(FMOD_DEBUG_LEVEL_ERROR, FMOD_DEBUG_MODE_TTY);

	auto result = Studio::System::create(&system);
	if (result != FMOD_OK) {
		SDL_Log("Failed to create FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}

	result = system->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK) {
		SDL_Log("Failed to create FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}
	
	system->getLowLevelSystem(&lowLevelSystem);

	LoadBank("Assets/Master Bank.strings.bank");
	LoadBank("Assets/Master Bank.bank");
	return true;
}

void AudioSystem::Update(const float deltaTime) {
	std::vector<unsigned int> done;

	for (auto& iter : eventInstances) {
		auto event = iter.second;
		FMOD_STUDIO_PLAYBACK_STATE state;
		event->getPlaybackState(&state);

		if (state == FMOD_STUDIO_PLAYBACK_STOPPED) {
			event->release();
			done.emplace_back(iter.first);
		}
	}

	for (auto id : done)
		eventInstances.erase(id);

	system->update();
}

void AudioSystem::Shutdown() {
	system->release();
}

SoundEvent AudioSystem::PlayEvent(const std::string& name) {
	auto iter = events.find(name);
	if (iter == events.cend())
		return SoundEvent(this, 0);

	Studio::EventInstance* event = nullptr;
	iter->second->createInstance(&event);

	if (!event) return SoundEvent(this, 0);

	event->start();
	eventInstances.emplace(++nextId, event);
	return SoundEvent(this, nextId);
}

void AudioSystem::LoadBank(const std::string& name) {
	if (banks.find(name) != banks.cend())
		return;

	Studio::Bank* bank = nullptr;
	const auto result = system->loadBankFile(
		name.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank
	);

	if (result != FMOD_OK) return;

	banks.emplace(name, bank);
	bank->loadSampleData();

	auto numEvents = 0;
	bank->getEventCount(&numEvents);

	if (numEvents > 0) {
		std::vector<Studio::EventDescription*> eventList(numEvents);
		bank->getEventList(eventList.data(), numEvents, &numEvents);
		char eventName[MaxPathLength]{ 0, };
	
		for (auto event : eventList) {
			event->getPath(eventName, MaxPathLength, nullptr);
			events.emplace(eventName, event);
		}
	}

	auto numBuses = 0;
	bank->getBusCount(&numBuses);

	if (numBuses > 0) {
		std::vector<FMOD::Studio::Bus*> busList(numBuses);
		bank->getBusList(busList.data(), numBuses, &numBuses);
		char busName[MaxPathLength]{ 0, };

		for (auto bus : busList) {
			bus->getPath(busName, MaxPathLength, nullptr);
			buses.emplace(busName, bus);
		}
	}
}

void AudioSystem::UnloadBank(const std::string& name) {
	const auto iter = banks.find(name);
	if (iter == banks.cend())
		return;

	auto bank = iter->second;

	auto numEvents = 0;
	bank->getEventCount(&numEvents);

	if (numEvents > 0) {
		std::vector<Studio::EventDescription*> eventList(numEvents);
		bank->getEventList(eventList.data(), numEvents, &numEvents);
		char eventName[MaxPathLength]{ 0, };

		for (auto event : eventList) {
			event->getPath(eventName, MaxPathLength, nullptr);

			const auto eventi = events.find(eventName);
			if (eventi != events.cend())
				events.erase(eventi);
		}
	}

	auto numBuses = 0;
	bank->getBusCount(&numBuses);

	if (numBuses > 0) {
		std::vector<Studio::Bus*> busList(numBuses);
		bank->getBusList(busList.data(), numBuses, &numBuses);
		char busName[MaxPathLength]{ 0, };

		for (auto bus : busList) {
			bus->getPath(busName, MaxPathLength, nullptr);

			const auto busi = buses.find(busName);
			if (busi != buses.cend())
				buses.erase(busi);
		}
	}

	bank->unloadSampleData();
	bank->unload();
	banks.erase(iter);
}

void AudioSystem::UnloadAllBanks() {
	for (auto& iter : banks) {
		iter.second->unloadSampleData();
		iter.second->unload();
	}

	banks.clear();
	events.clear();
}

void AudioSystem::SetListener(const Matrix4& viewMatrix) {
	const static auto VecToFMOD = [](const auto& in) {
		return FMOD_VECTOR{ in.y, in.z, in.x };
	};

	auto invView = viewMatrix;
	invView.Invert();

	FMOD_3D_ATTRIBUTES listener;
	listener.position = VecToFMOD(invView.GetTranslation());
	listener.forward = VecToFMOD(invView.GetZAxis());
	listener.up = VecToFMOD(invView.GetYAxis());
	listener.velocity = { 0.0f, 0.0f, 0.0f };

	system->setListenerAttributes(0, &listener);
}

bool AudioSystem::GetBusPaused(const std::string& name) const {
	bool ret = false;
	auto iter = buses.find(name);

	if (iter != buses.cend())
		iter->second->getPaused(&ret);

	return ret;
}

void AudioSystem::SetBusPaused(const std::string& name, bool pause) {
	auto iter = buses.find(name);
	if (iter != buses.cend())
		iter->second->setPaused(pause);
}

float AudioSystem::GetBusVolume(const std::string& name) const {
	float ret = false;
	auto iter = buses.find(name);

	if (iter != buses.cend())
		iter->second->getVolume(&ret);

	return ret;
}

void AudioSystem::SetBusVolume(const std::string& name, float volume) {
	auto iter = buses.find(name);
	if (iter != buses.cend())
		iter->second->setVolume(volume);
}

Studio::EventInstance* AudioSystem::GetEventInstance(const unsigned int id) {
	const auto iter = eventInstances.find(id);
	return iter != eventInstances.cend() ? iter->second : nullptr;
}