#pragma once

#include "Component.h"
#include <vector>
#include "SoundEvent.h"

class AudioComponent : public Component {
public:
	AudioComponent(class Actor* inOwner, int inUpdateOrder = 200);
	~AudioComponent();

	void Update(float deltaTime) override;
	void OnUpdateWorldTransform() override;

	SoundEvent PlayEvent(const std::string& name);
	void StopAllEvent();

private:
	std::vector<SoundEvent> events2D;
	std::vector<SoundEvent> events3D;
};

