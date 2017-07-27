var keyboard = {

  set: function (element, low, high) {
    if(low === undefined) low = 'A0';
    if(high === undefined) high = 'C8';

    // Check if valid notes given
    var noteLow = notes.fromName(low);
    var noteHigh = notes.fromName(high);
    if(noteLow == -1 || noteHigh == -1) return;
    if(noteLow > noteHigh) return;
    if(notes.isSharp(noteLow) || notes.isSharp(noteHigh)) return;

    // Create keyboard
    element.addClass('keyboard');

    // Compute widths
    var amountOfWhite = 0;
    for(var i = noteLow;i <= noteHigh;i ++) {
      if(!notes.isSharp(i)) amountOfWhite ++;
    }
    var margin = 0.999; // This seems to work...
    var width_white = (element.width() - margin * (amountOfWhite - 1)) / amountOfWhite;
    var width_black = width_white / 1.5;

    // Place keys
    var x = 0;
    var previous_key_type = null;
    var current_key_type = null;

    for(var i = noteLow;i <= noteHigh;i ++) {
      // Create key
      var key = $('<div>').prop('id', keyboard.keyId(i)).addClass('key');

      if(notes.isSharp(i)) {
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
    }

    // Animate keys
    midi.onMessage(function (event) {
      console.log(event);
      if(event.type == 0x9 && event.data2 > 0)
        element.find('#' + keyboard.keyId(event.data1)).addClass('pressed');

      if((event.type == 0x9 && event.data2 == 0) || event.type == 0x8)
        element.find('#' + keyboard.keyId(event.data1)).removeClass('pressed');
    });
  },

  keyId: function(note) {
    return notes.name(note).replace('#', 'sharp');
  }

};
