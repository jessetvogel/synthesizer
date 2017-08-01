var api = {

  command: function (command, callback, fail) {
    console.log('[COMMAND] ' + command);
    if(callback == undefined) callback = api.handle;
    var request = $.ajax('/api/' + command.replace(/\s+/g, '+')).done(callback); // TODO
    if(fail !== undefined) request.fail(fail);
  },

  restart: function (callback, fail) {
    console.log('[RESTART]');
    if(callback == undefined) callback = api.handle;
    var request = $.ajax('/restart').done(callback);
    if(fail !== undefined) request.fail(fail);
  },

  exit: function (callback, fail) {
    console.log('[EXIT]');
    if(callback == undefined) callback = api.handle;
    var request = $.ajax('/exit').done(callback);
    if(fail !== undefined) request.fail(fail);
  },

  setInstrumentData: function (data, callback) {
    if(callback == undefined) callback = api.handle;
    $.ajax({
      method: 'POST',
      url: '/data/instrument/set',
      data: JSON.stringify(data),
      contentType: 'application/json'
    }).done(callback);
  },

  getInstrumentData: function (callback) {
    $.ajax('/data/instrument/get').done(callback);
  },

  getInfo: function (info, callback) {
    $.ajax('/info/' + info).done(callback);
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
