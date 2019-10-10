#pragma once

class AIState {
public:
	AIState(class AIComponent* inOwner)
		: owner(inOwner) {}

	virtual void Update(float deltaTime) = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

	virtual const char* GetName() const = 0;

protected:
	class AIComponent* owner;
};

class AIPatrol : public AIState {
public:
	AIPatrol(class AIComponent* inOwner)
		:AIState(inOwner) {}

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override { return "Patrol"; }
};

class AIDeath : public AIState {
public:
	AIDeath(class AIComponent* inOwner)
		: AIState(inOwner) {}

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override { return "Death"; }
};

class AIAttack : public AIState {
public:
	AIAttack(class AIComponent* inOwner)
		: AIState(inOwner) {}

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override { return "Attack"; }
};