var devices = {

  // Constants
  outputDevice: -1,

  // Request methods
  refreshMIDIDevices: function () {
    $.ajax('/api/status?info=midi_devices').done(parseResponse);
  },

  refreshOutputDevices: function () {
    $.ajax('/api/status?info=midi_devices').done(parseResponse);
  },

  refreshAllDevices: function () {
    $.ajax('/api/status?info=midi_devices,output_devices').done(parseResponse);
  },

  // Handle response methods
  setMIDIDevices: function (data) {

    $('.midi-device-container').html('');

    for(var i = 0;i < data.length;i ++) {
        var midiDevice = $('<div>').addClass('midi-device');
        midiDevice.append($('<div>').addClass('midi-device-id').text(data[i].id));
        midiDevice.append($('<div>').addClass('midi-device-icon').append($('<div>').addClass('glyphicon glyphicon-music')));
        midiDevice.append($('<div>').addClass('midi-device-name').text(data[i].name))
        midiDevice.append($('<div>').addClass('midi-device-options').append($('<input>').attr('type', 'checkbox').prop('checked', data[i].active)));

        $('.midi-device-container').append(midiDevice);
    }

    $('.midi-device-container .midi-device-options input').change(function () {
      // TODO
      console.log('s');
    });

  },

  setOutputDevices: function (data) {

    $('.output-device-container').html('');

    for(var i = 0;i < data.length;i ++) {
        var outputDevice = $('<div>').addClass('output-device list-item');
        outputDevice.append($('<div>').addClass('output-device-id').text(data[i].id));
        outputDevice.append($('<div>').addClass('output-device-icon').append($('<div>').addClass('glyphicon glyphicon-volume-up')));
        outputDevice.append($('<div>').addClass('output-device-name').text(data[i].name))
        outputDevice.append($('<div>').addClass('output-device-options'));

        (function (n) {
          outputDevice.click(function () {
            devices.outputDevice = n;
          });
        })(data[i].id);

        $('.output-device-container').append(outputDevice);
    }

    $('.output-device-container .list-item').click(function () {
      $(this).siblings('.list-item').removeClass('list-item-selected');
      $(this).addClass('list-item-selected');
    });

  }

};
