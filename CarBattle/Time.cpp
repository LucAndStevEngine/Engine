#include "Time.h"

float Time::deltaTime = 0;
float Time::fixedDeltaTime = 0.01666666666f;
float Time::elapsedTime = 0;
float Time::m_lastFrameTime = 0;
int Time::FPS = 0;
int Time::frameCount = 0;
float Time::frameTime = 0;
float Time::m_fixedTime = 0;


void Time::UpdateTime(float time)
{
	if (m_lastFrameTime == 0)
	{
		m_lastFrameTime = time;
	}

	deltaTime = time - m_lastFrameTime;
	elapsedTime += deltaTime;
	m_fixedTime += deltaTime;

	frameCount++;
	frameTime += deltaTime;
	if (frameTime >= 1)
	{
		FPS = frameCount;
		frameCount = 0;
		frameTime = 0;
	}

	m_lastFrameTime = time;
}