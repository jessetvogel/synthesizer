$(document).ready(function () {
  // Initialize controls
  controls.initialize();

  // Refresh all devices
  devices.refreshAllDevices();

  // Refresh all units
  units.refreshUnits();

  // Unit search field
  $('.unit-search').on('input', function () { units.search($(this).val()); });

  // Unit sort button
  $('.unit-button-sort').click(units.sort);

  // TODO: remove this maybe?
  $('.tile-header').click(function () {
    $(this).next().slideToggle(200);
  });

});

function parseResponse(response) {
  // Parse the response
  console.log(response);

  var data = JSON.parse(response);

  for(var key in data) {
    // Skip loop if the property is from prototype
    if (!data.hasOwnProperty(key)) continue;

    switch(key) {
      case 'errors':
        if(data[key].length == 0) break;

        errors.add(data[key]);
        break;

      case 'midiDevices':
        devices.setMIDIDevices(data[key]);
        break;

      case 'outputDevices':
        devices.setOutputDevices(data[key]);
        break;

      case 'units':
        units.setUnits(data[key]);
        break;
    }
  }
}
