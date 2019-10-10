#pragma once

class Component {
public:
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();

	virtual void ProcessInput(const uint8_t* keyState) {}
	virtual void Update(float deltaTime) {}

	inline int GetUpdateOrder() const { return updateOrder; }

protected:
	class Actor* owner;
	int updateOrder;
};