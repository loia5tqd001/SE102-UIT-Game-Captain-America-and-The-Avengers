#include "pch.h"

bool                                Sounds::isMute            = false;
float                               Sounds::liVolume          = 1.0f ;
float                               Sounds::displayTimeRemain = 0.0f ;
CSoundManager                       Sounds::dsound                   ;
std::unordered_map<SoundId, CSound> Sounds::soundDictionary          ;

bool Sounds::CheckHoldingVolume()
{
	displayTimeRemain = 2.0f; // reset display volume bar time remain to 2 seconds every when pressing +/-

	static float timePressed = 0.0f; // time period has holded +/- 
	timePressed += GameTimer::Dt();

	static constexpr float thresholdHoldingTime = 0.05f;
	if (timePressed < thresholdHoldingTime) return false; // avoid holding +/- lead to volume fluctuating too fast
	else timePressed -= thresholdHoldingTime; return true;
}

void Sounds::VolumeUp()
{
	if (CheckHoldingVolume() == false) return; // avoid holding (+) too fast
	liVolume = min(1.0f, liVolume + 0.01f);

	if (!isMute) {
		for (auto& [_, sound] : soundDictionary)
			sound.GetBuffer(0)->SetVolume( LinearToLogVol(liVolume) );
	}
}

void Sounds::VolumeDown()
{
	if (CheckHoldingVolume() == false) return; // avoid holding (-) too fast
	liVolume = max(0.0f, liVolume - 0.01f);

	if (!isMute) {
		for (auto& [_, sound] : soundDictionary)
			sound.GetBuffer(0)->SetVolume( LinearToLogVol(liVolume) );
	}
}

void Sounds::SetMute(bool ismute)
{
	isMute = ismute;
	if (isMute) 
	{
		for (auto& [_, sound] : soundDictionary)
			sound.GetBuffer(0)->SetVolume(DSBVOLUME_MIN);
	}

	CheckHoldingVolume();
}

bool Sounds::IsPlayingAt(SoundId id)
{
	assert(soundDictionary.count(id) == 1);
	return soundDictionary.at(id).IsSoundPlaying();
}

void Sounds::LoadResources(const Json::Value& root)
{
	if (dsound.Initialize(Window::Instance().GetHWnd(), DSSCL_PRIORITY) != DS_OK)
		ThrowMyException("Init CSoundManager failed");
		
	if (dsound.SetPrimaryBufferFormat(2, 22050, 16) != DS_OK)
		ThrowMyException("Set primary buffer format for CSoundManager failed");

	const auto& soundsJson = root["sounds"];
	for (auto& sound : soundsJson)
	{
		const auto soundId          = (SoundId)sound[0].asInt();
		const auto& strWaveFileName = (LPSTR)sound[1].asCString();

		CSound* waveSound;
		if (dsound.Create(&waveSound, strWaveFileName, DSBCAPS_CTRLVOLUME) != S_OK)
		{
			ThrowMyException("Create CSound failed!");
		}

		soundDictionary.emplace(soundId, *waveSound);
		assert(soundDictionary.count(soundId) == 1);
	}
}

void Sounds::PlayAt(SoundId id)
{
	assert(soundDictionary.count(id) == 1);
	if (!isMute)
	{
		soundDictionary.at(id).Reset();
		soundDictionary.at(id).Play(0, 0, LinearToLogVol( liVolume ));
	}
}

void Sounds::PlayLoop(SoundId id)
{
	assert(soundDictionary.count(id) == 1);
	if (!isMute) soundDictionary.at(id).Play(0, DSBPLAY_LOOPING, LinearToLogVol( liVolume ));
}

void Sounds::StopAt(SoundId id)
{
	assert(soundDictionary.count(id) == 1);
	soundDictionary.at(id).Stop();
	soundDictionary.at(id).Reset();
}

void Sounds::StopAll()
{
	for (auto& [_, sound] : soundDictionary)
		sound.Stop(), sound.Reset();
}

void Sounds::HandleInput()
{
	static auto& wnd = Window::Instance();

	if (wnd.IsKeyPressed(VK_OEM_PLUS)) {
		Sounds::VolumeUp();
	}
	else if (wnd.IsKeyPressed(VK_OEM_MINUS)) {
		Sounds::VolumeDown();
	}
}

void Sounds::Draw()
{
	if (displayTimeRemain <= 0.0f) return;
	displayTimeRemain -= GameTimer::Dt();

	static Font fontDraw(8, "Segoe MDL2 Assets Regular");
	static auto texture = Textures::Get(TextureId::Bbox);	
	static const RectF bgArea       = { 228.0f, 40.0f, 242.0f, 90.0f }; // gray background of displaying volume
	static const RectF volBarArea   = { 232.0f, 42.0f, 238.0f, 82.0f }; // white background of displaying volume bar
	static const RectF bgNumberArea = { 228.0f, 82.0f, 242.0f, 90.0f }; // area of displaying volume number

	const float yPartition   = floor(40.0f + 42 - liVolume * 40.0f);
	const RectF fullPart     = { 232.0f, yPartition, 238.0f, 82.0f };
	const auto colorFullPart = isMute ? Colors::DimRed : Colors::DimBlue;
	const auto colorNumber   = isMute ? Colors::DimRed : Colors::White;

	Game::Instance().DrawSolidRect(bgArea    , Colors::DimGray); // gray background of displaying volume
	Game::Instance().DrawSolidRect(volBarArea, Colors::White  ); // white background of displaying volume bar
	Game::Instance().DrawSolidRect(fullPart  , colorFullPart  ); // display how full volume bar

	fontDraw.DrawStringWithin( std::to_string(int(liVolume * 100)), bgNumberArea, colorNumber ); // area of displaying volume number
}



