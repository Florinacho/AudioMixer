#include "AudioBuffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

namespace wave {
// http://soundfile.sapp.org/doc/WaveFormat/

struct FileHeader {
    uint32_t chunkId;           //"RIFF" (0x52,0x49,0x46,0x46)
    uint32_t chunkSize;         // (fileSize - 8)  - could also be thought of as bytes of data in file following this field (bytesRemaining)
    uint32_t riffType;          // "WAVE" (0x57415645)
};

struct FMTChunk {
    uint32_t chunkId;           // "fmt " - (0x666D7420)
    uint32_t chunkDataSize;     // 16 + extra format bytes
    uint16_t compressionCode;   // 1 - 65535
    uint16_t numChannels;       // 1 - 65535
    uint32_t sampleRate;        // 1 - 0xFFFFFFFF
    uint32_t avgBytesPerSec;    // 1 - 0xFFFFFFFF
    uint16_t blockAlign;        // 1 - 65535
    uint16_t bitsPerSample;     // 2 - 65535
    // uint16_t extraFormatBytes;  // 0 - 65535
};

struct DataChunk {
    uint32_t chunkId;
    uint32_t chunkDataSize;
	// uint8_t* data;
};

} // namespace wave

AudioBuffer::AudioBuffer() 
	: sampleRate(0), bitsPerSample(0), channelCount(0), length(0), data(nullptr) {
}

AudioBuffer::~AudioBuffer() {
	clear();
}

uint8_t* AudioBuffer::create(uint32_t nlength, uint32_t nsampleRate, uint16_t nbitsPerSample, uint16_t nchannelCount) {
	clear();
	
	sampleRate = nsampleRate;
	bitsPerSample = nbitsPerSample;
	channelCount = nchannelCount;
	
	length = nlength;
	data = new uint8_t[length];
	
	return data;
}

bool AudioBuffer::load(const char* filename) {
	clear();
	
	wave::FileHeader fileHeader;
	wave::FMTChunk fmtChunk;
	wave::DataChunk dataChunk;
	bool status = false;
	
	FILE* file = fopen(filename, "rb");
	if (file == nullptr) {
		goto EXIT_LABEL;
	}
	
	if (fread(&fileHeader, sizeof(fileHeader), 1, file) != 1) {
		goto EXIT_LABEL;
	}

	if ((fileHeader.chunkId != 0x46464952) || (fileHeader.riffType != 0x45564157)) {
		goto EXIT_LABEL;
	}
	
	if (fread(&fmtChunk, sizeof(fmtChunk), 1, file) != 1) {
		goto EXIT_LABEL;
	}

	if (fmtChunk.chunkId != 0x20746d66) {
		goto EXIT_LABEL;
	}
	
	sampleRate = fmtChunk.sampleRate;
	bitsPerSample = fmtChunk.bitsPerSample;
	channelCount = fmtChunk.numChannels;
	
	assert(fmtChunk.compressionCode == 1);
	assert(fmtChunk.avgBytesPerSec == 176400);
	assert(fmtChunk.blockAlign == 4);

	if (fread(&dataChunk, sizeof(dataChunk), 1, file) != 1) {
		goto EXIT_LABEL;
	}

	if (dataChunk.chunkId != 0x61746164) {
		goto EXIT_LABEL;
	}
	
	length = dataChunk.chunkDataSize;
	data = new uint8_t[dataChunk.chunkDataSize];
	if (fread(data, sizeof(uint8_t), dataChunk.chunkDataSize, file) != dataChunk.chunkDataSize) {
		goto EXIT_LABEL;
	}
	
	status = true;
	
EXIT_LABEL:
	fclose(file);		
	return status;
}

void AudioBuffer::clear() {
	if (data == nullptr) {
		return;
	}
	sampleRate = 0;
	bitsPerSample = 0;
	channelCount = 0;
	length = 0;
	delete [] data;
}

uint32_t AudioBuffer::getSampleRate() const {
	return sampleRate;
}

uint16_t AudioBuffer::getBitsPerSample() const {
	return bitsPerSample;
}

uint16_t AudioBuffer::getChannelCount() const {
	return channelCount;
}

uint32_t AudioBuffer::getLength() const {
	return length;
}

uint8_t* AudioBuffer::getData() const {
	return data;
}
