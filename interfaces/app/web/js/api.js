var api = {

  command: function (command, callback) {
    if(callback == undefined) callback = api.handle;
    (function (callback) {
      $.ajax('/api/' + command.replace(/\s+/g, '+')).done(function (data) { // TODO: fix this
        callback(data);
      });
    })(callback);
  },

  handle: function (data) {
    for(var key in data) {
      // Skip loop if the property is from prototype
      if (!data.hasOwnProperty(key)) continue;

      switch(key) {
        case 'info':
          for(var i = 0;i < data[key].length;i ++)
            messages.addInfo(data[key][i]['message']);
          break;

        case 'warning':
          for(var i = 0;i < data[key].length;i ++)
            messages.addWarning(data[key][i]['message']);
          break;

        case 'error':
          for(var i = 0;i < data[key].length;i ++)
            messages.addError(data[key][i]['message']);
          break;

        case 'midiDevices':
          midi.setDevices(data[key]);
          break;

        // case 'inputDevices':
        //   devices.setInputDevices(data[key]);
        //   break;
        //
        // case 'outputDevices':
        //   devices.setOutputDevices(data[key]);
        //   break;
      }
    }
  }
};
