#include "stdafx.h"
#include <iostream>
#include <cmath>

void PrintMaxHeight(float height, float maxHeight, float time)
{
	if (height > maxHeight)
		printf("T=%f, Max height=%f\n", time, maxHeight);
	else
		printf("T=%f, Max height=%f\n", time, height);
}

void PrintMinHeight(float height, float time)
{
	if (height < 0)
		printf("t=%f, Min height=%f\n", time, 0);
	else
		printf("t=%f, Min height=%f\n", time, height);
}

float GetHeightAtGivenTime(float time, const float speed, const float gravityConst)
{
	const float coef = 0.5;
	return speed * time - coef * gravityConst * time * time;
}

void PrintHeightAndTime(float jumpHeight)
{
	const float g = 9.8f;
	const float T = sqrt(jumpHeight * 2 / g);
	const float V0 = g * T;
	float h;
	bool isMaxHeight = false;
	printf("T=%f\n", T);
	for (float t = 0; t < T * 2; t += 0.1f)
	{
		if (t > T && !isMaxHeight)
		{
			isMaxHeight = true;
			h = GetHeightAtGivenTime(T, V0, g);
			PrintMaxHeight(h, jumpHeight, T);
		}
		h = GetHeightAtGivenTime(t, V0, g);
		printf("t=%f, h=%f\n", t, h);
	}
	h = GetHeightAtGivenTime(T * 2, V0, g);
	PrintMinHeight(h, T * 2);
}

int main()
{
	float jumpHeight;
	printf("Enter jump height, please: ");
	if (0 == scanf("%f", &jumpHeight))
	{
		printf("\n" "Expected floating-point or integer number." "\n");
		return 1;
	}
	if (jumpHeight >= 0)
		PrintHeightAndTime(jumpHeight);
	else
	{
		printf("You enter a negative height.\n");
		return 1;
	}
	return 0;
}
