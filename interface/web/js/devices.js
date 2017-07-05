var devices = {

  // Request methods
  refreshMIDIDevices: function () {
    api.send('status midi_devices');
  },

  refreshInputDevices: function () {
    api.send('status input_devices');
  },

  refreshOutputDevices: function () {
    api.send('status output_devices');
  },

  refreshAllDevices: function () {
    api.send('status midi_devices input_devices output_devices');
  },

  // Handle response methods
  setMIDIDevices: function (data) {

    $('.midi-device-container').html('');

    for(var i = 0;i < data.length;i ++) {
        var midiDevice = $('<label>').addClass('midi-device');
        if(data[i].active)
          midiDevice.addClass('midi-device-active');
        midiDevice.append($('<div>').addClass('midi-device-id').text(data[i].id));
        midiDevice.append($('<div>').addClass('midi-device-icon').append($('<div>').addClass('glyphicon glyphicon-music')));
        midiDevice.append($('<div>').addClass('midi-device-name').text(data[i].name))
        midiDevice.append($('<div>').addClass('midi-device-options').append((function (device_id) {
          return $('<input>').attr('type', 'checkbox').prop('checked', data[i].active).change(function () {
            if($(this).is(":checked")) {
              $(this).parent().parent().addClass('midi-device-active');
              api.send('midi_add_device ' + device_id);
            }
            else {
              $(this).parent().parent().removeClass('midi-device-active');
              api.send('midi_remove_device ' + device_id);
            }
            });
        })(data[i].id)));

        $('.midi-device-container').append(midiDevice);
    }

  },

  setInputDevices: function (data) {

    $('.input-device-container').html('');

    for(var i = 0;i < data.length;i ++) {
        var inputDevice = $('<div>').addClass('audio-device list-item');
        inputDevice.append($('<div>').addClass('audio-device-id').text(data[i].id));
        inputDevice.append($('<div>').addClass('audio-device-icon').append($('<div>').addClass('glyphicon glyphicon-volume-up')));
        inputDevice.append($('<div>').addClass('audio-device-name').text(data[i].name))
        inputDevice.append($('<div>').addClass('audio-device-options'));

        (function (n) {
          inputDevice.click(function () {
            api.send('audio_set_input_device ' + n);
          });
        })(data[i].id);

        $('.input-device-container').append(inputDevice);
    }

    $('.input-device-container .list-item').click(function () {
      $(this).siblings('.list-item').removeClass('list-item-selected');
      $(this).addClass('list-item-selected');
    });

  },

  setOutputDevices: function (data) {

    $('.output-device-container').html('');

    for(var i = 0;i < data.length;i ++) {
        var outputDevice = $('<div>').addClass('audio-device list-item');
        outputDevice.append($('<div>').addClass('audio-device-id').text(data[i].id));
        outputDevice.append($('<div>').addClass('audio-device-icon').append($('<div>').addClass('glyphicon glyphicon-volume-up')));
        outputDevice.append($('<div>').addClass('audio-device-name').text(data[i].name))
        outputDevice.append($('<div>').addClass('audio-device-options'));

        (function (n) {
          outputDevice.click(function () {
            api.send('audio_set_output_device ' + n);
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
