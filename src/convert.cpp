#include <memory> 
#include <iostream>
#include <chrono>
#include <vector>
#include <sndfile.h>

#include "NAM/dsp.h"

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

#define AUDIO_BUFFER_SIZE 32

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		std::cerr << "Usage: " << argv[0] << " <model_path> <input_wav> <output_wav>\n";
		return 1;
	}

	const char* modelPath = argv[1];
	const char* inputWavPath = argv[2];
	const char* outputWavPath = argv[3];

	std::cout << "Loading model " << modelPath << "\n";

	// Turn on fast tanh approximation
	nam::activations::Activation::enable_fast_tanh();

	std::unique_ptr<nam::DSP> model;

	model.reset();
	model = std::move(nam::get_dsp(modelPath));

	if (model == nullptr)
	{
		std::cerr << "Failed to load model\n";
		return 1;
	}

	// Open input WAV file
	SF_INFO sfinfo;
	SNDFILE* infile = sf_open(inputWavPath, SFM_READ, &sfinfo);
	if (!infile)
	{
		std::cerr << "Error opening input file: " << sf_strerror(nullptr) << "\n";
		return 1;
	}

	// Prepare output WAV file
	SNDFILE* outfile = sf_open(outputWavPath, SFM_WRITE, &sfinfo);
	if (!outfile)
	{
		std::cerr << "Error opening output file: " << sf_strerror(nullptr) << "\n";
		sf_close(infile);
		return 1;
	}

	// Initialize model
	model->Reset(sfinfo.samplerate, AUDIO_BUFFER_SIZE);

	std::vector<double> inputBuffer(AUDIO_BUFFER_SIZE);
	std::vector<double> outputBuffer(AUDIO_BUFFER_SIZE);
	std::vector<double> normalizedBuffer(AUDIO_BUFFER_SIZE);

	// Calculate the scaling factor for -6dB normalization
	const double targetLevel = std::pow(10, -6.0 / 20.0);
	double maxAmplitude = 0.0;

	std::cout << "Processing audio...\n";
	sf_count_t readCount;
	
	// First pass: find maximum amplitude
	while ((readCount = sf_read_double(infile, inputBuffer.data(), AUDIO_BUFFER_SIZE)) > 0)
	{
		model->process(inputBuffer.data(), outputBuffer.data(), readCount);
		for (sf_count_t i = 0; i < readCount; ++i)
		{
			maxAmplitude = std::max(maxAmplitude, std::abs(outputBuffer[i]));
		}
	}

	// Calculate scaling factor
	double scalingFactor = targetLevel / maxAmplitude;

	// Reset file position to the beginning
	sf_seek(infile, 0, SEEK_SET);

	// Second pass: process and normalize
	while ((readCount = sf_read_double(infile, inputBuffer.data(), AUDIO_BUFFER_SIZE)) > 0)
	{
		// Process the audio
		model->process(inputBuffer.data(), outputBuffer.data(), readCount);

		// Normalize the output
		for (sf_count_t i = 0; i < readCount; ++i)
		{
			normalizedBuffer[i] = outputBuffer[i] * scalingFactor;
		}

		// Write normalized audio to output file
		sf_write_double(outfile, normalizedBuffer.data(), readCount);
	}

	std::cout << "Processing complete.\n";

	// Clean up
	sf_close(infile);
	sf_close(outfile);

	return 0;
}
