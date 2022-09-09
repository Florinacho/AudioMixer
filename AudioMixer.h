#ifndef __AUDIO_MIXER_H__
#define __AUDIO_MIXER_H__

#if defined(_WIN32)
#include <windows.h>
#endif // _WIN32

#include <pthread.h>
#include <vector>

#include "AudioSource.h"

#define AUDIO_MIXER_MAX_SOURCE_COUNT 16

class AudioMixer {
#if defined(_WIN32)
	HWAVEOUT waveOut;
	WAVEFORMATEX waveFormat;
	WAVEHDR* waveHeaders;
	friend void CALLBACK waveOutProc(HWAVEOUT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);
	friend void* AudioThreadProc(void*);
#else
#error Platform not supported!
#endif // _WIN32
	uint8_t* waveHeaderBuffer;
	int32_t* accumulator;
	
	volatile uint8_t freeBlocks;
	uint32_t blockCount;
	uint32_t blockSize;
	uint32_t blockIndex;

	std::vector<AudioSource*> sources;
	
	uint32_t samplesPerSec;
	uint16_t bitsPerSample;
	uint16_t channelCount;
	
	pthread_mutex_t freeBlocksMutex;
	pthread_cond_t freeBlocksCond;
	pthread_t audioThread;
	volatile bool ready;
	
	bool create(uint32_t sampleRate, uint16_t bitsPerSample, uint16_t channels, uint8_t nblockCount, uint32_t nchunkSize);
	void destroy();
public:
	AudioMixer(uint32_t sampleRate = 44100, uint16_t bitsPerSample = 16, uint16_t channels = 2, uint8_t nblockCount = 4, uint32_t nchunkSize = 1024);
	
	~AudioMixer();
	
	AudioSource* create(AudioBuffer* audioBuffer);
	
	AudioSource* create(const char* filename);
	
	AudioSource* play(AudioBuffer* audioBuffer);
	
	AudioSource* play(const char* filename);
};

#endif // __SOUND_MIXER_H__
