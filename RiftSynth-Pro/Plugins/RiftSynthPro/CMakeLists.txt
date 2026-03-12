if (NOT COMMAND juce_add_plugin)
    return()
endif()

juce_add_plugin(RiftSynthPro
    COMPANY_NAME "GareBear99"
    BUNDLE_ID "com.garebear99.riftsynthpro"
    PRODUCT_NAME "RiftSynthPro"
    FORMATS AU VST3
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    PLUGIN_MANUFACTURER_CODE Gb99
    PLUGIN_CODE RsPr
)

target_sources(RiftSynthPro PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
    ../../Shared/Analysis/AnalyzerState.cpp
    ../../Shared/Analysis/AnalyzerState.h
    ../../Shared/Seed/SeedEngine.cpp
    ../../Shared/Seed/SeedEngine.h
    ../../Shared/Synth/SimpleVoice.cpp
    ../../Shared/Synth/SimpleVoice.h
    ../../Shared/UI/AnalyzerComponent.cpp
    ../../Shared/UI/AnalyzerComponent.h
    ../../Shared/UI/SceneRenderer.cpp
    ../../Shared/UI/SceneRenderer.h
    ../../Shared/UI/NeonLookAndFeel.h
    ../../Shared/Core/ProductTypes.h
    ../../Shared/Core/ProductConfig.h
    ../../Shared/Core/SharedParameters.h
    ../../Shared/Core/TransportState.h
    ../../Shared/Presets/PresetBank.h
    ../../Shared/Playback/PatternPlayer.h
)
target_include_directories(RiftSynthPro PRIVATE ../../Shared)
target_link_libraries(RiftSynthPro PRIVATE juce::juce_audio_utils juce::juce_dsp)
riftwave_apply_common(RiftSynthPro)
