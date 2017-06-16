$(document).ready(function () {

  // Refresh all devices
  devices.refreshAllDevices();

  // List item selection
  $('.list-item').click(function () {
    $(this).siblings('.list-item').removeClass('list-item-selected');
    $(this).addClass('list-item-selected');
  });

  // Unit search field
  $('.unit-search').on('input', function () {
    var query = $(this).val();
    $(this).parent().parent().parent().find('.unit').each(function () {
      if((query == '') || ($(this).find('.unit-name').text().indexOf(query) != -1))
        $(this).show();
      else
        $(this).hide();
    });
  });

  // Unit sort button
  $('.unit-button-sort').click(unitsSort);



  // TODO: remove this maybe?
  $('.tile-header').click(function () {
    $(this).next().slideToggle(200);
  });

});

function parseResponse(response) {
  // Parse the response
  var data = JSON.parse(response);

  for(var key in data) {
    // Skip loop if the property is from prototype
    if (!data.hasOwnProperty(key)) continue;

    switch(key) {
      case 'error':
        alert(data[key]);
        break;

      case 'midiDevices':
        devices.setMIDIDevices(data[key]);
        break;

      case 'outputDevices':
        devices.setOutputDevices(data[key]);
        break;
    }
  }
}
