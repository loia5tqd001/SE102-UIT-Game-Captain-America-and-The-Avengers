#include "pch.h"
#include "PittsburghScene.h"

AbstractScene::AbstractScene() noexcept
{
	isDark = typeid(*this) == typeid(PittsburghScene);
}

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

	if (HasMusic()) // handle music for current main scene
	{
		if (isPause) Sounds::StopAt(GetBgMusic());
		else         Sounds::PlayLoop(GetBgMusic());
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
