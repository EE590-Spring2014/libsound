// libAudioTool.cpp
#include "AudioTool.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <random>

using namespace libsound;
using namespace Platform;

// Defined for randn()
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

AudioTool::AudioTool( unsigned int numChannels, unsigned int samplerate )
{
	this->numChannels = numChannels;
	this->samplerate = samplerate;
}

AudioTool::~AudioTool()
{
	this->numChannels = 0;
	this->samplerate = 0;
}

Platform::Array<float>^ AudioTool::conform( const Platform::Array<float>^ data, unsigned int srcNumChannels, unsigned int srcSamplerate ) {
	// Note, we don't actually conform samplerates here, as I don't have the patience to make a samplerate converter. :P

	// Since we don't do anything with samplerate, if the channels are the same, just pass the original data back
	if( srcNumChannels == this->numChannels )
		return *((Platform::Array<float>^ *) &data);	// I feel dirty just for writing this

	// Otherwise, we need to fix up the number of channels
	auto newData = ref new Platform::Array<float>(this->numChannels*data->Length);

	// If we're going from 1 channel to more (e.g. 2), then we'll just copy it!
	if( srcNumChannels == 1 ) {
		for( unsigned int i=0; i<data->Length; ++i ) {
			for( unsigned int k=0; k<this->numChannels; ++k ) {
				newData[this->numChannels*i + k] = data[i];
			}
		}
	}
		
	// Otherwise, if we're going from a multi-channel mix to a single-channel, just average everything together
	if( this->numChannels == 1 ) {
		for( unsigned int i=0; i<data->Length; ++i ) {
			newData[i] = 0;
			for( unsigned int k=0; k<srcNumChannels; ++k ) {
				newData[i] += data[srcNumChannels*i + k];
			}
		}
	}
	return newData;
}

/*
void LibAudioTool::mixIn( Platform::Array<float>^ * data1, const Platform::Array<float>^ data2 ) {
    // Don't do anything if we don't have matching lengths
    if( data2->Length != data1->Length )
        return;

    // Sum data2 into outArray (which is just a dereferenced data1)
    for( unsigned int i=0; i<data1->Length; ++i ) {
        data1[i] += data2[i];
    }
}
*/

Platform::Array<float>^ AudioTool::randn( unsigned int numSamples ) {
	// Buffer to store output data
	Platform::Array<float>^ data = ref new Platform::Array<float>(numSamples);

	// Use stdlib's random number generation
	std::default_random_engine generator;
	std::normal_distribution<float> randn(0.0,.25);
	for( unsigned int i=0; i<numSamples; ++i ) {
		data[i] = max(min(randn(generator), 1.0f), -1.0f);
	}

	// Send the data back!
	return data;
}

Platform::Array<float>^ AudioTool::silence( unsigned int numSamples ) {
	auto data = ref new Platform::Array<float>(this->numChannels*numSamples);
	memset( data->Data, 0, sizeof(float)*data->Length );
	return data;
}

Platform::Array<float>^ AudioTool::sin( unsigned int numSamples, float numOscillations ) {
	return AudioTool::sin( numSamples, numOscillations, 0.0f );
}

Platform::Array<float>^ AudioTool::sin( unsigned int numSamples, float numOscillations, float startingPhase  ) {
	auto data = ref new Platform::Array<float>(this->numChannels*numSamples);
	for( unsigned int i=0; i<numSamples; ++i ) {
		data[this->numChannels*i+0] = .5f*sinf(2*3.1415926f*numOscillations*i/float(numSamples) + startingPhase);
		for( unsigned int k=1; k<this->numChannels; ++k )
			data[this->numChannels*i+k] = data[this->numChannels*i+0];
	}
	return data;
}

float AudioTool::calcEnergy( const Platform::Array<float>^ data ) {
	float * realData = data->Data;
	float energy = 0.0f;
	float mean = 0.0f;
	
	for( unsigned int i=0;i<data->Length/this->numChannels; ++i ) {
		 energy += realData[i]*realData[i];
		 mean += realData[i];
	}

	return energy - mean*mean;
}