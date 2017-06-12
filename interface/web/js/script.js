$(document).ready(function () {

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
