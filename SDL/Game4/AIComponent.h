#pragma once

#include "Component.h"
#include <unordered_map>
#include <string>

class AIComponent : public Component {
public:
	AIComponent(class Actor* inOwner);
	
	void Update(float deltaTime) override;
	void ChangeState(const std::string& name);

	void RegisterState(class AIState* state);

private:
	std::unordered_map<std::string, class AIState*> mStateMap;
	class AIState* mCurrentState;
};
