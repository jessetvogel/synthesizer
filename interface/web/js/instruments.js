var instruments = {

  // Request methods
  refreshInstruments: function () {
    api.send('status instruments');
  },

  // Handle response methods
  setInstruments: function (data) {

    $('.instrument-container').html('');

    for(var i = 0;i < data.length;i ++) {
      var instrument = $('<div>').addClass('instrument list-item')
      instrument.append($('<div>').addClass('instrument-id').text(data[i].id));

      (function (data) {
        instrument.click(function () { instruments.setData(data); });
      })(data[i]);

      $('.instrument-container').append(instrument);
    }

    $('.instrument-container .list-item').click(function () {
      $(this).siblings('.list-item').removeClass('list-item-selected');
      $(this).addClass('list-item-selected');
    });

  },

  setData: function (data) {

    $('.instrument-parameter-container').html('');

    var parameters = ["key_output", "output"];

    for(var i = 0;i < parameters.length; i++) {
      var parameter = $('<div>').addClass('instrument-parameter');
      parameter.append($('<div>').addClass('instrument-parameter-label').text(parameters[i]));
      parameter.append($('<div>').addClass('instrument-parameter-value').append(
          $('<div>').addClass('instrument-parameter-input').append(
            (function (data, parameter) {
              return $('<input>').val(data[parameter]).change(function () {
              api.send('instrument_set_' + parameter + ' ' + data.id + ' ' + $(this).val());
            })})(data, parameters[i]))));

      $('.instrument-parameter-container').append(parameter);
    }
  },

  // Other methods
  sort: function () {
    // Sort alphabetically
    $('.unit-container').append($('.unit-container .unit').sort(function (a, b) {
      var nameA = $(a).find('.unit-id').text();
      var nameB = $(b).find('.unit-id').text();
      return nameA.localeCompare(nameB);
    }));
  },

  search: function (query) {
    $('.unit').each(function () {
      if((query == '') || ($(this).find('.unit-id').text().indexOf(query) != -1))
        $(this).show();
      else
        $(this).hide();
    });
  }

};
