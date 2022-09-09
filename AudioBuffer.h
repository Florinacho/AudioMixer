#ifndef __AUDIO_BUFFER_H__
#define __AUDIO_BUFFER_H__

#include <stdint.h>

class AudioBuffer {
	uint8_t* data;
	uint32_t length;
	uint32_t sampleRate;
	uint16_t bitsPerSample;
	uint16_t channelCount;
	
public:
	AudioBuffer();
	
	~AudioBuffer();
	
	uint8_t* create(uint32_t length, uint32_t sampleRate = 44100, uint16_t bitsPerSample = 16, uint16_t channelCount = 1);
	
	bool load(const char* filename);
	
	void clear();

	uint32_t getSampleRate() const;
	
	uint16_t getBitsPerSample() const;
	
	uint16_t getChannelCount() const;
	
	uint32_t getLength() const;
	
	uint8_t* getData() const;
};

#endif // __AUDIO_BUFFER_H__
