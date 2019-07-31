#include "pch.h"

// Learn more about jsoncpp: https://github.com/open-source-parsers/jsoncpp
Json::Value AbstractScene::GetRootJson(LPCSTR jsonPath)
{
	std::ifstream jsonFile(jsonPath);

	if (!jsonFile.is_open())
	{
		ThrowMyException("Can't open json file", jsonPath);
	}

	Json::Reader reader;
	Json::Value  root;

	if (!reader.parse(jsonFile, root))
	{
		const auto msg = reader.getFormattedErrorMessages();
		ThrowMyException("Parse json file failed:", msg.c_str());
	}

	return root;
}

void AbstractScene::SetPause(bool setpause)
{
	if (isDoingTransitionScene) return;
	isPause = setpause;

	// handle music for current main scene
	if (isPause) Sounds::StopAll();
	else {
		if (AmbushTrigger::Instance()->GetState() == State::Ambush_Being)
			Sounds::PlayLoop(SoundId::Ambush);
		else
			Sounds::PlayLoop(GetBgMusic());
	}
	
}

// My teammates, you don't have to understand this, see how it's used!
void AbstractScene::DoTransitionScene(std::optional<Scene> next, float transTime)
{
	assert(transTime > 0.0f);
	static float timeRemain = 0.0f;

	if (isDoingTransitionScene) // update transition scene effect
	{
		timeRemain -= GameTimer::Dt();
		if (timeRemain <= 0.0f)
		{
			isDoingTransitionScene = false;
			SetPause(false);
		}
	}
	else // first call transition scene effect
	{
		assert(timeRemain <= 0.0f); // timeRemain should less or equal 0 if this is a first calll
		SetPause(true);
		isDoingTransitionScene = true;
		timeRemain = transTime;
		nextScene = next;
	}
}

std::optional<Scene> AbstractScene::GetNextScene() const
{ 
	if (isDoingTransitionScene) return {};
	else return nextScene; 
}

void AbstractScene::ToggleLight()
{
	static std::chrono::steady_clock::time_point lastToggle;
	std::chrono::duration<float> duration = std::chrono::steady_clock::now() - lastToggle;
	if (duration.count() > 0.169f)
	{
		lastToggle = std::chrono::steady_clock::now();
		isDark = !isDark;
	}
}
