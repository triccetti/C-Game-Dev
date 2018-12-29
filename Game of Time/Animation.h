#pragma once

struct Animation {
	//	int startX, startY, frameWidth, frameHeight, frameDuration, frames, loop, reverse, scale;
	int index;
	int frames;
	int speed;
	bool repeat;
	std::string next;
	bool started = false;

	Animation() {}

	//	(startX, startY, frameWidth, frameHeight, frameDuration, frames, loop, reverse, scale)
	Animation(int i, int f, int s, bool r = true, std::string n = "") {
		index = i;
		frames = f;
		speed = s;
		repeat = r;
		next = n;
	}
};