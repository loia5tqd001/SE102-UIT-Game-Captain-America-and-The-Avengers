#pragma once

class Sounds
{
private:
	static bool isMute;
	static float liVolume; // linear volume (NOTE: lVolume value used in IDirectSoundBuffer::SetVolume is logarithmic volume - not linear)
	static float displayTimeRemain; // time period to display volume bar remains: substracted each frame if not pressing +/-
	static CSoundManager dsound;
	static std::unordered_map<SoundId, CSound> soundDictionary;

	static bool  CheckHoldingVolume();

public:
	static void LoadResources(const Json::Value& root);

	static void PlayAt     (SoundId id);
	static void PlayLoop   (SoundId id);
	static void StopAt     (SoundId id);
	static void StopAll    (          );
	static bool IsPlayingAt(SoundId id);	

	static void VolumeUp();
	static void VolumeDown();

	static void SetMute(bool ismute);
	inline static bool IsMute() { return isMute; }

	static void HandleInput();
	static void Draw();
};



