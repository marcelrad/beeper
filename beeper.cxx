#include "SoundOutput.h"


int main(int argc, char* argv[]) {
	
	// sample rate
	int sr = 22050;

	SoundOutput out(sr);

	// stream with duration of 1 second
	for (int i = 1; i <= 5; ++i) {
		Stream as(sr, 2);
		as.generate_sine(440);
		as.modulate_sine(i);
		out.play(as);
	}

	return 0;
}
