$(document).ready(function () {

  keyboard.create('keyboard', 'A0', 'C8');

  knob.create('knob-attack')
    .setText('Attack')
    .onChangeMIDICC = function (midiCC) {
      api.command('node_set attack_time.midiCC ' + midiCC);
    };

  knob.create('knob-decay')
    .setText('Decay')
    .onChangeMIDICC = function (midiCC) {
      api.command('node_set decay_time.midiCC ' + midiCC);
    };

  knob.create('knob-sustain')
    .setText('Sustain')
    .onChangeMIDICC = function (midiCC) {
      api.command('node_set sustain_level.midiCC ' + midiCC);
    };

  knob.create('knob-release')
    .setText('Release')
    .onChangeMIDICC = function (midiCC) {
      api.command('node_set release_time.midiCC ' + midiCC);
    };

  knob.create('knob-bandpass-center')
    .setText('Center')
    .onChangeMIDICC = function (midiCC) {
      api.command('node_set bandpass_center.midiCC ' + midiCC);
    };

  knob.create('knob-bandpass-width')
    .setText('Width')
    .onChangeMIDICC = function (midiCC) {
      api.command('node_set bandpass_width.midiCC ' + midiCC);
    };

  knob.create('knob-fuzz')
    .setText('Fuzz')
    .onChangeMIDICC = function (midiCC) {
      api.command('node_set fuzz_gain.midiCC ' + midiCC);
    };

knob.create('knob-delay-gain')
  .setText('Delay gain')
  .onChangeMIDICC = function (midiCC) {
    api.command('node_set delay_gain.midiCC ' + midiCC);
  };

knob.create('knob-delay-time')
  .setText('Delay time')
  .onChangeMIDICC = function (midiCC) {
    api.command('node_set delay_time.midiCC ' + midiCC);
  };

});
