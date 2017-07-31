$(document).ready(function () {
  // Initialize MIDI
  if(window.midi) midi.initialize();

  // Navigation
  $('#button-settings').click(function () {
    window.open("/settings", "_blank");
  });

  $('#button-home').click(function () {
    window.location.href = "/";
  });

  $('#button-fullscreen').click(function () {
    fullscreen.toggle(document.body);
  })

  // Toggle boxes
  $('.toggle-box .title').prepend($('<span>').addClass('glyphicon glyphicon-triangle-right')).click(function () {
    var content = $(this).next();
    if(content.is(':visible')) {
      content.slideUp();
      $(this).find('span').css({'transform': 'rotate(0deg)'});
    }
    else {
      content.slideDown();
      $(this).find('span').css({'transform': 'rotate(90deg)'});
    }
  });

});
