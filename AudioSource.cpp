#include "AudioSource.h"

#include <algorithm>

AudioSource::AudioSource(AudioBuffer* newBuffer) 
	: buffer(nullptr), position(0), volume(1.0), pan(0.5f), speed(1.0f), status(AS_STOP), loop(false) {
	setBuffer(newBuffer);
}

AudioSource::~AudioSource() {
	setBuffer(nullptr);
}

void AudioSource::setBuffer(AudioBuffer* value) {
	buffer = value;
}

AudioBuffer* AudioSource::getBuffer() const {
	return buffer;
}

bool AudioSource::play() {
	switch (status) {
	case AS_STOP :
		status = AS_PLAY;
		position = 0;
		return true;
	case AS_PAUSE :
		status = AS_PLAY;
		return true;
	}
	return false;
}

bool AudioSource::pause() {
	switch (status) {
	case AS_PLAY :
		status = AS_PAUSE;
		return true;
	}
	return false;
}

bool AudioSource::stop() {
	switch (status) {
	case AS_PLAY :
	case AS_PAUSE :
		status = AS_STOP;
		position = 0;
		return true;
	}
	return false;
}

bool AudioSource::finished() {
	return (status == AS_STOP);
}

uint8_t AudioSource::getStatus() const {
	return status;
}

double AudioSource::getElapsedSeconds() const {
	return (buffer == nullptr) ? 0.0 : position / buffer->getSampleRate() / buffer->getChannelCount() / 2;
}

double AudioSource::getTotalSeconds() const {
	return (buffer == nullptr) ? 0.0 : getSampleCount() / buffer->getSampleRate();
}

uint32_t AudioSource::getSampleCount() const {
	if (buffer == nullptr) {
		return 0;
	}
	
	if (buffer->getLength() == 0) {
		return 0;
	}
	
	if (buffer->getBitsPerSample() == 0) {
		return 0;
	}
	
	if (buffer->getChannelCount() == 0) {
		return 0;
	}
	
	return (buffer->getLength() / (buffer->getBitsPerSample() / 8) / buffer->getChannelCount());
}

void AudioSource::setPosition(uint32_t value) {
	if (buffer == nullptr) {
		return;
	}
	
	position = std::max(0u, std::min(buffer->getLength(), value));
	position = (position >> 2) << 2;
	
	if (status == AS_STOP) {
		status = AS_PAUSE;
	}
}

uint32_t AudioSource::getPosition() const {
	return position;
}

void AudioSource::setProgress(float value) {
	if (buffer == nullptr) {
		return;
	}
	
	position = (float)buffer->getLength() * std::min(1.0f, std::max(0.0f, value));
	position = (position >> 2) << 2; // align to the channel and byte count
	position = std::min(position, buffer->getLength());
	
	if (status == AS_STOP) {
		status = AS_PAUSE;
	}
}

float AudioSource::getProgress() const {
	return (buffer == nullptr) ? 0.0f : (float)position / (float)buffer->getLength();
}

void AudioSource::setLoop(bool value) {
	loop = value;
}

bool AudioSource::getLoop() const {
	return loop;
}

void AudioSource::setVolume(float value) {
	volume = std::max(0.0f, std::min(1.0f, value));
}

float AudioSource::getVolume() const {
	return volume;
}

void AudioSource::setPan(float value) {
	pan = std::max(0.0f, std::min(1.0f, value));;
}

float AudioSource::getPan() const {
	return pan;
}

void AudioSource::setSpeed(float value) {
	speed = value;
}

float AudioSource::getSpeed() const {
	return speed;
}
