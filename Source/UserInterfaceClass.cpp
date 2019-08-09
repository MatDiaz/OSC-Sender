/*
  ==============================================================================

    UserInterfaceClass.cpp
    Created: 4 Mar 2019 6:53:21pm
    Author:  Mateo Yepes

  ==============================================================================
*/

#include "MainComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "UserInterfaceClass.h"
#include <iostream>
#include <fstream>


//==============================================================================
UserInterfaceClass::UserInterfaceClass()
{
	getLookAndFeel().setColour (ComboBox::backgroundColourId, Colour(0x81361b45));
	getLookAndFeel().setColour(PopupMenu::backgroundColourId, Colour(0x81361b45));
	getLookAndFeel().setColour(PopupMenu::highlightedBackgroundColourId, Colour(0x54373636));
	getLookAndFeel().setColour(TextButton::buttonColourId, Colour(0x81361b45));

    fileSearch.reset(new FilenameComponent("fileSearch",
                                           {},
                                           false,
                                           false,
                                           false,
                                           "*.txt; *.dat",
                                           {},
                                           "Archivos"));
    fileSearch->setColour(TextEditor::backgroundColourId, Colour(Colours::white));
    addAndMakeVisible(fileSearch.get());
	fileSearch->addListener(this);
}

UserInterfaceClass::~UserInterfaceClass()
{
}

void UserInterfaceClass::addReference(MainComponent& parentComponent)
{
    fatherComponent = &parentComponent;
}

void UserInterfaceClass::paint (Graphics& g)
{
    
    g.fillAll (Colour(Colours::black));
    
}

void UserInterfaceClass::resized()
{
    auto area = getLocalBounds();
    area.removeFromBottom (getHeight() * 0.2);
    fileSearch->setBounds(area);
}

void UserInterfaceClass::filenameComponentChanged (FilenameComponent *fileComponentThatHasChanged)
{
    if (fileComponentThatHasChanged == fileSearch.get())
    {
        File inputFile (fileSearch->getCurrentFile());
        if (inputFile.exists())
        {
			std::string fileName = inputFile.getFullPathName().toStdString();
			std::ifstream F(fileName);
            std::string inputLine;
            
            while (std::getline(F, inputLine, ','))
            {
                float xValue;
                std::string stringData;
                std::istringstream inLine (inputLine);
                
                inLine >> xValue >> stringData;
                
                xValue = xValue >= 1000 || xValue <= - 1000 ? 0 : xValue;
                
                arrayToPass.add(xValue);
                stringsToPass.add(stringData);
            }
            fatherComponent->receiveArray(arrayToPass, stringsToPass, arrayToPass.size());
            
        }
    }
}
