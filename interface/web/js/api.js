var api = {

  send: function (url) {
    $.ajax(url).done(parseResponse);
  }

};
