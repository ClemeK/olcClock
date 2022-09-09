#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <string>
#include <cmath>
#include <time.h>

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
	const double pi = (22 / 7);

public:
	Example()
	{
		// Name your application
		sAppName = "olcClock";
	}
	// *********************************************************************************
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}
	// *********************************************************************************
	double Radians(double degrees)
	{
		return (degrees * (pi / 180));
	}

	olc::vd2d GetCartesian(double angle, double hyp)
	{
		olc::vd2d output; // vd2d

		output.y = sin(Radians(angle-90)) * hyp;
		output.x = cos(Radians(angle-90)) * hyp;

		return output;
	}

	void ClockMarks(POINT c, olc::vd2d start, olc::vd2d end, int i)
	{
		DrawLine(c.x + start.x, c.y - start.y, c.x + end.x, c.y - end.y, olc::GREY);
	}

	void ClockNumbers(POINT centre, int radius)
	{
		double NumberMarks = 360 / 12;
		for (int i = 1; i < 13; i++)
		{
			olc::vd2d mp = GetCartesian((i * NumberMarks), (radius + 15));

			DrawString(centre.x + (int32_t)mp.x, centre.y + (int32_t)mp.y, std::to_string(i), olc::WHITE, 1U);
		}
	}

	void DisplayTime(POINT centre, double division, int radius) {
		
		// Get the Time
		time_t timer;
		struct tm now = { time(&timer) };
		double hours = ((now.tm_sec / 60) / 60) % 12;
		hours++; // to allow for BST
		double minutes = (now.tm_sec / 60) % 60;
		double seconds = now.tm_sec % 60;

		// get the end points for each hand
		olc::vd2d h = GetCartesian((hours * division) - 90, (radius - 10));
		olc::vd2d m = GetCartesian((minutes * division) , radius );
		olc::vd2d s = GetCartesian((seconds * division) , radius);

		// Draw the Hands
		// Hours
		DrawLine(centre.x, centre.y, centre.x + h.x, centre.y + h.y, olc::BLUE);
		// Minutes
		DrawLine(centre.x, centre.y, centre.x + m.x, centre.y + m.y, olc::RED);
		// Seconds
		DrawLine(centre.x, centre.y, centre.x + s.x, centre.y + s.y, olc::GREEN);

		DigitalDisplay(hours, minutes, seconds);
	}

	void DigitalDisplay(int h, int m, int s)
	{
		std::string time{ std::to_string(h) + ":" + std::to_string(m) + ":" + std::to_string(s) };

		DrawString(10,10,time);
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Clear the screen
		Clear(olc::BLACK);

		// find the middle
		POINT centre{};
		centre.x = ScreenWidth() / 2;
		centre.y = ScreenHeight() / 2;

		int radius = (ScreenHeight() / 2) - 20;
		double circumferance = 2 * pi * radius;

		// Draw Centre
		DrawCircle(centre.x, centre.y, 2, olc::WHITE);

		// Draw clock Face
		DrawCircle(centre.x, centre.y, radius, olc::WHITE);

		double division = 360 / 60;

		for (int i = 0; i < 60; i++)
		{
			olc::vd2d startPoints = GetCartesian((i * division), (radius));
			olc::vd2d endPoints = GetCartesian((i * division), (radius - 10));

			ClockMarks(centre, startPoints, endPoints, i);
		}

		// Draw the Numbers
		ClockNumbers(centre, radius);

		DisplayTime(centre, division, radius);

		return true;
	}
};
// *********************************************************************************
int main()
{
	Example demo;

	if (demo.Construct(256, 156, 2, 2))
		demo.Start();

	return 0;
}