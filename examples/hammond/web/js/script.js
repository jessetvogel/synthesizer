$(document).ready(function () {
  // Initialize
  log.initialize();
  drawbars.initialize();
  connection.connect();

  // Set fullscreen
  $('#fullscreen-overlay').click(function () {
    fullscreen($('#content').get(0));
  });

  // Make sliders out of the drawbars
  $('.drawbar').on('touchmove', function(event) {
    var touch = event.targetTouches[0];

    var height = $(this).height();
    var width = $(this).width();

    var minY = width / 2;
    var maxY = height - width / 2;

    var y = touch.pageY - $(this).offset().top;
    if(y < minY) y = minY;
    if(y > maxY) y = maxY;
    var value = (y - minY) / (maxY - minY);

    $(this).find('.lever').css({ top: (y - maxY) + "px" });

    drawbars.update($(this).data('n'), value);
  });
});
