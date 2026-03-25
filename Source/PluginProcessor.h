#pragma once
#pragma once

#include <JuceHeader.h>

class MyPluginAudioProcessor : public juce::AudioProcessor
{
public:
    MyPluginAudioProcessor();
    ~MyPluginAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;
    
    double getTailLengthSeconds() const override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    void MyPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
        juce::MidiBuffer& midiMessages);

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override { return "Flauto Plugin"; }
    juce::MidiMessageCollector midiCollector;
    juce::MidiMessageCollector& getMidiCollector();
    void pushMidiMessage(const juce::MidiMessage& msg)
    {
        std::lock_guard<std::mutex> lock(midiMutex);
        midiQueue.push_back(msg);
    }
   

private:
    struct Voice
    {
        const juce::AudioBuffer<float>* sample = nullptr;
        int position = 0;
        float velocity = 1.0f;
        bool active = false;

        void start(const juce::AudioBuffer<float>* s, float vel)
        {
            sample = s;
            position = 0;
            velocity = vel;
            active = true;
        }

        void stop() { active = false; }

        void render(juce::AudioBuffer<float>& output, int start, int num)
        {
            if (!active || sample == nullptr)
                return;

            int remaining = sample->getNumSamples() - position;
            int toCopy = juce::jmin(num, remaining);

            for (int ch = 0; ch < output.getNumChannels(); ++ch)
            {
                output.addFrom(ch, start,
                    *sample, 0, position,
                    toCopy, velocity);
            }

            position += toCopy;

            if (position >= sample->getNumSamples())
                active = false;
        }
    };

    std::vector<std::unique_ptr<juce::AudioBuffer<float>>> samples;
    std::vector<Voice> voices;
    std::vector<juce::MidiMessage> midiQueue;
    std::mutex midiMutex;

    int baseNote = 36;

    

    void loadSamples();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyPluginAudioProcessor)
};