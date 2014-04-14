#pragma once

namespace libsound
{
    public ref class AudioTool sealed
    {
    public:
		// Initialize AudioTool with the parameters of the output audio format
        AudioTool( unsigned int numChannels, unsigned int samplerate );
		virtual ~AudioTool();

		/////////////// SIMPLE BUFFER MANIPULATIONS ///////////////
		// This converts a source buffer into something the output can handle (e.g. performs upmixing or downmixing if srcNumChannels != this->numChannels)
		Platform::Array<float>^ conform( const Platform::Array<float>^ data, unsigned int srcNumChannels, unsigned int srcSamplerate );

		// Finds the energy in a Platform::Array<float>^
		float calcEnergy( const Platform::Array<float>^ data );

		/////////////// SOUND GENERATION ROUTINES ///////////////
		// Generate random noise using standard library
		Platform::Array<float>^ randn( unsigned int numSamples );

		// Generate a sin wave that lasts [numSamples] and oscillates [numOscilllations] times
		Platform::Array<float>^ sin( unsigned int numSamples, float numOscillations );
		Platform::Array<float>^ sin( unsigned int numSamples, float numOscillations, float startingPhase );

		// Returns [0,0,0,0,.....,0]
		Platform::Array<float>^ silence( unsigned int numSamples );
	private:
		// these are the output audio parameters, which allow us to generate the proper types of audio
		unsigned int numChannels, samplerate;
    };
}