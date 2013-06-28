#include <math.h>
#include <vector>
#include <iostream>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <chrono>
#include <thread>

struct SoundOutput
{

	void init() {
		ALCdevice *dev = NULL;
		ALCcontext *ctx = NULL;

		const char *defname = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
		std::cout << "Default device: " << defname << std::endl;

		dev = alcOpenDevice(defname);
		ctx = alcCreateContext(dev, NULL);
		alcMakeContextCurrent(ctx);
	}

	void exit() {
		ALCdevice *dev = NULL;
		ALCcontext *ctx = NULL;
		ctx = alcGetCurrentContext();
		dev = alcGetContextsDevice(ctx);

		alcMakeContextCurrent(NULL);
		alcDestroyContext(ctx);
		alcCloseDevice(dev);
	}

	void play_tone(float frequency, int seconds) {
		
		size_t const sample_rate = 22050; 
		size_t const volume      = 32760; 
		
		std::vector<uint16_t> samples(seconds * sample_rate);
		
		for(std::size_t i = 0; i < samples.size(); ++i) { 
			samples[i] = volume * sin((2.f * M_PI * frequency) / sample_rate * i); 
		}

		/* Create buffer to store samples */
		ALuint buf;
		alGenBuffers(1, &buf);

		/* Download buffer to OpenAL */
		alBufferData(buf, AL_FORMAT_MONO16, samples.data(), sizeof(uint16_t) * samples.size(), sample_rate);

		/* Set-up sound source and play buffer */
		ALuint src = 0;
		alGenSources(1, &src);
		alSourcei(src, AL_BUFFER, buf);
		alSourcePlay(src);

		/* While sound is playing, sleep */
		std::chrono::milliseconds dura( seconds * 1000 );
		std::this_thread::sleep_for( dura );
	}

	void operator()(float frequency, int seconds) {
		play_tone(frequency, seconds);
	}
};

int main(int argc, char* argv[]) {
	
	SoundOutput out;
	out.init();

	char key;
	while (key != 'q') {
		std::cin >> key;
		std::cout << "Key pressed: [" << key << "]" << std::endl;

		if (key == 'a') {std::thread t1(out, 440.f, 2); t1.detach();}
		else if (key == 's') {std::thread t2(out, 220.f, 2); t2.detach();}
		else {std::cout << "Not a valid key. Please try again." << std::endl;}
	}
	std::cout << "I'm quitting now..." << std::endl;

	out.exit();
	return 0;
}
