var band8, band16, band24, band32;

$(document).ready(function () {

  keyboard.create('keyboard', 'A0', 'C8');

  band8 = Switch.create('switch-bands-8').setId('bands-8').setCaption('8').setValue(false).onOn(function () { api.command('include settings_8.synth'); band16.setValue(false); band24.setValue(false); band32.setValue(false); });
  band16 = Switch.create('switch-bands-16').setId('bands-16').setCaption('16').setValue(true).onOn(function () { api.command('include settings_16.synth'); band8.setValue(false); band24.setValue(false); band32.setValue(false); });
  band24 = Switch.create('switch-bands-24').setId('bands-24').setCaption('24').setValue(false).onOn(function () { api.command('include settings_24.synth'); band8.setValue(false); band16.setValue(false); band32.setValue(false); });
  band32 = Switch.create('switch-bands-32').setId('bands-32').setCaption('32').setValue(false).onOn(function () { api.command('include settings_32.synth'); band8.setValue(false); band16.setValue(false); band24.setValue(false); });

  slider.create('slider-band-1').setValue(0.5).setCaption('50Hz').setId('eq_1');
  slider.create('slider-band-2').setValue(0.5).setCaption('100kHz').setId('eq_2');
  slider.create('slider-band-3').setValue(0.5).setCaption('200kHz').setId('eq_3');
  slider.create('slider-band-4').setValue(0.5).setCaption('0.5kHz').setId('eq_4');
  slider.create('slider-band-5').setValue(0.5).setCaption('1kHz').setId('eq_5');
  slider.create('slider-band-6').setValue(0.5).setCaption('2kHz').setId('eq_6');
  slider.create('slider-band-7').setValue(0.5).setCaption('5kHz').setId('eq_7');
  slider.create('slider-band-8').setValue(0.5).setCaption('13kHz').setId('eq_8');

  knob.create('knob-bandwidth').setId('bandwidth').setValue(0.5).setCaption('Band width');
  knob.create('knob-gain').setValue(0.5).setCaption('Gain');
  knob.create('knob-reverb').setValue(0.0).setCaption('Reverb');

});
