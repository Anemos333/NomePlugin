#include <JuceHeader.h>
#include "PluginEditor.h"

//==============================
// COSTRUTTORE
//==============================
MyPluginAudioProcessorEditor::MyPluginAudioProcessorEditor(MyPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(800, 400);

    

    backgroundImage = juce::ImageFileFormat::loadFrom(BinaryData::flauto_jpg,
        BinaryData::flauto_jpgSize);

    createKeyboard();
}

MyPluginAudioProcessorEditor::~MyPluginAudioProcessorEditor() {}

//==============================
// HELPER
//==============================
bool MyPluginAudioProcessorEditor::isBlack(int note) const
{
    return std::find(blackKeys.begin(), blackKeys.end(), note % 12) != blackKeys.end();
}

//==============================
// CREAZIONE TASTIERA
//==============================
void MyPluginAudioProcessorEditor::createKeyboard()
{
    keys.clear();

    int whiteIndex = 0;

    // TASTI BIANCHI
    for (int i = 0; i < numKeys; ++i)
    {
        int note = baseNote + i;

        if (!isBlack(note))
        {
            int x = whiteIndex * whiteWidth;

            keys[note] = juce::Rectangle<int>(x, 200, whiteWidth, whiteHeight);

            whiteIndex++;
        }
    }

    whiteIndex = 0;

    // TASTI NERI
    for (int i = 0; i < numKeys; ++i)
    {
        int note = baseNote + i;

        if (isBlack(note))
        {
            int x = (whiteIndex * whiteWidth) - (blackWidth / 2);

            keys[note] = juce::Rectangle<int>(x, 200, blackWidth, blackHeight);
        }
        else
        {
            whiteIndex++;
        }
    }
}

//==============================
// DISEGNO
//==============================
void MyPluginAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    // 🖼️ IMMAGINE
    if (backgroundImage.isValid())
    {
        g.drawImage(backgroundImage,
            0, 0,
            getWidth(),
            200,
            0, 0,
            backgroundImage.getWidth(),
            backgroundImage.getHeight());
    }

    // 🎹 TASTIERA
    for (const auto& key : keys)
    {
        int note = key.first;
        auto bounds = key.second;

        bool black = isBlack(note);

        if (activeNotes.count(note))
        {
            g.setColour(black ? juce::Colours::red : juce::Colours::yellow);
        }
        else
        {
            g.setColour(black ? juce::Colours::black : juce::Colours::white);
        }

        g.fillRect(bounds);

        g.setColour(juce::Colours::black);
        g.drawRect(bounds);
    }
}

//==============================
// MOUSE → MIDI
//==============================
void MyPluginAudioProcessorEditor::mouseDown(const juce::MouseEvent& e)
{
    for (const auto& key : keys)
    {
        if (key.second.contains(e.getPosition()))
        {
            int note = key.first;

            activeNotes.insert(note);

            // invia MIDI al processor
            processor.pushMidiMessage(
                juce::MidiMessage::noteOn(1, note, (juce::uint8)100)
            );

            repaint();
        }
    }
}

void MyPluginAudioProcessorEditor::mouseUp(const juce::MouseEvent& e)
{
    for (const auto& key : keys)
    {
        if (key.second.contains(e.getPosition()))
        {
            int note = key.first;

            activeNotes.erase(note);

            
            processor.pushMidiMessage(
                juce::MidiMessage::noteOff(1, note)
            );

            repaint();
        }
    }
}

//==============================
// RESIZE
//==============================
void MyPluginAudioProcessorEditor::resized()
{
}
