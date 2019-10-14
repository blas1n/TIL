#pragma once

#include <cstdint>

class Component {
public:
	Component(class Actor* inOwner, int inUpdateOrder = 100);
	virtual ~Component();

	virtual void ProcessInput(const struct InputState& inputState) {}
	virtual void Update(float deltaTime) {}
	virtual void OnUpdateWorldTransform() {}

	inline int GetUpdateOrder() const { return updateOrder; }

protected:
	class Actor* owner;
	int updateOrder;
};