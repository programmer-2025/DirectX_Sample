#pragma once
#include <string>
#include <xaudio2.h>
#include <vector>

struct AudioData {
	XAUDIO2_BUFFER buffer = {};
	IXAudio2SourceVoice** sourceVoice = nullptr;
	std::wstring fileName;
};

struct Chunk {
	char id[5] = "";
	unsigned int size = 0;
};

namespace AudioManager {

	inline IXAudio2* audio = nullptr;
	inline IXAudio2MasteringVoice* masteringVoice = nullptr;
	inline std::vector<AudioData> audioList;

	void InitManager();
	int Load(std::wstring fileName, bool loop = false);
	void Play(int id);
	void Stop(int id);

}