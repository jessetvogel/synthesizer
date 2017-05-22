var drawbars = {

  initialize: function () {
    $('#drawbar-1').data('n', 1);
    $('#drawbar-2').data('n', 2);
    $('#drawbar-3').data('n', 3);
    $('#drawbar-4').data('n', 4);
    $('#drawbar-5').data('n', 5);
    $('#drawbar-6').data('n', 6);
    $('#drawbar-7').data('n', 7);
    $('#drawbar-8').data('n', 8);
    $('#drawbar-9').data('n', 9);

    $('.drawbar').append($('<div>').addClass('lever'));
    $('.lever').append($('<div>').addClass('stick'));
    $('.lever').append($('<div>').addClass('button'));
  },

  update: function (id, value) {
    // Convert value into amplitude
    var amplitude;
    if(value == 0)
      amplitude = 0.0;
    else
      amplitude = 1.0 * Math.pow(0.5, 4.0 * (1.0 - value));

    connection.send('unit_set_value drawbar_' + id + ' value ' + amplitude);
  }
};
