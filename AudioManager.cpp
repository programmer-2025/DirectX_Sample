#include "AudioManager.h"

void AudioManager::InitManager() {
    CoInitializeEx(0, COINIT_MULTITHREADED);
    XAudio2Create(&audio);
    audio->CreateMasteringVoice(&masteringVoice);
}

int AudioManager::Load(std::wstring fileName, bool loop) {
    for (int n = 0; n < audioList.size(); n++) {
        if (audioList[n].fileName == fileName) { //リストと同じ名前（=すでに使っている）
            return n;
        }
    }
    HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD byteSize = 0;
    Chunk riffChunk = {};
    ReadFile(hFile, &riffChunk.id, 4, &byteSize, NULL);
    ReadFile(hFile, &riffChunk.size, 4, &byteSize, NULL);

    char wave[4] = "";
    ReadFile(hFile, &wave, 4, &byteSize, NULL);

    Chunk formatChunk = {};
    while (formatChunk.id[0] != 'f') {
        ReadFile(hFile, &formatChunk.id, 4, &byteSize, NULL);
    }
    ReadFile(hFile, &formatChunk.size, 4, &byteSize, NULL);

    WAVEFORMATEX format = {};
    ReadFile(hFile, &format.wFormatTag, 2, &byteSize, NULL);
    ReadFile(hFile, &format.nChannels, 2, &byteSize, NULL);
    ReadFile(hFile, &format.nSamplesPerSec, 4, &byteSize, NULL);
    ReadFile(hFile, &format.nAvgBytesPerSec, 4, &byteSize, NULL);
    ReadFile(hFile, &format.nBlockAlign, 2, &byteSize, NULL);
    ReadFile(hFile, &format.wBitsPerSample, 2, &byteSize, NULL);

    Chunk data = {};

    while (true) {

        ReadFile(hFile, &data.id, 4, &byteSize, NULL);
        if (strcmp(data.id, "data") == 0) {
            break;
        }
        else {
            ReadFile(hFile, &data.size, 4, &byteSize, NULL);
            char* buf = new char[data.size];
            ReadFile(hFile, buf, data.size, &byteSize, NULL);
        }
    }

    ReadFile(hFile, &data.size, 4, &byteSize, NULL);
    char* buf = new char[data.size];
    ReadFile(hFile, buf, data.size, &byteSize, NULL);
    CloseHandle(hFile);

    AudioData audioData = {};
    audioData.fileName = fileName;
    audioData.buffer.AudioBytes = data.size;
    audioData.buffer.pAudioData = (BYTE*)buf;
    audioData.buffer.Flags = XAUDIO2_END_OF_STREAM;
    audioData.sourceVoice = new IXAudio2SourceVoice * [1];
    
    if (loop) {
        audioData.buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
    }
    audio->CreateSourceVoice(&audioData.sourceVoice[0], &format);
    audioList.push_back(audioData);

    return (int)audioList.size() - 1;
}

void AudioManager::Play(int id) {
    XAUDIO2_VOICE_STATE state = {};
    audioList[id].sourceVoice[0]->GetState(&state);
    if (state.BuffersQueued == 0) {
        audioList[id].sourceVoice[0]->SubmitSourceBuffer(&audioList[id].buffer);
        audioList[id].sourceVoice[0]->Start();
    }
}

void AudioManager::Stop(int id) {
    audioList[id].sourceVoice[0]->Stop();
    audioList[id].sourceVoice[0]->FlushSourceBuffers();
}
