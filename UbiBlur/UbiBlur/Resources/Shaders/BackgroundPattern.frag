#version 400 core

// Output
layout(location = 0) out vec4 oColor;

// Input
in vec2 vTexCoords;

// Functions
void main() {
	const float CellCount = 8.0;

	ivec2 cellIndex = ivec2(vTexCoords * CellCount);
	int displacement = cellIndex.y % 2;
	cellIndex.x += displacement;
	bool isEven = cellIndex.x % 2 != 0;

	oColor = isEven ? vec4(1.0, 1.0, 1.0, 1.0) : vec4(0.2, 0.2, 0.6, 1.0);
}