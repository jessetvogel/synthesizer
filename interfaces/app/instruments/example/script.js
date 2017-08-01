$(document).ready(function () {

  keyboard.create('keyboard', 'A0', 'C8');

  knob.create('knob-attack')
    .setCaption('Attack');

  knob.create('knob-decay')
    .setCaption('Decay');

  knob.create('knob-sustain')
    .setCaption('Sustain');

  knob.create('knob-release')
    .setCaption('Release');

  slider.create('slider-1')
    .setCaption('Slider 1');

  slider.create('slider-2')
    .setCaption('Slider 2');

  slider.create('slider-3')
    .setCaption('Slider 3');

  slider.create('slider-4')
    .setCaption('Slider 4');


});
