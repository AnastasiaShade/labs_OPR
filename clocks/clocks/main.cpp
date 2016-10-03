#include "stdafx.h"
#include "Consts.h"
#include <ctime>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

struct ClockFace
{
	CircleShape dot[NUM_OF_DOTS];
	CircleShape mainClockCircle;
	CircleShape centerClockCircle;
	Text digit[NUM_OF_DIGITS];
};

struct ClockHands
{
	RectangleShape hourHand;
	RectangleShape minuteHand;
	RectangleShape secondsHand;
};

void InitClockDots(RenderWindow &window, ClockFace &clockFace)
{
	float x = 0.0, y = 0.0;
	float angle = 0.0;
	for (int i = 0; i < NUM_OF_DOTS; i++)
	{
		x = (MAIN_CIRCLE_RADIUS - OFFSET_DOT) * cos(angle);
		y = (MAIN_CIRCLE_RADIUS - OFFSET_DOT) * sin(angle);
		if (i % 5 == 0)
			clockFace.dot[i] = CircleShape(3);
		else
			clockFace.dot[i] = CircleShape(1);
		clockFace.dot[i].setFillColor(Color::Black);
		clockFace.dot[i].setOrigin(clockFace.dot[i].getGlobalBounds().width / 2, clockFace.dot[i].getGlobalBounds().height / 2);
		clockFace.dot[i].setPosition(x + window.getSize().x / 2, y + window.getSize().y / 2);
		angle += ((2 * PI) / NUM_OF_DOTS);
	}
}

int InitDigits(RenderWindow &window, ClockFace &clockFace, Font &font)
{
	if (!font.loadFromFile("resources/fonts/GOST_B.ttf"))
		exit(EXIT_FAILURE);
	float x = 0.0, y = 0.0;
	float angle = 0.0;
	for (int i = 0; i < NUM_OF_DIGITS; ++i)
	{
		x = (MAIN_CIRCLE_RADIUS - OFFSET_DIGIT) * cos(angle - (PI / 3));
		y = (MAIN_CIRCLE_RADIUS - OFFSET_DIGIT) * sin(angle - (PI / 3));
		clockFace.digit[i].setString(to_string(i + 1));
		clockFace.digit[i].setFont(font);
		clockFace.digit[i].setCharacterSize(DIGIT_SIZE);
		clockFace.digit[i].setFillColor(Color::Black);
		clockFace.digit[i].setOrigin(clockFace.digit[i].getGlobalBounds().width / 2, clockFace.digit[i].getGlobalBounds().height);
		clockFace.digit[i].setPosition(x + window.getSize().x / 2, y + window.getSize().y / 2);
		angle += ((2 * PI) / NUM_OF_DIGITS);
	}
	return EXIT_SUCCESS;
}

void InitMainClockCircle(RenderWindow &window, ClockFace &clockFace)
{
	clockFace.mainClockCircle.setRadius(MAIN_CIRCLE_RADIUS);
	clockFace.mainClockCircle.setPointCount(NUM_OF_POLYGONS);
	clockFace.mainClockCircle.setOutlineThickness(CLOCK_CIRCLE_THICKNESS);
	clockFace.mainClockCircle.setOutlineColor(Color::Black);
	clockFace.mainClockCircle.setOrigin(clockFace.mainClockCircle.getGlobalBounds().width / 2, clockFace.mainClockCircle.getGlobalBounds().height / 2);
	clockFace.mainClockCircle.setPosition(window.getSize().x / 2 + CLOCK_CIRCLE_THICKNESS, window.getSize().y / 2 + CLOCK_CIRCLE_THICKNESS);
}

void InitCenterClockCircle(RenderWindow &window, ClockFace &clockFace, const Vector2f windowCenter)
{
	clockFace.centerClockCircle.setRadius(CENTER_CIRCLE_RADIUS);
	clockFace.centerClockCircle.setPointCount(NUM_OF_POLYGONS);
	clockFace.centerClockCircle.setFillColor(Color::Red);
	clockFace.centerClockCircle.setOrigin(clockFace.centerClockCircle.getGlobalBounds().width / 2, clockFace.centerClockCircle.getGlobalBounds().height / 2);
	clockFace.centerClockCircle.setPosition(windowCenter);
}

void InitClockHands(RenderWindow &window, ClockHands &hands, const Vector2f windowCenter)
{
	const Vector2f hourHandSize = Vector2f(5, 180);
	const Vector2f minuteHandSize = Vector2f(3, 240);
	const Vector2f secondsHandSize = Vector2f(2, 250);

	hands.hourHand.setSize(hourHandSize);
	hands.minuteHand.setSize(minuteHandSize);
	hands.secondsHand.setSize(secondsHandSize);

	hands.hourHand.setFillColor(Color::Black);
	hands.minuteHand.setFillColor(Color::Black);
	hands.secondsHand.setFillColor(Color::Red);

	hands.hourHand.setOrigin(hands.hourHand.getGlobalBounds().width / 2, hands.hourHand.getGlobalBounds().height - OFFSET_HAND);
	hands.minuteHand.setOrigin(hands.minuteHand.getGlobalBounds().width / 2, hands.minuteHand.getGlobalBounds().height - OFFSET_HAND);
	hands.secondsHand.setOrigin(hands.secondsHand.getGlobalBounds().width / 2, hands.secondsHand.getGlobalBounds().height - OFFSET_HAND);

	hands.hourHand.setPosition(windowCenter);
	hands.minuteHand.setPosition(windowCenter);
	hands.secondsHand.setPosition(windowCenter);
}

void InitClockParts(RenderWindow &window, ClockFace &clockFace, ClockHands &hands, Font &font, const Vector2f windowCenter)
{
	InitClockDots(window, clockFace);
	InitDigits(window, clockFace, font);
	InitMainClockCircle(window, clockFace);
	InitCenterClockCircle(window, clockFace, windowCenter);
	InitClockHands(window, hands, windowCenter);
}

int SetMusic(Music &clockTick)
{
	if (!clockTick.openFromFile("resources/sounds/clock-1.wav"))
		return EXIT_FAILURE;
	clockTick.setLoop(true);
	clockTick.play();
	return EXIT_SUCCESS;
}

void ProcessEvents(RenderWindow &window)
{
	Event event;
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
			window.close();
	}
}

void GetLocalSystemTime(ClockHands &hands)
{
	time_t currentTime = time(NULL);
	struct tm * ptm = localtime(&currentTime);

	hands.hourHand.setRotation(ptm->tm_hour * 30 + (ptm->tm_min / 2.f));
	hands.minuteHand.setRotation(ptm->tm_min * 6 + (ptm->tm_sec / 12.f));
	hands.secondsHand.setRotation(ptm->tm_sec * 6.f);
}

void DrawClockParts(RenderWindow &window, ClockHands &hands, ClockFace &clockFace)
{
	window.clear(Color::White);

	window.draw(clockFace.mainClockCircle);
	for (int i = 0; i < NUM_OF_DOTS; i++)
		window.draw(clockFace.dot[i]);
	for (int i = 0; i < NUM_OF_DIGITS; i++)
		window.draw(clockFace.digit[i]);
	window.draw(hands.hourHand);
	window.draw(hands.minuteHand);
	window.draw(hands.secondsHand);
	window.draw(clockFace.centerClockCircle);

	window.display();
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML Analog Clock", sf::Style::Close, settings);
	const Vector2f windowCenter = Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

	ClockFace clockFace;
	Font font;
	ClockHands hands;
	InitClockParts(window, clockFace, hands, font, windowCenter);

	Music clockTick;
	SetMusic(clockTick);

	while (window.isOpen())
	{
		ProcessEvents(window);
		GetLocalSystemTime(hands);
		DrawClockParts(window, hands, clockFace);
	}

	return EXIT_SUCCESS;
}