$(document).ready(function () {

  keyboard.create('keyboard', 'A0', 'C8');

  knob.create('knob-attack')
    .setValue(0.2)
    .setText('Attack');

  knob.create('knob-decay')
    .setValue(0.55)
    .setText('Decay');

  knob.create('knob-sustain')
    .setValue(0.35)
    .setText('Sustain');

  knob.create('knob-release')
    .setValue(0.7)
    .setText('Release');

  slider.create('slider-1')
    .setValue(0.0)
    .setText('Slider 1');

  slider.create('slider-2')
    .setValue(1.0)
    .setText('Slider 2');

  slider.create('slider-3')
    .setValue(0.618)
    .setText('Slider 3');

  slider.create('slider-4')
    .setValue(0.618)
    .setText('Slider 4');


});
