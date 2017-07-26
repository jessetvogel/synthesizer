$(document).ready(function () {
  // Initialize everything
  controls.initialize();
  files.initialize();
  devices.refreshAllDevices();
  units.refreshUnits();
  state.refreshState();
  components.initialize();

  // By default, click units button
  $('.controls-button-units').click();


  // Unit search field
  $('.unit-search').on('input', function () { units.search($(this).val()); });

  // Unit sort button
  $('.unit-button-sort').click(units.sort);

  // TODO: remove this maybe?
  $('.tile-header').click(function () {
    $(this).next().slideToggle(200);
  });

});
