#pragma once

class Position final
{
public:
	void TurnLeft(bool isTurn);
	void TurnRight(bool isTurn);

	void SetFrame(float inFrame) noexcept { frame = inFrame; }
	float GetRotation() const noexcept { return rot; }

private:
	float rot, frame, leftTurnSpeed, rightTurnSpeed;
};
