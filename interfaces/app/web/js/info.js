var info = {

  request: function (type, callback) {
    $.ajax('/info/' + type).done(callback);
  }

};
// TODO: delete this file, if needed, move it to api.getInfo or something
