$(document).ready(function () {

  loadInstrumentData(function () {
    window.onbeforeunload = storeInstrumentData;
    setInterval(storeInstrumentData, 60000);
  });

});

function loadInstrumentData(callback) {
  (function (callback) {
    api.getInstrumentData(function (data) {
      parameters.setData(data.parameters);
      callback();
    });
  })(callback);
}

function storeInstrumentData() {
  api.setInstrumentData({
      parameters: parameters.getData()
  });
}
