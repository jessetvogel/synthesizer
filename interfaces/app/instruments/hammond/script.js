$(document).ready(function () {

  keyboard.create('keyboard', 'A0', 'C8');

  slider.create('drawbar-1')
    .setText('16\'')
    .onChangeMIDICC = function (midiCC) {
      api.command('node_set drawbar_1.midiCC ' + midiCC);
    };

  slider.create('drawbar-2')
    .setText('5 1/3\'')
    .onChangeMIDICC = function (midiCC) {
      api.command('node_set drawbar_2.midiCC ' + midiCC);
    };

  slider.create('drawbar-3')
    .setText('8\'')
    .onChangeMIDICC = function (midiCC) {
      api.command('node_set drawbar_3.midiCC ' + midiCC);
    };

  slider.create('drawbar-4')
    .setText('4\'')
    .onChangeMIDICC = function (midiCC) {
      api.command('node_set drawbar_4.midiCC ' + midiCC);
    };

  slider.create('drawbar-5')
    .setText('2 3/5\'')
    .onChangeMIDICC = function (midiCC) {
      api.command('node_set drawbar_5.midiCC ' + midiCC);
    };

  slider.create('drawbar-6')
    .setText('2\'')
    .onChangeMIDICC = function (midiCC) {
      api.command('node_set drawbar_6.midiCC ' + midiCC);
    };

  slider.create('drawbar-7')
    .setText('1 3/5\'')
    .onChangeMIDICC = function (midiCC) {
      api.command('node_set drawbar_7.midiCC ' + midiCC);
    };

  slider.create('drawbar-8')
    .setText('1 1/3\'')
    .onChangeMIDICC = function (midiCC) {
      api.command('node_set drawbar_8.midiCC ' + midiCC);
    };

  slider.create('drawbar-9')
    .setText('1\'')
    .onChangeMIDICC = function (midiCC) {
      api.command('node_set drawbar_9.midiCC ' + midiCC);
    };

});
