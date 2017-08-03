$(document).ready(function () {

  knob.create('knob-glide').setCaption('Glide');
  knob.create('knob-delay-gain').setCaption('Delay gain');
  knob.create('knob-delay-time').setCaption('Delay time');

  Switch.create('switch-1').setId('switch-1').setCaption('Oscillator 1').onOn(function () { console.log('On! (1)'); }).onOff(function () { console.log('Off! (1)'); });
  Switch.create('switch-2').setId('switch-2').setCaption('Oscillator 2').onOn(function () { console.log('On! (2)'); }).onOff(function () { console.log('Off! (2)'); });
  Switch.create('switch-3').setId('switch-3').setCaption('Oscillator 3').onOn(function () { console.log('On! (3)'); }).onOff(function () { console.log('Off! (3)'); });

  knob.create('knob-waveform-1');
  knob.create('knob-waveform-2');
  knob.create('knob-waveform-3').setCaption('Waveform');

  knob.create('knob-harmonic-1');
  knob.create('knob-harmonic-2');
  knob.create('knob-harmonic-3').setCaption('Harmonic');

  knob.create('knob-gain-1');
  knob.create('knob-gain-2');
  knob.create('knob-gain-3').setCaption('Gain');

  knob.create('knob-attack').setCaption('Attack');
  knob.create('knob-decay').setCaption('Decay');
  knob.create('knob-sustain').setCaption('Sustain');
  knob.create('knob-release').setCaption('Release');

  knob.create('knob-filter-attack').setCaption('Attack');
  knob.create('knob-filter-decay').setCaption('Decay');
  knob.create('knob-filter-sustain').setCaption('Sustain');
  knob.create('knob-filter-release').setCaption('Release');

  knob.create('knob-filter-cutoff').setCaption('Cutoff');
  knob.create('knob-range').setCaption('Range');

  keyboard.create('keyboard', 'A0', 'C8');

});
