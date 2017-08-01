$(document).ready(function () {

  api.getInfo('instruments', function (data) {
    var instruments = data.instruments;
    for(var i = 0;i < instruments.length;i ++) {
      var instrument = $('<div>').text(instruments[i]);

      (function (x) { instrument.click(function () {
        window.location = '/instrument/' + x + '/';
      });})(instruments[i]);

      $('#instruments').append(instrument);
    }
  });

});
