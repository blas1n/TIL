#include "AudioComponent.h"
#include "Actor.h"
#include "Game.h"
#include "AudioSystem.h"

AudioComponent::AudioComponent(Actor* inOwner, const int inUpdateOrder /*= 200*/)
	: Component(inOwner, inUpdateOrder), events2D(), events3D() {}

AudioComponent::~AudioComponent() {
	StopAllEvent();
}

void AudioComponent::Update(const float deltaTime) {
	Component::Update(deltaTime);

	const static auto eventCollect = [](auto& events) {
		for (auto iter = events.begin(); iter != events.end();) {
			if (!iter->IsValid())
				iter = events.erase(iter);
			else
				++iter;
		}
	};

	eventCollect(events2D);
	eventCollect(events3D);
}

void AudioComponent::OnUpdateWorldTransform() {
	const auto transform = owner->GetWorldTransform();

	for (auto& event : events3D)
		if (event.IsValid())
			event.Set3DAttributes(transform);
}

SoundEvent AudioComponent::PlayEvent(const std::string& name) {
	auto event = owner->GetGame()->GetAudioSystem()->PlayEvent(name);
	const auto is3D = event.Is3D();

	auto& events = is3D ? events3D : events2D;
	events.emplace_back(event);

	if (is3D) event.Set3DAttributes(owner->GetWorldTransform());
	return event;
}

void AudioComponent::StopAllEvent() {
	const static auto stopEvent = [](auto& events) {
		for (auto& event : events)
			event.Stop();
		
		events.clear();
	};

	stopEvent(events2D);
	stopEvent(events3D);
}