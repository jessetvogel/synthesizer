var messages = {

  addInfo: function (message) {
    var info = $('<div>').addClass('message info');
    info.append($('<span>').addClass('glyphicon glyphicon-info-sign'));
    info.append(message);
    $('.message-container').append(info);
    (function (e) {
      setTimeout(function() {
        e.fadeOut(1000, function() { $(this).remove(); });
      }, 2000);
    })(info);
  },

  addWarning: function (message) {
    var warning = $('<div>').addClass('message warning');
    warning.append($('<span>').addClass('	glyphicon glyphicon-exclamation-sign'));
    warning.append(message);
    $('.message-container').append(warning);
    (function (e) {
      setTimeout(function() {
        e.fadeOut(1000, function() { $(this).remove(); });
      }, 2000);
    })(warning);
  },

  addError: function (message) {
    var error = $('<div>').addClass('message error');
    error.append($('<span>').addClass('glyphicon glyphicon-alert'));
    error.append(message);
    $('.message-container').append(error);
    (function (e) {
      setTimeout(function() {
        e.fadeOut(1000, function() { $(this).remove(); });
      }, 2000);
    })(error);
  }

};
