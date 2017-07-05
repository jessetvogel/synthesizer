var api = {

  send: function (command) {
    $.ajax('/api?q=' + command).done(parseResponse);
  }

};
