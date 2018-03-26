#pragma once

enum Direction { top, right, bottom, left };
enum Position { x, y };

struct Physics
{
	float gravity = 1.0f;
	float terminalVel = 10.0f;
};