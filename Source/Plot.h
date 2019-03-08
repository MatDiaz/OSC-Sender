/*
  ==============================================================================

    Plot.h
    Created: 8 Mar 2019 2:23:34pm
    Author:  MATEO

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DataCursor.h"

//==============================================================================
/*
*/
class Plot    : public Component
{
public:
    Plot()
    {
    }

    ~Plot()
    {
    }

	void paint(Graphics& g) override
	{
		g.setColour(Colour((uint8)250, (uint8)250, (uint8)250, (uint8)35));

		g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 1.0f);

		if (isLoaded)
		{
			Path dataSetPlot;

			g.setColour(Colour(Colours::white));

			float divX = getWidth() / (dataSetSize - 1);

			float Min, Max;

			findMinAndMax(dataSetToPlot, dataSetSize, Min, Max);

			auto height = Min < 0 ? getHeight() / 2 : getHeight();

			for (auto i = 0; i < dataSetSize; ++i)
			{	
				Point<float> kPoint((i * divX) * 1.005, height - ((dataSetToPlot[i] / Max) * height));

				pointsArray.add(&kPoint);

				if (!i)
					dataSetPlot.startNewSubPath (kPoint);
				else
					dataSetPlot.lineTo (kPoint);
			}

			g.strokePath (dataSetPlot, PathStrokeType(2.0f));

			/*createDataCursor(pointsArray);*/

		}
    }

	void updatePlot(float* dataSet, int dataSize, bool Loaded)
	{
		isLoaded = Loaded;
		dataSetSize = dataSize;
		dataSetToPlot = dataSet;
		repaint();
	}

    void resized() override
    {
        
    }

	void createDataCursor(OwnedArray<Point<float>> &pointsToPass)
	{
		addAndMakeVisible(mainDataCursor);
		mainDataCursor.setBounds(getBounds());
	}

	OwnedArray<Point<float>> pointsArray;

	bool isLoaded = false;

	int dataSetSize = 0;

	float* dataSetToPlot;

	DataCursor mainDataCursor;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plot)
};
