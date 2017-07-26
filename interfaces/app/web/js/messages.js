function messagesAdd(div) {
  $('#messages').prepend(div);
  (function (e) {
    setTimeout(function() {
      e.fadeOut(1000, function() { $(this).remove(); });
    }, 2000);
  })(div);
}

var messages = {
  addInfo: function (message) {
    var info = $('<div>').addClass('message info');
    info.append($('<span>').text(message));
    messagesAdd(info);
  },

  addWarning: function (message) {
    var warning = $('<div>').addClass('message warning');
    warning.append($('<span>').text(message));
    messagesAdd(warning);
  },

  addError: function (message) {
    var error = $('<div>').addClass('message error');
    error.append($('<span>').text(message));
    messagesAdd(error);
  }
};
