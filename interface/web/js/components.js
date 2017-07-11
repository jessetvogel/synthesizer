var components = {

  initialize: function () {

    $('.component-view').on('dragover', function (e) {
      e.originalEvent.preventDefault();
    }).on('drop', function (e) {
      var event = e.originalEvent;
      event.preventDefault();
      var data = event.dataTransfer.getData("text");

      var x = Math.floor((event.clientX - $(this).offset().left) / 64) * 64;
      var y = Math.floor((event.clientY - $(this).offset().top) / 64) * 64;

      var component = $('<div>').addClass('component');
      component.css({
        left: '' + x + 'px',
        top: '' + y + 'px'
      })
      component.text(data);

      component.click(function () {

        $('.component-parameters').show();
        $('.component-parameter-list').text($(this).text());


      });

      $(this).append(component);
    });

    $('.component-list .list-item')
      .prop('draggable', 'true')
      .on('dragstart', function (e) {
        e.originalEvent.dataTransfer.setData("text", $(this).text());
      });



    // Navigating
    $('html').on('keydown', function (event) {
      var dx = 0;
      var dy = 0;
      switch(event.which) {
        case 37: dx = 1; break;
        case 38: dy = 1; break;
        case 39: dx = -1; break;
        case 40: dy = -1; break;
      }

      if(dx == 0 && dy == 0) return;

      var bpx = parseFloat($('.component-view').css('background-position-x'));
      var bpy = parseFloat($('.component-view').css('background-position-y'));

      $('.component-view').animate({
        'background-position-x': (bpx + dx*64) + 'px',
        'background-position-y': (bpy + dy*64) + 'px'
      }, 100, "linear");

      $('.component-view .component').each(function () {
        var x = parseFloat($(this).css('left'));
        var y = parseFloat($(this).css('top'));

        $(this).animate({
          'left': (x + dx*64) + 'px',
          'top': (y + dy*64) + 'px'
        }, 100, "linear");
      });

    });
  }

};
