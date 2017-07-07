var state = {

  // Request methods
  refreshState: function () {
    api.command('status state');
  },

  // Handle response methods
  update: function (data) {

    var playing = data['playing'];
    if(playing === true)
      $('.controls-button-play').html('<span class="glyphicon glyphicon-stop"></span>');
    if(playing === false)
      $('.controls-button-play').html('<span class="glyphicon glyphicon-play"></span>');

  }

};
