/*
  ==============================================================================

    AudioRecorder.h
    Created: 7 Oct 2019 6:16:40pm
    Author:  WATEO

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class AudioRecorder : public AudioIODeviceCallback
{
public:
	AudioRecorder()
	{
		backgroundThread.startThread();
	}
	~AudioRecorder()
	{
		stop();
	}

	void startRecording(const File& file)
	{
		stop();

		if (sampleRate > 0)
		{
			file.deleteFile();

			if (auto fileStream = std::unique_ptr<FileOutputStream>(file.createOutputStream()))
			{
				WavAudioFormat wavFormat;

				if (auto writer = wavFormat.createWriterFor(fileStream.get(), sampleRate, 1, 16, {}, 0))
				{
					fileStream.release();

					threadedWriter.reset(new AudioFormatWriter::ThreadedWriter(writer, backgroundThread, 32768));

					const ScopedLock sl(writerLock);
					activeWriter = threadedWriter.get();
				}
			}
		}
	}

	void stop()
	{
		{
			const ScopedLock sl(writerLock);
			activeWriter = nullptr;
		}

		threadedWriter.reset();
	}

	bool isRecording() const
	{
		return activeWriter.load() != nullptr;
	}

	void audioDeviceAboutToStart(AudioIODevice* device) override
	{
		sampleRate = device->getCurrentSampleRate();
	}

	void audioDeviceStopped() override
	{
		sampleRate = 0;
	}

	void audioDeviceIOCallback(const float** inputChannelData, int numInputChannels, float** outputChannelData, int numOutputChannels, int numSamples) override
	{
		const ScopedLock sl(writerLock);

		if (activeWriter.load() != nullptr)
		{
			activeWriter.load()->write(inputChannelData, numSamples);

			AudioBuffer<float> buffer(const_cast<float**> (inputChannelData), numInputChannels, numSamples);

		}
		for (int i = 0; i < numOutputChannels; ++i)
			if (outputChannelData[i] != nullptr)
				FloatVectorOperations::clear(outputChannelData[i], numSamples);
	}

private:
	double sampleRate = 0.0;
	int64 nextSampleNum = 0;

	CriticalSection writerLock;

	TimeSliceThread backgroundThread{ "Audio Recorder Thread" };

	std::unique_ptr<AudioFormatWriter::ThreadedWriter> threadedWriter;

	std::atomic<AudioFormatWriter::ThreadedWriter*> activeWriter{ nullptr };
};
