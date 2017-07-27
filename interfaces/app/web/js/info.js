var info = {

  request: function (type, callback) {
    $.ajax('/info/' + type).done(callback);
  }
  
};
