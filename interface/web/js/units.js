function unitsSort() {

  // Sort alphabetically
  $('.unit-list').append($('.unit-list .unit').sort(function (a, b) {
    var nameA = $(a).find('.unit-name').text();
    var nameB = $(b).find('.unit-name').text();
    return nameA.localeCompare(nameB);
  }));

}
