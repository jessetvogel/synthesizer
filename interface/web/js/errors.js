var errors = {

  add: function (message) {
    var error = $('<div>').addClass('error');
    error.append($('<span>').addClass('glyphicon glyphicon-alert'));
    error.append(message);
    $('.error-container').append(error);
    (function (e) {
      setTimeout(function() {
        e.fadeOut(1000, function() { $(this).remove(); });
      }, 2000);
    })(error);
  }

};
