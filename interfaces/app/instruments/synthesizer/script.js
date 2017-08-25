$(document).ready(function () {

  Switch.create('switch-polyphonic').setId('switch-polyphonic').setCaption('Polyphonic').onOn(function () { api.command('node_set collector.value collector_poly'); }).onOff(function () { api.command('node_set collector.value collector_lead'); })
  Switch.create('switch-vocoder').setId('switch-vocoder').setCaption('Vocoder').onOn(function () { api.command('node_set sound.value vocoder'); }).onOff(function () { api.command('node_set sound.value collector'); })

  knob.create('knob-glide').setId('glide').setCaption('Glide');
  knob.create('knob-delay-gain').setId('delay_gain').setCaption('Delay gain');
  knob.create('knob-delay-time').setId('delay_time').setCaption('Delay time');

  Switch.create('switch-1').setId('switch-1').setCaption('Osc. 1').onOn(function () { api.command('node_activate oscillator_1'); }).onOff(function () { api.command('node_deactivate oscillator_1'); });
  Switch.create('switch-2').setId('switch-2').setCaption('Osc. 2').onOn(function () { api.command('node_activate oscillator_2'); }).onOff(function () { api.command('node_deactivate oscillator_2'); });
  Switch.create('switch-3').setId('switch-3').setCaption('Osc. 3').onOn(function () { api.command('node_activate oscillator_3'); }).onOff(function () { api.command('node_deactivate oscillator_3'); })

  knob.create('knob-waveform-1').setId('waveform_1');
  knob.create('knob-waveform-2').setId('waveform_2');
  knob.create('knob-waveform-3').setId('waveform_3').setCaption('Waveform');

  knob.create('knob-harmonic-1').setId('harmonic_1');
  knob.create('knob-harmonic-2').setId('harmonic_2');
  knob.create('knob-harmonic-3').setId('harmonic_3').setCaption('Harmonic');

  knob.create('knob-detune-1').setId('detune_1');
  knob.create('knob-detune-2').setId('detune_2');
  knob.create('knob-detune-3').setId('detune_3').setCaption('Detune');

  knob.create('knob-gain-1').setId('gain_1');
  knob.create('knob-gain-2').setId('gain_2');
  knob.create('knob-gain-3').setId('gain_3').setCaption('Gain');

  knob.create('knob-attack').setId('attack_time').setCaption('Attack');
  knob.create('knob-decay').setId('decay_time').setCaption('Decay');
  knob.create('knob-sustain').setId('sustain_level').setCaption('Sustain');
  knob.create('knob-release').setId('release_time').setCaption('Release');

  knob.create('knob-filter-attack').setCaption('Attack');
  knob.create('knob-filter-decay').setCaption('Decay');
  knob.create('knob-filter-sustain').setCaption('Sustain');
  knob.create('knob-filter-release').setCaption('Release');

  knob.create('knob-filter-cutoff').setId('cutoff').setCaption('Cutoff');
  knob.create('knob-range').setCaption('Range');

  keyboard.create('keyboard', 'A0', 'C8');

});
