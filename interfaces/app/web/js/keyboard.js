var keyboard = {

  set: function (element) {
    element.addClass('keyboard');

    var margin = 1;
    var width_white = (element.width() - margin * 51) / 52;
    var width_black = width_white / 1.5;

    var x = 0;
    var previous_key_type = null;
    var current_key_type = null;

    for(var i = 9;i < 9 + 88;i ++) {
      var id = keyboard.noteToKeyId(i);
      var key = $('<div>').prop('id', id).addClass('key');

      if(id.indexOf('sharp') !== -1) {
        key.addClass('black');
        key.css({ width: width_black + 'px' });
        current_key_type = 'black';
      }
      else {
        key.addClass('white');
        key.css({ width: width_white + 'px' });
        current_key_type = 'white';
      }

      if(previous_key_type != null) {
        if(previous_key_type == 'white' && current_key_type == 'white') x += width_white + margin;
        if(previous_key_type == 'white' && current_key_type == 'black') x += width_white + margin / 2 - width_black / 2;
        if(previous_key_type == 'black' && current_key_type == 'white') x += width_black / 2 + margin / 2;
      }

      key.css({ left: x + 'px' });
      element.append(key);
      previous_key_type = current_key_type;

      midi.onMessage(function (event) {
        console.log(event);
        if(event.type == 0x9 && event.data2 > 0)
          element.find('#' + keyboard.noteToKeyId(event.data1)).addClass('pressed');

        if((event.type == 0x9 && event.data2 == 0) || event.type == 0x8)
          element.find('#' + keyboard.noteToKeyId(event.data1)).removeClass('pressed');
      });
    }
  },

  noteName: function(note) {
    var notes = ['C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B'];
    var name = notes[Math.round(note) % 12];
    var octave = Math.floor(Math.round(note) / 12);
    return name + octave;
  },

  noteToKeyId: function(note) {
    return keyboard.noteName(note).replace('#', 'sharp');
  }



};
