#pragma once

#include <audioclient.h>
#include <phoneaudioclient.h>


namespace libsound
{
	// Callback definitions

	public delegate void AudioInCallback(const Platform::Array<float>^ data);
	public delegate Platform::Array<float>^ AudioOutCallback( unsigned int numSamples );

    public ref class SoundIO sealed
    {
    public:
		// Opens up the audio devices and starts threads for input/output, if desired.
		// Note, you can pass nullptr for both, either or none of the callbacks
		SoundIO();
		virtual ~SoundIO();

		// Starts callback threads, etc...
		void start();

		// Stops the callback threads, etc...
		void stop();

		// Methods used to query the audio output format properties
		unsigned int getOutputBufferSize();
		unsigned int getOutputSampleRate();
		unsigned int getOutputNumChannels();
		unsigned int getOutputBitdepth();

		// Methods used to query the audio input format properties
		unsigned int getInputBufferSize();
		unsigned int getInputSampleRate();
		unsigned int getInputNumChannels();
		unsigned int getInputBitdepth();

		// Feed a chunk of audio out to the device.  Returns how many samples were fed
		unsigned int writeAudio( const Platform::Array<float>^ data );

		// Read in a chunk of audio from the microphone
		Platform::Array<float>^ readAudio( void );

		// Event interface! Yeah!
		event AudioInCallback^ audioInEvent;
		event AudioOutCallback^ audioOutEvent;
	private:
		// Audio devices and clients
		IAudioClient2 *outputDevice, *inputDevice;
		IAudioRenderClient *outputClient;
		IAudioCaptureClient *inputClient;

		// Formats of audio streams for capture and playback
		WAVEFORMATEX *outputFormat, *inputFormat;

		// Buffer lengths for input and output in samples
		unsigned int outputBufferLen, inputBufferLen;

		// Threads that get audio input and call their callback, or call their callbacks and output to the sound card
		void audioInThread( Windows::Foundation::IAsyncAction^ operation );
		void audioOutThread( Windows::Foundation::IAsyncAction^ operation );

		// Handles to the threads so we can close them down when we die
		Windows::Foundation::IAsyncAction ^audioInThreadHandle, ^audioOutThreadHandle;
		
		// Handles that are used to tell if an audio buffer is ready to be read or outputted to
		HANDLE audioInReady, audioOutReady;
    };
}