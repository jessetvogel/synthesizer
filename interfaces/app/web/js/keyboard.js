var keyboard = {

  create: function (id, low, high) {
    if(low === undefined) low = 'A0';
    if(high === undefined) high = 'C8';

    // Check if valid notes given
    var noteLow = notes.fromName(low);
    var noteHigh = notes.fromName(high);
    if(noteLow == -1 || noteHigh == -1) return;
    if(noteLow > noteHigh) return;
    if(notes.isSharp(noteLow) || notes.isSharp(noteHigh)) return;

    return new function () {
        // Variables
        this.element = $(document.getElementById(id));
        this.low = noteLow;
        this.high = noteHigh;

        // Visuals
        this.element.addClass('keyboard');

        // Compute widths
        var amountOfWhite = 0;
        for(var i = noteLow;i <= noteHigh;i ++) {
          if(!notes.isSharp(i)) amountOfWhite ++;
        }
        var margin = 0.999; // This seems to work...
        var width_white = (this.element.width() - margin * (amountOfWhite - 1)) / amountOfWhite;
        var width_black = width_white / 1.5;

        // Place keys
        var x = 0;
        var previous_key_type = null;
        var current_key_type = null;

        for(var i = noteLow;i <= noteHigh;i ++) {
          // Create key
          var key = $('<div>').prop('id', keyboard.keyId(i)).addClass('key');

          if(notes.isSharp(i)) {
            key.addClass('black').css({ width: width_black + 'px' });
            current_key_type = 'black';
          }
          else {
            key.addClass('white').css({ width: width_white + 'px' });
            current_key_type = 'white';
          }

          if(previous_key_type != null) {
            if(previous_key_type == 'white' && current_key_type == 'white') x += width_white + margin;
            if(previous_key_type == 'white' && current_key_type == 'black') x += width_white + margin / 2 - width_black / 2;
            if(previous_key_type == 'black' && current_key_type == 'white') x += width_black / 2 + margin / 2;
          }

          key.css({ left: x + 'px' });
          this.element.append(key);
          previous_key_type = current_key_type;
        }

        // Let it respond to midi
        (function (_) { midi.onEvent(function (event) {
          console.log(event);
          if(event.type == midi.NOTE_ON && event.data2 > 0)
            _.element.find('#' + keyboard.keyId(event.data1)).addClass('pressed');

          if((event.type == midi.NOTE_ON && event.data2 == 0) || event.type == midi.NOTE_OFF)
            _.element.find('#' + keyboard.keyId(event.data1)).removeClass('pressed');
        }); })(this);
    };
  },

  keyId: function(note) {
    return notes.name(note).replace('#', 'sharp');
  }

};
