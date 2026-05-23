SomeReverbAudioProcessorEditor::TutorialAudioProcessorEditor(
    TutorialAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p) {
  setSize(200, 200);
}
