var midi = {

  access: null,

  devices: [],

  initialize: function () {
    // Request MIDI access
    if(navigator.requestMIDIAccess) {
      navigator.requestMIDIAccess().then(function (midiAccess) {
        // Set access
        midi.access = midiAccess;

        // Set message handlers
        var inputs = midi.access.inputs.values();
        for(var input = inputs.next(); input && !input.done; input = inputs.next()) {
          console.log('[MIDI DEVICE] ' + input.value.name);
          input.value.onmidimessage = midi.messageHandler;
        }
      }, function (error) {
        // In case of no access, show error message
        messages.addError('No access for MIDI');
        console.log(error);
      });
    } else {
      messages.addError('No support for WebMIDI API');
    }

    // Request list of MIDI devices
    api.command('status midi_devices');
  },

  messageHandlers: [],

  onMessage: function (handler) {
    midi.messageHandlers.push(handler);
  },

  messageHandler: function (message) {
    // Check if this message is to be handled
    var sourceDevice = midi.getDevice(message.srcElement.name);
    if(sourceDevice == null || !sourceDevice.active) return;

    // Create event from message data
    var data = message.data;
    var event = {};
    event.type = data[0] >> 4;
    event.channel = data[0] & 0xF;
    event.data1 = data[1];
    event.data2 = data[2];

    // Send it to all message handlers
    for(var i = 0;i < midi.messageHandlers.length;i ++)
      midi.messageHandlers[i](event);
  },

  setDevices: function (data) {
    midi.devices = data;
  },

  getDevice: function (name) {
    for(var i = 0;i < midi.devices.length;i ++) {
      if(midi.devices[i].name == name)
        return midi.devices[i];
    }
    return null;
  }

};
