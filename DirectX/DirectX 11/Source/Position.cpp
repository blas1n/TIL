#include "Position.h"

void Position::TurnLeft(bool isTurn)
{
	if (isTurn)
	{
		leftTurnSpeed += frame * 0.01f;

		if (leftTurnSpeed > (frame * 0.15f))
			leftTurnSpeed = frame * 0.115f;
	}
	else
	{
		leftTurnSpeed -= frame * 0.005f;

		if (leftTurnSpeed < 0.0f)
			leftTurnSpeed = 0.0f;
	}

	rot -= leftTurnSpeed;
	if (rot < 0.0f)
		rot += 360.0f;
}

void Position::TurnRight(bool isTurn)
{
	if (isTurn)
	{
		rightTurnSpeed += frame * 0.01f;

		if (rightTurnSpeed > (frame * 0.15f))
			rightTurnSpeed = frame * 0.115f;
	}
	else
	{
		rightTurnSpeed -= frame * 0.005f;

		if (rightTurnSpeed < 0.0f)
			rightTurnSpeed = 0.0f;
	}

	rot += rightTurnSpeed;
	if (rot > 360.0f)
		rot -= 360.0f;
}
