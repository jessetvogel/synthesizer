$(document).ready(function () {

  info.request("instruments", function (data) {
    console.log(data);
    var instruments = data.instruments;
    for(var i = 0;i < instruments.length;i ++) {
      var instrument = $('<div>')
        .addClass('button large')
        .text(instruments[i]);

      (function (x) { instrument.click(function () {
        window.location = '/instrument/' + x + '/';
      });})(instruments[i]);

      $('#instruments').append(instrument);
    }
  });

});
