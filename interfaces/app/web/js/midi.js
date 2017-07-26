var midi = {

  access: null,

  initialize: function () {
    if(navigator.requestMIDIAccess) {
      navigator.requestMIDIAccess({
        sysex: false // this defaults to 'false' and we won't be covering sysex in this article.
      }).then(function (midiAccess) {
        messages.addInfo("Got MIDI access!");

        midi.access = midiAccess;

        var inputs = midi.access.inputs.values();
        var f = function (message) {
          var data = message.data;
          var event = {};
          event.type = data[0] >> 4;
          event.channel = data[0] & 0xF;
          event.data1 = data[1];
          event.data2 = data[2];
          for(var i = 0;i < midi.messageHandlers.length;i ++)
            midi.messageHandlers[i](event);
        };
        for(var input = inputs.next(); input && !input.done; input = inputs.next()) {
          console.log('[MIDI DEVICE] ' + input.value.name);
          input.value.onmidimessage = f;
        }

      }, function (e) {
        messages.addError("No access to MIDI devices or your browser doesn't support WebMIDI API. Please use WebMIDIAPIShim " + e);
      });
    } else {
      messages.addError("No MIDI support in your browser.");
    }
  },

  messageHandlers: [],

  onMessage: function(handler) {
    midi.messageHandlers.push(handler);
  }

};
