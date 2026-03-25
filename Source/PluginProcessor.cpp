#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "PluginEditor.h"

//==============================================================================
MyPluginAudioProcessor::MyPluginAudioProcessor()
{
    voices.resize(128);
}

MyPluginAudioProcessor::~MyPluginAudioProcessor() {}
struct EmbeddedSample
{
    const char* data;
    int size;
    
};
static const EmbeddedSample embeddedSamples[] =
{
    { BinaryData::_426_09Hz_0_1_1_0_0_wav, BinaryData::_426_09Hz_0_1_1_0_0_wavSize },
    { BinaryData::_426_09Hz_1_0_0_0_1_wav, BinaryData::_426_09Hz_1_0_0_0_1_wavSize },
    { BinaryData::_426_09Hz_1_0_0_1_0_wav, BinaryData::_426_09Hz_1_0_0_1_0_wavSize },
    { BinaryData::_426_09Hz_1_0_1_0_0_wav, BinaryData::_426_09Hz_1_0_1_0_0_wavSize },
    { BinaryData::_426_09Hz_1_0_1_0_1_wav, BinaryData::_426_09Hz_1_0_1_0_1_wavSize },

    { BinaryData::_445_45Hz_0_0_0_0_1_wav, BinaryData::_445_45Hz_0_0_0_0_1_wavSize },

    { BinaryData::_454_64Hz_0_1_1_0_1_wav, BinaryData::_454_64Hz_0_1_1_0_1_wavSize },

    { BinaryData::_464_21Hz_0_1_1_1_1_wav, BinaryData::_464_21Hz_0_1_1_1_1_wavSize },

    { BinaryData::_466_67Hz_1_1_0_0_0_wav, BinaryData::_466_67Hz_1_1_0_0_0_wavSize },

    { BinaryData::_608_28Hz_0_1_0_0_0_wav, BinaryData::_608_28Hz_0_1_0_0_0_wavSize },

    { BinaryData::_634_53Hz_0_0_0_0_0_wav, BinaryData::_634_53Hz_0_0_0_0_0_wavSize },

    { BinaryData::_668_18Hz_0_1_1_1_0_wav, BinaryData::_668_18Hz_0_1_1_1_0_wavSize },

    { BinaryData::_864_71Hz_0_0_1_0_1_wav, BinaryData::_864_71Hz_0_0_1_0_1_wavSize },
    { BinaryData::_864_71Hz_0_0_1_1_0_wav, BinaryData::_864_71Hz_0_0_1_1_0_wavSize },
    { BinaryData::_864_71Hz_0_0_1_1_1_wav, BinaryData::_864_71Hz_0_0_1_1_1_wavSize },

    { BinaryData::_882_00Hz_1_0_0_1_1_wav, BinaryData::_882_00Hz_1_0_0_1_1_wavSize },

    { BinaryData::_890_91Hz_1_0_0_0_0_wav, BinaryData::_890_91Hz_1_0_0_0_0_wavSize },

    { BinaryData::_918_75Hz_0_0_0_1_0_wav, BinaryData::_918_75Hz_0_0_0_1_0_wavSize },
    { BinaryData::_918_75Hz_0_0_0_1_1_wav, BinaryData::_918_75Hz_0_0_0_1_1_wavSize },
    { BinaryData::_918_75Hz_0_1_0_0_1_wav, BinaryData::_918_75Hz_0_1_0_0_1_wavSize },
    { BinaryData::_918_75Hz_0_1_0_1_0_wav, BinaryData::_918_75Hz_0_1_0_1_0_wavSize },
    { BinaryData::_918_75Hz_0_1_0_1_1_wav, BinaryData::_918_75Hz_0_1_0_1_1_wavSize },
    { BinaryData::_918_75Hz_1_0_1_1_0_wav, BinaryData::_918_75Hz_1_0_1_1_0_wavSize },
    { BinaryData::_918_75Hz_1_0_1_1_1_wav, BinaryData::_918_75Hz_1_0_1_1_1_wavSize },
    { BinaryData::_918_75Hz_1_1_0_0_1_wav, BinaryData::_918_75Hz_1_1_0_0_1_wavSize },
    { BinaryData::_918_75Hz_1_1_0_1_0_wav, BinaryData::_918_75Hz_1_1_0_1_0_wavSize },
    { BinaryData::_918_75Hz_1_1_0_1_1_wav, BinaryData::_918_75Hz_1_1_0_1_1_wavSize },
    { BinaryData::_918_75Hz_1_1_1_0_0_wav, BinaryData::_918_75Hz_1_1_1_0_0_wavSize },
    { BinaryData::_918_75Hz_1_1_1_0_1_wav, BinaryData::_918_75Hz_1_1_1_0_1_wavSize },
    { BinaryData::_918_75Hz_1_1_1_1_0_wav, BinaryData::_918_75Hz_1_1_1_1_0_wavSize },
    { BinaryData::_918_75Hz_1_1_1_1_1_wav, BinaryData::_918_75Hz_1_1_1_1_1_wavSize },

    { BinaryData::_958_70Hz_0_0_1_0_0_wav, BinaryData::_958_70Hz_0_0_1_0_0_wavSize }
};
void MyPluginAudioProcessor::loadSamples()
{
    samples.clear();

    juce::AudioFormatManager fm;
    fm.registerBasicFormats();

    for (auto& s : embeddedSamples)
    {
        auto stream = std::make_unique<juce::MemoryInputStream>(
            s.data,
            s.size,
            false
        );

        auto reader = std::unique_ptr<juce::AudioFormatReader>(
            fm.createReaderFor(std::move(stream))
        );

        if (reader)
        {
            auto buffer = std::make_unique<juce::AudioBuffer<float>>(
                (int)reader->numChannels,
                (int)reader->lengthInSamples
            );

            reader->read(buffer.get(), 0,
                (int)reader->lengthInSamples,
                0, true, true);

            samples.push_back(std::move(buffer));
        }
    }

    DBG("Loaded samples (BinaryData): " << samples.size());
}
//==============================================================================
void MyPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    
        midiCollector.reset(sampleRate);

        loadSamples();
    
}

void MyPluginAudioProcessor::releaseResources() {}
juce::MidiMessageCollector& MyPluginAudioProcessor::getMidiCollector()
{
    return midiCollector;
}

//==============================================================================



double MyPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

//==============================================================================
void MyPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
    juce::MidiBuffer& midiMessages)
{
    buffer.clear();

    std::vector<juce::MidiMessage> localQueue;

    {
        std::lock_guard<std::mutex> lock(midiMutex);
        localQueue.swap(midiQueue);
    }

    // =========================
    // 1. MIDI da GUI (tuo)
    // =========================
    for (auto& msg : localQueue)
    {
        if (msg.isNoteOn())
        {
            int note = msg.getNoteNumber();
            float vel = msg.getVelocity() / 127.0f;

            int index = note - baseNote;

            if (index >= 0 && index < (int)samples.size())
            {
                auto* samplePtr = samples[index].get();

                if (samplePtr)
                    voices[note].start(samplePtr, vel);
            }
        }
        else if (msg.isNoteOff())
        {
            int note = msg.getNoteNumber();

            if (note >= 0 && note < (int)voices.size())
                voices[note].stop();
        }
    }

    // =========================
    // 2. MIDI dalla DAW (IMPORTANTISSIMO)
    // =========================
    for (const auto metadata : midiMessages)
    {
        auto msg = metadata.getMessage();

        if (msg.isNoteOn())
        {
            int note = msg.getNoteNumber();
            float vel = msg.getVelocity() / 127.0f;

            int index = note - baseNote;

            if (index >= 0 && index < (int)samples.size())
            {
                auto* samplePtr = samples[index].get();

                if (samplePtr)
                    voices[note].start(samplePtr, vel);
            }
        }
        else if (msg.isNoteOff())
        {
            int note = msg.getNoteNumber();

            if (note >= 0 && note < (int)voices.size())
                voices[note].stop();
        }
    }

    // =========================
    // 3. Render audio
    // =========================
    for (auto& v : voices)
        v.render(buffer, 0, buffer.getNumSamples());
}bool MyPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() != juce::AudioChannelSet::disabled();
}

int MyPluginAudioProcessor::getNumPrograms() { return 1; }
int MyPluginAudioProcessor::getCurrentProgram() { return 0; }

void MyPluginAudioProcessor::setCurrentProgram(int) {}

const juce::String MyPluginAudioProcessor::getProgramName(int)
{
    return {};
}



void MyPluginAudioProcessor::changeProgramName(int, const juce::String&) {}

//==============================================================================
bool MyPluginAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* MyPluginAudioProcessor::createEditor()
{
    return new MyPluginAudioProcessorEditor(*this);
}

void MyPluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // Se aggiungerai parametri in futuro, li salvi qui
}
void MyPluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // Qui ricarichi lo stato salvato
}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MyPluginAudioProcessor();
}