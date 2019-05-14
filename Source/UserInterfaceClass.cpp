/*
  ==============================================================================

    UserInterfaceClass.cpp
    Created: 4 Mar 2019 6:53:21pm
    Author:  Mateo Yepes

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "UserInterfaceClass.h"
#include "MainComponent.h"

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
            FileInputStream dataStream (inputFile);
            if (dataStream.openedOk())
            {
                arrayToPass = nullptr;
                int size = 0;
                
                while (!dataStream.isExhausted())
                {
                    dataStream.readNextLine();
                    size++;
                }
                
                arrayToPass = new float[size];
                dataStream.setPosition(0);
                size = 0;
                
                while (!dataStream.isExhausted())
                {
                    auto Line = dataStream.readNextLine();
                    arrayToPass[size++] = Line.getFloatValue();
                }
                
                fatherComponent->receiveArray(arrayToPass, size);
            }
        }
    }
}
