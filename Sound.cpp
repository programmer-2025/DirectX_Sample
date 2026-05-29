#include "Sound.h"

void Sound::Init() {
	CoInitializeEx(0, COINIT_MULTITHREADED);
	XAudio2Create(&audio);
	audio->CreateMasteringVoice(&masteringVoice);
}

int Sound::Load(std::wstring fileName, bool isLoop, int svNum) {
	for (int n = 0; n < audioDataList.size(); n++) {
		if (audioDataList[n].fileName == fileName) { //リスト内やつと同じ（=すでに使っている）
			return n;
		}
	}

	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);;
}

void Sound::Play(int ID)
{
}

void Sound::Stop(int ID)
{
}
