#pragma once

struct Animation {
	//	int startX, startY, frameWidth, frameHeight, frameDuration, frames, loop, reverse, scale;
	int index;
	int frames;
	int speed;
	int delay;

	Animation() {}

	//	(startX, startY, frameWidth, frameHeight, frameDuration, frames, loop, reverse, scale)
	Animation(int i, int f, int s, int d) {
		index = i;
		frames = f;
		speed = s;
		delay = d;
	}
};