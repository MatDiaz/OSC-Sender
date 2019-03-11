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
        addAndMakeVisible (mainDataCursor);
        mainDataCursor.setBounds(300, 300, 100, 100);
    }

    ~Plot()
    {
        pointsArray.clearQuick(false);
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
				pointsArray.add(new Point<float>);

                pointsArray[i]->addXY((i * divX) * 1.005, height - ((dataSetToPlot[i] / Max) * height));

				if (!i)
                    dataSetPlot.startNewSubPath (*pointsArray[i]);
				else
                    dataSetPlot.lineTo (*pointsArray[i]);
			}

			g.strokePath (dataSetPlot, PathStrokeType(2.0f));
            
            mainDataCursor.setBounds(pointsArray[3]->getX() - 5, pointsArray[3]->getY() - 5, 10, 10);
            mainDataCursor.shouldPaint = true;
            isLoaded = false;
		}
    }

	void updatePlot(float* dataSet, int dataSize, bool Loaded)
	{
		isLoaded = Loaded;
		dataSetSize = dataSize;
		dataSetToPlot = dataSet;
		repaint();
	}
    
    void mouseDown (const MouseEvent& e) override
    {
        
    }
    
    void mouseDrag (const MouseEvent& e) override
    {
        if(e.mods.isLeftButtonDown())
        {
            int position = ((float)e.getMouseDownX() / (float)getWidth()) * dataSetSize;
            
            mainDataCursor.setBounds(pointsArray[position]->getX() - 5, pointsArray[position]->getY() - 5, 10, 10);
        }
    }
    
    void resized() override
    {
        
    }

	OwnedArray<Point<float>> pointsArray;

	bool isLoaded = false;

	int dataSetSize = 0;

	float* dataSetToPlot;

	DataCursor mainDataCursor;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plot)
};
