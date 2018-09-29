#pragma once
#include "Components.h"
#include "Vector2D.h"

class TransformComponent : public Component {
public:
	Vector2D position;
	Vector2D velocity;

	int height = 32;
	int width = 32;
	int scale = 1;

	int speed = 3;

	bool blocked = false;

	TransformComponent() {
		printf("First constructor\n");
	}

	TransformComponent(int sc) {
		printf("Second constructor\n");
		scale = sc;
	}

	TransformComponent(float x, float y) {
		printf("Third constructor\n");
		position.x = x;
		position.y = y;
	}

	TransformComponent(float x, float y, int h, int w, int sc) {
		printf("Fourth constructor\n");
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}

	void init() override {
		velocity.Zero(); 
	}

	void update() override {
		position.x += static_cast<float>(velocity.x * speed);
		position.y += static_cast<float>(velocity.y * speed);
	}
};