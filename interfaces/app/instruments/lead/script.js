$(document).ready(function () {

  keyboard.create('keyboard', 'A0', 'C8');

  knob.create('knob-attack')
    .setId('attack_time')
    .setCaption('Attack');

  knob.create('knob-decay')
    .setId('decay_time')
    .setCaption('Decay');

  knob.create('knob-sustain')
    .setId('sustain_level')
    .setCaption('Sustain');

  knob.create('knob-release')
    .setId('release_time')
    .setCaption('Release');

  knob.create('knob-bandpass-center')
    .setId('bandpass_center')
    .setCaption('Center');

  knob.create('knob-bandpass-width')
    .setId('bandpass_width')
    .setCaption('Width');

  knob.create('knob-fuzz')
    .setId('fuzz_gain')
    .setCaption('Fuzz');

  knob.create('knob-delay-gain')
    .setId('delay_gain')
    .setCaption('Delay gain');

  knob.create('knob-delay-time')
    .setId('delay_time')
    .setCaption('Delay time');

});
