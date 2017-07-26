$(document).ready(function () {

  midi.initialize();

  $('#button-settings').click(function () {
    window.open("/settings", "_blank");
  });

  $('#button-fullscreen').click(function () {
    fullscreen.toggle(document.body);
  })

});
