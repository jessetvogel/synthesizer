var settings;

$(document).ready(function () {

  api.command('status midi_devices input_devices output_devices settings', function (data) {
    var midiDevices = data.midiDevices;
    for(var i = 0;i < midiDevices.length;i ++) {
      var midiDevice = $('<div>').addClass('input-line');
      var checkboxId = 'checkbox-midi-' + midiDevices[i].id;
      midiDevice.append('<label for="' + checkboxId + '"><input type="checkbox" id="' + checkboxId + '" ' + (midiDevices[i].active ? 'checked' : '') + '/><span><span class="glyphicon glyphicon-ok"></span></span><span>' + midiDevices[i].name + '</span></label>');
      $('#midi-devices').append(midiDevice);
      (function (id) { $('#' + checkboxId).change(function () {
        if($(this).is(':checked'))
          api.command('midi_add_device ' + id);
        else
          api.command('midi_remove_device ' + id);
      }); })(midiDevices[i].id);
    }

    var inputDevices = data.inputDevices;
    for(var i = 0;i < inputDevices.length;i ++) {
      var inputDevice = $('<div>').addClass('input-line');
      var radioId = 'radio-input-' + inputDevices[i].id;
      inputDevice.append('<label for="' + radioId + '"><input type="radio" name="input-device" id="' + radioId + '" ' + (inputDevices[i].active ? 'checked' : '') + '/><span><span>•</span></span></span><span>' + inputDevices[i].name + '</span></label>');
      $('#input-devices').append(inputDevice);
      (function (id) { $('#' + radioId).change(function () {
        if($(this).is(':checked'))
          api.command('audio_set_input_device ' + id);
      }); })(inputDevices[i].id);
    }

    var outputDevices = data.outputDevices;
    for(var i = 0;i < outputDevices.length;i ++) {
      var outputDevice = $('<div>').addClass('input-line');
      var radioId = 'radio-output-' + outputDevices[i].id;
      outputDevice.append('<label for="' + radioId + '"><input type="radio" name="output-device" id="' + radioId + '" ' + (outputDevices[i].active ? 'checked' : '') + '/><span><span>•</span></span></span><span>' + outputDevices[i].name + '</span></label>');
      $('#output-devices').append(outputDevice);
      (function (id) { $('#' + radioId).change(function () {
        if($(this).is(':checked'))
          api.command('audio_set_output_device ' + id);
      }); })(outputDevices[i].id);
    }

    settings = data.settings;
    settings.store = function () {
      $.ajax({
        method: 'POST',
        url: '/data/settings/store',
        data: 'settings_set sample_rate ' + settings.sampleRate + '\nsettings_set buffer_size ' + settings.bufferSize,
    	  contentType: 'text/plain',
        success: api.handle
      });
    }

    $('#input-sample-rate').val(settings.sampleRate).change(function () {
      settings.sampleRate = $(this).val();
      settings.store();
    });
    $('#input-buffer-size').val(settings.bufferSize).change(function () {
      settings.bufferSize = $(this).val();
      settings.store();
    });

  });

  // Buttons
  $('#button-restart').click(function () {
    $.ajax({ // TODO: make it api.restart()
      method: 'GET',
      url: '/restart'
    }).done(function (data) {
      if(data.error == undefined)
        window.location.reload();
    });
  });

  $('#button-exit').click(function () {
    $.ajax({ // TODO: make it api.exit()
      method: 'GET',
      url: '/exit'
    }).done(function (data) {
      if(data.error == undefined)
        window.close();
    });
  });

});
