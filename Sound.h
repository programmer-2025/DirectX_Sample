#pragma once
#include <string>
#include <vector>
#include <xaudio2.h>

struct Chunk {
	char id[10] = "";
	unsigned int size = 0;
};

struct AudioData {
	XAUDIO2_BUFFER buffer = {};
	IXAudio2SourceVoice** sourceVoice = nullptr;
	int svNum;
	std::wstring fileName;
};

namespace Sound {
	IXAudio2* audio = nullptr;
	IXAudio2MasteringVoice* masteringVoice = nullptr;
	std::vector<AudioData> audioDataList;

	void Init();
	int Load(std::wstring fileName, bool isLoop, int svNum);
	void Play(int ID);
	void Stop(int ID);
}