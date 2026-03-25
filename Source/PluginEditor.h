
#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class MyPluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    MyPluginAudioProcessorEditor(MyPluginAudioProcessor&);
    ~MyPluginAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void mouseDown(const juce::MouseEvent&) override;
    void mouseUp(const juce::MouseEvent&) override;


    MyPluginAudioProcessor& processor;

    // GUI
    juce::Image backgroundImage;

    
    std::map<int, juce::Rectangle<int>> keyRects;
    std::map<int, bool> keyPressed;
    std::map<int, juce::Rectangle<int>> keys;
    std::set<int> activeNotes;

    const int baseNote = 36;
    const int numKeys = 32;

    const int whiteWidth = 20;
    const int whiteHeight = 140;
    const int blackWidth = 12;
    const int blackHeight = 90;

    const std::vector<int> blackKeys = { 1, 3, 6, 8, 10 };

    bool isBlack(int note) const;

    void createKeyboard();

    // void highlight(int note);
   // void unhighlight(int note);


   // int getNoteFromX(int x) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyPluginAudioProcessorEditor)
};