var parameters = {

  list: [],

  getData: function () {
    var data = [];
    for(var i = 0;i < parameters.list.length;i ++) {
      var parameter = parameters.list[i];
      if(parameter.id == null) continue;
      data.push({
        id: parameter.id,
        midiCC: parameter.midiCC,
        value: parameter.value
      });
    }
    return data;
  },

  setData: function (data) {
    if(data == undefined || data == null) return;
    for(var i = 0;i < data.length;i ++) {
      var parameter = parameters.get(data[i].id);
      if(parameter != null)
        parameter.setMIDICC(data[i].midiCC).setValue(data[i].value)
      if(parameter.type == 'knob' || parameter.type == 'slider')
        parameter.pushValue();
    }
  },

  get: function (id) {
    for(var i = 0;i < parameters.list.length;i ++) {
      if(parameters.list[i].id == id)
        return parameters.list[i];
    }
    return null;
  }

};
