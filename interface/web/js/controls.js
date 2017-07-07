var controls = {

  initialize: function () {
    // Start / stop button
    $('.controls-button-play').click(function () {
      api.command('play toggle');
    });

    // Device button
    $('.controls-button-devices').click(function () {
      $('#views > .row').hide();
      $('#devices').show();
      $('.button-pressed').removeClass('button-pressed');
      $(this).addClass('button-pressed');
    });

    // Options button
    $('.controls-button-options').click(function () {
      $('#views > .row').hide();
      $('#options').show();
      $('.button-pressed').removeClass('button-pressed');
      $(this).addClass('button-pressed');
    });

    // Components button
    $('.controls-button-components').click(function () {
      $('#views > .row').hide();
      $('#components').show();
      $('.button-pressed').removeClass('button-pressed');
      $(this).addClass('button-pressed');
    });

    // Fullscreen button
    $('.controls-button-fullscreen').click(function () {
      fullscreen.toggle($('body').get(0));
    });

  }

};
