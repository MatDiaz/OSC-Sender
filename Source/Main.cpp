/*
  ==============================================================================

    This file was auto-generated!
|
    It contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"

//==============================================================================
class OSCSenderApplication  : public JUCEApplication
{
public:
    //==============================================================================
    OSCSenderApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        // This method is where you should put your application's initialisation code..

        mainWindow.reset (new MainWindow (getApplicationName()));
    }

    void shutdown() override
    {
        // Add your application's shutdown code here..

        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
    class MainWindow    : public ResizableWindow
    {
    public:
        MainWindow (String name)  : ResizableWindow(name, true)
        {
            setUsingNativeTitleBar (false);
            setContentOwned (new MainComponent(), true);
            setBackgroundColour (Colour(Colours::black));
            setFullScreen (false);
           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
           #else
            setResizable (false, false);
            juce::Rectangle<int> r = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
            centreWithSize ((float)r.getWidth(), (float)r.getHeight());
           #endif
            setVisible (true);
            setDraggable(true);
            setAlwaysOnTop(false);
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (OSCSenderApplication)
