#include <math.h>
#include <vector>
#include <iostream>
// adjust header for linux
#if defined (__APPLE__) || defined(MACOSX)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
#include <chrono>
#include <thread>

struct Stream
{
	Stream(size_t sample_rate, float duration) : 
		sample_rate(sample_rate),
		duration(duration),
       		samples(duration * sample_rate) {};

	void generate_sine(float frequency) {
		// create a sine wave. basically a tone. :)
		for(size_t i = 0; i < samples.size(); ++i) { 
			samples[i] = volume * sin((2.f * M_PI * frequency) / sample_rate * i); 
		}
	}
	
	void modulate_sine(float frequency) {
		// multiplicate the existing sound with a sinus wave.
		for(size_t i = 0; i <  samples.size(); ++i) {
			samples[i] *=  sin((2.f * M_PI * frequency) / sample_rate * i);
		}
	}

	template<typename functor>
	void modulate(functor &f) {
        	for (size_t i = 0; i < samples.size(); ++i) {
			samples[i] *= f(sample_rate, i);
        	}
	}

	size_t size() const {
		return sizeof(uint16_t)*samples.size();
	}

	const uint16_t* data() const {
		return samples.data();
	}

	size_t const sample_rate;
	float const duration;
	size_t const volume = 32760;
	
	private:
		std::vector<uint16_t> samples;
};

struct SoundOutput
{
	size_t const sample_rate;
		
	SoundOutput(size_t sample_rate) : sample_rate(sample_rate) {
		ALCdevice *dev = NULL;
		ALCcontext *ctx = NULL;

		const char *defname = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
		std::cout << "Default device: " << defname << std::endl;

		dev = alcOpenDevice(defname);
		ctx = alcCreateContext(dev, NULL);
		alcMakeContextCurrent(ctx);
	}

	~SoundOutput() {
		ALCdevice *dev = NULL;
		ALCcontext *ctx = NULL;
		ctx = alcGetCurrentContext();
		dev = alcGetContextsDevice(ctx);

		alcMakeContextCurrent(NULL);
		alcDestroyContext(ctx);
		alcCloseDevice(dev);
		std::cout << "Device closed." << std::endl;
	}

	void play(Stream stream) {
		
		/* Create buffer to store samples */
		ALuint buf;
		alGenBuffers(1, &buf);

		/* Download buffer to OpenAL */
		alBufferData(buf, AL_FORMAT_MONO16, stream.data(), stream.size(), sample_rate);

		/* Set-up sound source and play buffer */
		ALuint src = 0;
		alGenSources(1, &src);
		alSourcei(src, AL_BUFFER, buf);
		alSourcePlay(src);

		/* While sound is playing, sleep */
		std::chrono::milliseconds duration( int(stream.duration * 1000) );
		std::this_thread::sleep_for( duration );
	}

	void operator()(Stream stream) {
		play(stream);
	}
};
