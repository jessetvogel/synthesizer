var controls = {

  active: false,

  initialize: function () {
    // Start button
    $('.controls-button-start').click(function () {
      if(!controls.active) {
        api.send('/api/start');
        controls.active = true;

        $('.controls-button-start').addClass('button-pressed');
        $('.controls-button-stop').removeClass('button-pressed');
      }
    });

    // Stop button
    $('.controls-button-stop').click(function () {
      if(controls.active) {
        api.send('/api/stop');
        controls.active = false;

        $('.controls-button-stop').addClass('button-pressed');
        $('.controls-button-start').removeClass('button-pressed');
      }
    });

    // Fullscreen button
    $('.controls-button-fullscreen').click(function () {
      fullscreen.toggle($('body').get(0));
    });
  }

};
