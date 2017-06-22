var units = {

  // Request methods
  refreshUnits: function () {
    api.send('/api/status?info=units');
  },

  // Handle response methods
  setUnits: function (data) {

    $('.unit-container').html('');

    for(var i = 0;i < data.length;i ++) {
      var unit = $('<div>').addClass('unit list-item')
      if(data[i].keyDependent == 'true')
        unit.addClass('unit-type-key-dependent');
      unit.append($('<div>').addClass('unit-type').text(data[i].type));
      unit.append($('<div>').addClass('unit-id').text(data[i].id));

      (function (id, parameters) {
        unit.click(function () { units.setParameters(id, parameters); });
      })(data[i].id, data[i].parameters);

      $('.unit-container').append(unit);
    }

    $('.unit-container .list-item').click(function () {
      $(this).siblings('.list-item').removeClass('list-item-selected');
      $(this).addClass('list-item-selected');
    });

  },

  setParameters: function (id, parameters) {

    $('.unit-parameter-container').html('');

    for(var i = 0;i < parameters.length;i ++) {
      var parameter = $('<div>').addClass('unit-parameter');
      parameter.append($('<div>').addClass('unit-parameter-label').text(parameters[i].label));
      parameter.append($('<div>').addClass('unit-parameter-value').append(
        $('<div>').addClass('unit-parameter-input').append(
          (function (id, parameter) {
            return $('<input>').val(parameter.value).change(function () {
            api.send('/api/unit_set_value?id=' + id + '&label=' + parameter.label + '&value=' + $(this).val());
          })})(id, parameters[i]))));

      $('.unit-parameter-container').append(parameter);
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
