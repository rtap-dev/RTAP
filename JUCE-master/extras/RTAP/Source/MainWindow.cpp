/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-11 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

#include "JuceDemoHeader.h"

//==============================================================================
struct AlphabeticDemoSorter
{
    static int compareElements (const JuceDemoTypeBase* first, const JuceDemoTypeBase* second)
    {
        return first->name.compare (second->name);
    }
};

JuceDemoTypeBase::JuceDemoTypeBase (const String& demoName)  : name (demoName)
{
    AlphabeticDemoSorter sorter;
    getDemoTypeList().addSorted (sorter, this);
}

JuceDemoTypeBase::~JuceDemoTypeBase()
{
    getDemoTypeList().removeFirstMatchingValue (this);
}

Array<JuceDemoTypeBase*>& JuceDemoTypeBase::getDemoTypeList()
{
    static Array<JuceDemoTypeBase*> list;
    return list;
}

//==============================================================================
#if JUCE_WINDOWS || JUCE_LINUX || JUCE_MAC

// Just add a simple icon to the Window system tray area or Mac menu bar..
class DemoTaskbarComponent  : public SystemTrayIconComponent,
                              private Timer
{
public:
    DemoTaskbarComponent()
    {
        setIconTooltip ("RTAP");
    }

    void mouseDown (const MouseEvent&) override
    {
        // On OSX, there can be problems launching a menu when we're not the foreground
        // process, so just in case, we'll first make our process active, and then use a
        // timer to wait a moment before opening our menu, which gives the OS some time to
        // get its act together and bring our windows to the front.

        Process::makeForegroundProcess();
        startTimer (50);
    }

    // This is invoked when the menu is clicked or dismissed
    static void menuInvocationCallback (int chosenItemID, DemoTaskbarComponent*)
    {
        if (chosenItemID == 1)
            JUCEApplication::getInstance()->systemRequestedQuit();
    }

private:
    void timerCallback() override
    {
        stopTimer();

        PopupMenu m;
        m.addItem (1, "Quit RTAP");

        // It's always better to open menus asynchronously when possible.
        m.showMenuAsync (PopupMenu::Options(),
                         ModalCallbackFunction::forComponent (menuInvocationCallback, this));
    }
};

#endif

bool juceDemoRepaintDebuggingActive = false;

//==============================================================================
class ContentComponent   : public Component,
                           public ListBoxModel,
                           public ApplicationCommandTarget
{
public:
    ContentComponent()
    {
        LookAndFeel::setDefaultLookAndFeel (&lookAndFeelV3);
        addAndMakeVisible (demoList);

        demoList.setModel (this);
        demoList.setColour (ListBox::backgroundColourId, Colour::greyLevel (0.7f));
        demoList.selectRow (0);
    }

    void resized() override
    {
        Rectangle<int> r (getLocalBounds());

        if (r.getWidth() > 600)
        {
            demoList.setBounds (r.removeFromLeft (210));
            demoList.setRowHeight (20);
        }
        else
        {
            demoList.setBounds (r.removeFromLeft (130));
            demoList.setRowHeight (30);
        }

        if (currentDemo != nullptr)
            currentDemo->setBounds (r);
    }

    int getNumRows() override
    {
        return JuceDemoTypeBase::getDemoTypeList().size();
    }

    void paintListBoxItem (int rowNumber, Graphics& g, int width, int height, bool rowIsSelected) override
    {
        if (rowIsSelected)
            g.fillAll (findColour (TextEditor::highlightColourId));

        if (JuceDemoTypeBase* type = JuceDemoTypeBase::getDemoTypeList() [rowNumber])
        {
            String name (type->name.trimCharactersAtStart ("0123456789").trimStart());

            AttributedString a;
            a.setJustification (Justification::centredLeft);

            String category;

            if (name.containsChar (':'))
            {
                category = name.upToFirstOccurrenceOf (":", true, false);
                name = name.fromFirstOccurrenceOf (":", false, false).trim();

                if (height > 20)
                    category << "\n";
                else
                    category << " ";
            }

            if (category.isNotEmpty())
                a.append (category, Font (10.0f), Colours::black);

            a.append (name, Font (13.0f).boldened(), Colours::black);

            a.draw (g, Rectangle<int> (width + 10, height).reduced (6, 0).toFloat());
        }
    }

    void selectedRowsChanged (int lastRowSelected) override
    {
        if (JuceDemoTypeBase* selectedDemoType = JuceDemoTypeBase::getDemoTypeList() [lastRowSelected])
        {
            currentDemo = nullptr;
            addAndMakeVisible (currentDemo = selectedDemoType->createComponent());
            currentDemo->setName (selectedDemoType->name);
            resized();
        }
    }

    MouseCursor getMouseCursorForRow (int /*row*/) override
    {
        return MouseCursor::PointingHandCursor;
    }

    int getCurrentPageIndex() const noexcept
    {
        if (currentDemo == nullptr)
            return -1;

        Array<JuceDemoTypeBase*>& demos (JuceDemoTypeBase::getDemoTypeList());

        for (int i = demos.size(); --i >= 0;)
            if (demos.getUnchecked (i)->name == currentDemo->getName())
                return i;

        return -1;
    }

    void moveDemoPages (int numPagesToMove)
    {
        const int newIndex = negativeAwareModulo (getCurrentPageIndex() + numPagesToMove,
                                                  JuceDemoTypeBase::getDemoTypeList().size());
        demoList.selectRow (newIndex);
        // we have to go through our demo list here or it won't be updated to reflect the current demo
    }

    bool isShowingOpenGLDemo() const
    {
        return currentDemo != nullptr && currentDemo->getName().contains ("OpenGL");
    }

private:
    ListBox demoList;
    ScopedPointer<Component> currentDemo;

    LookAndFeel_V1 lookAndFeelV1;
    LookAndFeel_V2 lookAndFeelV2;
    LookAndFeel_V3 lookAndFeelV3;

    //==============================================================================
    // The following methods implement the ApplicationCommandTarget interface, allowing
    // this window to publish a set of actions it can perform, and which can be mapped
    // onto menus, keypresses, etc.

    ApplicationCommandTarget* getNextCommandTarget() override
    {
        // this will return the next parent component that is an ApplicationCommandTarget (in this
        // case, there probably isn't one, but it's best to use this method in your own apps).
        return findFirstTargetParentComponent();
    }

    void getAllCommands (Array <CommandID>& commands) override
    {
        // this returns the set of all commands that this target can perform..
        const CommandID ids[] = { MainAppWindow::showPreviousDemo,
                                  MainAppWindow::showNextDemo,
                                  MainAppWindow::welcome,
                                  MainAppWindow::componentsAnimation,
                                  MainAppWindow::componentsDialogBoxes,
                                  MainAppWindow::componentsKeyMappings,
                                  MainAppWindow::componentsMDI,
                                  MainAppWindow::componentsPropertyEditors,
                                  MainAppWindow::componentsTransforms,
                                  MainAppWindow::componentsWebBrowsers,
                                  MainAppWindow::componentsWidgets,
                                  MainAppWindow::useLookAndFeelV1,
                                  MainAppWindow::useLookAndFeelV2,
                                  MainAppWindow::useLookAndFeelV3,
                                  MainAppWindow::toggleRepaintDebugging,
                                 #if ! JUCE_LINUX
                                  MainAppWindow::goToKioskMode,
                                 #endif
                                  MainAppWindow::useNativeTitleBar
                                };

        commands.addArray (ids, numElementsInArray (ids));

        const CommandID engineIDs[] = { MainAppWindow::renderingEngineOne,
                                        MainAppWindow::renderingEngineTwo,
                                        MainAppWindow::renderingEngineThree };

        StringArray renderingEngines (MainAppWindow::getMainAppWindow()->getRenderingEngines());
        commands.addArray (engineIDs, renderingEngines.size());
    }

    void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result) override
    {
        const String generalCategory ("General");
        const String demosCategory ("Demos");

        switch (commandID)
        {
            case MainAppWindow::showPreviousDemo:
                result.setInfo ("Previous Demo", "Shows the previous demo in the list", demosCategory, 0);
                result.addDefaultKeypress ('-', ModifierKeys::commandModifier);
                break;

            case MainAppWindow::showNextDemo:
                result.setInfo ("Next Demo", "Shows the next demo in the list", demosCategory, 0);
                result.addDefaultKeypress ('=', ModifierKeys::commandModifier);
                break;

            case MainAppWindow::welcome:
                result.setInfo ("Welcome Demo", "Shows the first demo in the list", demosCategory, 0);
                result.addDefaultKeypress ('1', ModifierKeys::commandModifier);
                break;

            case MainAppWindow::componentsAnimation:
                result.setInfo ("Animation Demo", "Shows the second demo in the list", demosCategory, 0);
                result.addDefaultKeypress ('2', ModifierKeys::commandModifier);
                break;

            case MainAppWindow::componentsDialogBoxes:
                result.setInfo ("Dialog Boxes Demo", "Shows the third demo in the list", demosCategory, 0);
                result.addDefaultKeypress ('3', ModifierKeys::commandModifier);
                break;

            case MainAppWindow::componentsKeyMappings:
                result.setInfo ("Key Mappings Demo", "Shows the fourth demo in the list", demosCategory, 0);
                result.addDefaultKeypress ('4', ModifierKeys::commandModifier);
                break;

            case MainAppWindow::componentsMDI:
                result.setInfo ("Multi-Document Demo", "Shows the fith demo in the list", demosCategory, 0);
                result.addDefaultKeypress ('5', ModifierKeys::commandModifier);
                break;

            case MainAppWindow::componentsPropertyEditors:
                result.setInfo ("Property Editor Demo", "Shows the sixth demo in the list", demosCategory, 0);
                result.addDefaultKeypress ('6', ModifierKeys::commandModifier);
                break;

            case MainAppWindow::componentsTransforms:
                result.setInfo ("Component Transforms Demo", "Shows the sevent demo in the list", demosCategory, 0);
                result.addDefaultKeypress ('7', ModifierKeys::commandModifier);
                break;

            case MainAppWindow::componentsWebBrowsers:
                result.setInfo ("Web Browser Demo", "Shows the eight demo in the list", demosCategory, 0);
                result.addDefaultKeypress ('8', ModifierKeys::commandModifier);
                break;

            case MainAppWindow::componentsWidgets:
                result.setInfo ("Widgets Demo", "Shows the ninth demo in the list", demosCategory, 0);
                result.addDefaultKeypress ('9', ModifierKeys::commandModifier);
                break;

            case MainAppWindow::renderingEngineOne:
            case MainAppWindow::renderingEngineTwo:
            case MainAppWindow::renderingEngineThree:
            {
                MainAppWindow& mainWindow = *MainAppWindow::getMainAppWindow();
                const StringArray engines (mainWindow.getRenderingEngines());
                const int index = commandID - MainAppWindow::renderingEngineOne;

                result.setInfo ("Use " + engines[index], "Uses the " + engines[index] + " engine to render the UI", generalCategory, 0);
                result.setTicked (mainWindow.getActiveRenderingEngine() == index);

                result.addDefaultKeypress ('1' + index, ModifierKeys::noModifiers);
                break;
            }

            case MainAppWindow::useLookAndFeelV1:
                result.setInfo ("Use LookAndFeel_V1", String::empty, generalCategory, 0);
                result.addDefaultKeypress ('i', ModifierKeys::commandModifier);
                result.setTicked (typeid (LookAndFeel_V1) == typeid (getLookAndFeel()));
                break;

            case MainAppWindow::useLookAndFeelV2:
                result.setInfo ("Use LookAndFeel_V2", String::empty, generalCategory, 0);
                result.addDefaultKeypress ('o', ModifierKeys::commandModifier);
                result.setTicked (typeid (LookAndFeel_V2) == typeid (getLookAndFeel()));
                break;

            case MainAppWindow::useLookAndFeelV3:
                result.setInfo ("Use LookAndFeel_V3", String::empty, generalCategory, 0);
                result.addDefaultKeypress ('p', ModifierKeys::commandModifier);
                result.setTicked (typeid (LookAndFeel_V3) == typeid (getLookAndFeel()));
                break;

            case MainAppWindow::toggleRepaintDebugging:
                result.setInfo ("Toggle repaint display", String::empty, generalCategory, 0);
                result.addDefaultKeypress ('r', ModifierKeys());
                result.setTicked (juceDemoRepaintDebuggingActive);
                break;

            case MainAppWindow::useNativeTitleBar:
            {
                result.setInfo ("Use native window title bar", String::empty, generalCategory, 0);
                result.addDefaultKeypress ('n', ModifierKeys::commandModifier);
                bool nativeTitlebar = false;

                if (MainAppWindow* map = MainAppWindow::getMainAppWindow())
                    nativeTitlebar = map->isUsingNativeTitleBar();

                result.setTicked (nativeTitlebar);
                break;
            }

           #if ! JUCE_LINUX
            case MainAppWindow::goToKioskMode:
                result.setInfo ("Show full-screen kiosk mode", String::empty, generalCategory, 0);
                result.addDefaultKeypress ('f', ModifierKeys::commandModifier);
                result.setTicked (Desktop::getInstance().getKioskModeComponent() != 0);
                break;
           #endif

            default:
                break;
        }
    }

    bool perform (const InvocationInfo& info) override
    {
        MainAppWindow* mainWindow = MainAppWindow::getMainAppWindow();

        if (mainWindow == nullptr)
            return true;

        switch (info.commandID)
        {
            case MainAppWindow::showPreviousDemo:   moveDemoPages (-1); break;
            case MainAppWindow::showNextDemo:       moveDemoPages ( 1); break;

            case MainAppWindow::welcome:
            case MainAppWindow::componentsAnimation:
            case MainAppWindow::componentsDialogBoxes:
            case MainAppWindow::componentsKeyMappings:
            case MainAppWindow::componentsMDI:
            case MainAppWindow::componentsPropertyEditors:
            case MainAppWindow::componentsTransforms:
            case MainAppWindow::componentsWebBrowsers:
            case MainAppWindow::componentsWidgets:
                demoList.selectRow (info.commandID - MainAppWindow::welcome);
                break;

            case MainAppWindow::renderingEngineOne:
            case MainAppWindow::renderingEngineTwo:
            case MainAppWindow::renderingEngineThree:
                mainWindow->setRenderingEngine (info.commandID - MainAppWindow::renderingEngineOne);
                break;

            case MainAppWindow::useLookAndFeelV1:  LookAndFeel::setDefaultLookAndFeel (&lookAndFeelV1); break;
            case MainAppWindow::useLookAndFeelV2:  LookAndFeel::setDefaultLookAndFeel (&lookAndFeelV2); break;
            case MainAppWindow::useLookAndFeelV3:  LookAndFeel::setDefaultLookAndFeel (&lookAndFeelV3); break;

            case MainAppWindow::toggleRepaintDebugging:
                juceDemoRepaintDebuggingActive = ! juceDemoRepaintDebuggingActive;
                mainWindow->repaint();
                break;

            case MainAppWindow::useNativeTitleBar:
                mainWindow->setUsingNativeTitleBar (! mainWindow->isUsingNativeTitleBar());
                break;

           #if ! JUCE_LINUX
            case MainAppWindow::goToKioskMode:
                {
                    Desktop& desktop = Desktop::getInstance();

                    if (desktop.getKioskModeComponent() == nullptr)
                        desktop.setKioskModeComponent (getTopLevelComponent());
                    else
                        desktop.setKioskModeComponent (nullptr);

                    break;
                }
           #endif

            default:
                return false;
        }

        return true;
    }
};

//==============================================================================
static ScopedPointer<ApplicationCommandManager> applicationCommandManager;
static ScopedPointer<AudioDeviceManager> sharedAudioDeviceManager;

MainAppWindow::MainAppWindow()
    : DocumentWindow (JUCEApplication::getInstance()->getApplicationName(),
                      Colours::lightgrey,
                      DocumentWindow::allButtons)
{
    setUsingNativeTitleBar (true);
    setResizable (true, false);
    setResizeLimits (400, 400, 10000, 10000);

   #if JUCE_IOS || JUCE_ANDROID
    setFullScreen (true);
   #else
	setFullScreen (true);/*
    setBounds ((int) (0.1f * getParentWidth()),
               (int) (0.1f * getParentHeight()),
               jmax (1364, (int) (0.5f * getParentWidth())),
               jmax (730, (int) (0.7f * getParentHeight())));*/
   #endif

    contentComponent = new ContentComponent();
    setContentNonOwned (contentComponent, false);
    setVisible (true);

    // this lets the command manager use keypresses that arrive in our window to send out commands
    addKeyListener (getApplicationCommandManager().getKeyMappings());

   #if JUCE_WINDOWS || JUCE_LINUX || JUCE_MAC
    taskbarIcon = new DemoTaskbarComponent();
   #endif

    triggerAsyncUpdate();
}

MainAppWindow::~MainAppWindow()
{
    clearContentComponent();
    contentComponent = nullptr;
    applicationCommandManager = nullptr;
    sharedAudioDeviceManager = nullptr;

   #if JUCE_OPENGL
    openGLContext.detach();
   #endif
}

void MainAppWindow::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}

ApplicationCommandManager& MainAppWindow::getApplicationCommandManager()
{
    if (applicationCommandManager == nullptr)
        applicationCommandManager = new ApplicationCommandManager();

    return *applicationCommandManager;
}

AudioDeviceManager& MainAppWindow::getSharedAudioDeviceManager()
{
    if (sharedAudioDeviceManager == nullptr)
    {
        sharedAudioDeviceManager = new AudioDeviceManager();
        sharedAudioDeviceManager->initialise (2, 2, 0, true, String::empty, 0);
    }

    return *sharedAudioDeviceManager;
}

MainAppWindow* MainAppWindow::getMainAppWindow()
{
    for (int i = TopLevelWindow::getNumTopLevelWindows(); --i >= 0;)
        if (MainAppWindow* maw = dynamic_cast<MainAppWindow*> (TopLevelWindow::getTopLevelWindow (i)))
            return maw;

    return nullptr;
}

void MainAppWindow::handleAsyncUpdate()
{
    // This registers all of our commands with the command manager but has to be done after the window has
    // been created so we can find the number of rendering engines available
    ApplicationCommandManager& commandManager = MainAppWindow::getApplicationCommandManager();
    commandManager.registerAllCommandsForTarget (contentComponent);
    commandManager.registerAllCommandsForTarget (JUCEApplication::getInstance());
}

void MainAppWindow::showMessageBubble (const String& text)
{
    BubbleMessageComponent* bm = new BubbleMessageComponent (500);
    getContentComponent()->addChildComponent (bm);

    AttributedString attString;
    attString.append (text, Font (15.0f));

    bm->showAt (Rectangle<int> (getLocalBounds().getCentreX(), 10, 1, 1),
                attString,
                500,  // numMillisecondsBeforeRemoving
                true,  // removeWhenMouseClicked
                true); // deleteSelfAfterUse
}

static const char* openGLRendererName = "OpenGL Renderer";

StringArray MainAppWindow::getRenderingEngines() const
{
    StringArray renderingEngines;

    if (ComponentPeer* peer = getPeer())
        renderingEngines = peer->getAvailableRenderingEngines();

   #if JUCE_OPENGL
    renderingEngines.add (openGLRendererName);
   #endif

    return renderingEngines;
}

void MainAppWindow::setRenderingEngine (int index)
{
    showMessageBubble (getRenderingEngines()[index]);

   #if JUCE_OPENGL
    if (getRenderingEngines()[index] == openGLRendererName
          && contentComponent != nullptr
          && (! contentComponent->isShowingOpenGLDemo()))
    {
        openGLContext.attachTo (*getTopLevelComponent());
        return;
    }

    openGLContext.detach();
   #endif

    if (ComponentPeer* peer = getPeer())
        peer->setCurrentRenderingEngine (index);
}

int MainAppWindow::getActiveRenderingEngine() const
{
   #if JUCE_OPENGL
    if (openGLContext.isAttached())
        return getRenderingEngines().indexOf (openGLRendererName);
   #endif

    if (ComponentPeer* peer = getPeer())
        return peer->getCurrentRenderingEngine();

    return 0;
}
