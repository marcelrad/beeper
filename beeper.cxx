#include "SoundOutput.h"

struct ModulationFunctor {
    float frequency;
    ModulationFunctor ( float frequency )
    : frequency(frequency){
    
    }

    float operator()(float sample_value, unsigned int sample_rate, unsigned int time) {
        return sample_value * sin((2.f * M_PI * frequency) / sample_rate * time);
    }
};

int main(int argc, char* argv[]) {
	
	// sample rate
	int sr = 22050;

	SoundOutput out(sr);

	// stream with duration of 1 second
	for (int i = 1; i <= 5; ++i) {
        ModulationFunctor functor( i );
        Stream as(sr, 2);
		as.generate_sine(440);
		//as.modulate_sine(i);
        as.modulate( functor );
		out.play(as);
	}

	return 0;
}
