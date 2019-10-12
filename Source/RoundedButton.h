/*
  ==============================================================================

    RoundedButton.h
    Created: 12 Oct 2019 12:47:28pm
    Author:  Mateo Yepes

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class RoundedButton: public TextButton
{
public:
    RoundedButton(){}
    ~RoundedButton(){}
    void paint(Graphics& g) override
    {
        g.setColour(Colour(Colours::black));
        g.fillAll();
        
        this->getButtonText();
        std::cout << "Ripain" << std::endl;
    }
};
