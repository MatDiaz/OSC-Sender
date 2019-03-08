/*
  ==============================================================================

    DataCursor.h
    Created: 8 Mar 2019 3:41:08pm
    Author:  MATEO

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class DataCursor    : public Component
{
public:
    DataCursor()
    {	
		
    }

    ~DataCursor()
    {
    }

    void paint (Graphics& g) override
    {
        
    }

    void resized() override
    {
        

    }

	OwnedArray<Point<float>> *misPoints;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DataCursor)
};
