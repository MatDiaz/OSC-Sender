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

    DataCursor() {}

    ~DataCursor() {}

    void paint (Graphics& g) override
    {
        if (shouldPaint)
        {
            g.setColour (Colour (Colours::purple));
            g.fillEllipse (0, 0, 10, 10);
        }
    }
  
    void resized() override {}
    
    bool shouldPaint = false;
    
private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DataCursor)
};
