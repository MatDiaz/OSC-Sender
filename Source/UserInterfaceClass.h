/*
  ==============================================================================

    UserInterfaceClass.h
    Created: 4 Mar 2019 6:53:21pm
    Author:  Mateo Yepes

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/

class MainComponent;

class UserInterfaceClass    : public Component,
                              public FilenameComponentListener
{
public:
    UserInterfaceClass();
    ~UserInterfaceClass();

    void paint (Graphics&) override;
    void resized() override;
    
    void addReference(MainComponent& parentComponent);
    
    void filenameComponentChanged (FilenameComponent *fileComponentThatHasChanged) override;

private:
    MainComponent* fatherComponent;
    StringArray stringsToPass;
    Array<float> arrayToPass;
    std::unique_ptr<FilenameComponent> fileSearch;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UserInterfaceClass)
};

