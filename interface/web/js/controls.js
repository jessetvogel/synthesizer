var controls = {

  active: false,

  initialize: function () {
    // Start button
    $('.controls-button-start').click(function () {
      if(!controls.active) {
        $.ajax('/api/start' + (devices.outputDevice == -1 ? '' : '?outputDevice=' + devices.outputDevice)).done(parseResponse);
        controls.active = true;

        $('.controls-button-start').addClass('button-pressed');
        $('.controls-button-stop').removeClass('button-pressed');
      }
    });

    // Stop button
    $('.controls-button-stop').click(function () {
      if(controls.active) {
        $.ajax('/api/stop').done(parseResponse);
        controls.active = false;

        $('.controls-button-stop').addClass('button-pressed');
        $('.controls-button-start').removeClass('button-pressed');
      }
    });
  }

};
