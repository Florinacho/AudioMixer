#include <stdio.h>
#include "AudioMixer.h"

static const int PROGRESS_BAR_LENGTH = 20;

int main(int argc, char* argv[]) {
	const char* filename = "test.wav";
	if (argc >= 2) {
		filename = argv[1];
	}
	
	AudioMixer mixer(44100, 16, 2, 8, 4096);
	AudioSource* source = mixer.play(filename);
	if (source == NULL) {
		printf("Failed to load '%s'.\n", filename);
		return 1;
	}
	
	printf("Filename  '%s'\n", filename);
	printf("Duration  %.2d:%.2d\n", (int)source->getTotalSeconds() / 60, (int)source->getTotalSeconds() % 60);
	printf("Volume    %.2f\n", source->getVolume());
	printf("Pan       %.2f\n", source->getPan());
	printf("Speed     %.2fx\n", source->getSpeed());
	
	while (!source->finished()) {
		const float progress = source->getProgress();
		const int left = progress * PROGRESS_BAR_LENGTH;
		const int right = PROGRESS_BAR_LENGTH - left;
		printf("\33[2K\rProgress  [%*s%*s", left, "*", right, "]");
	}
	printf("\nFinished playing\n");
	
	return 0;
}