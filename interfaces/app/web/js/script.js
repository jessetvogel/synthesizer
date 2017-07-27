$(document).ready(function () {

  if(window.midi) midi.initialize();

  $('#button-settings').click(function () {
    window.open("/settings", "_blank");
  });

  $('#button-fullscreen').click(function () {
    fullscreen.toggle(document.body);
  })

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
