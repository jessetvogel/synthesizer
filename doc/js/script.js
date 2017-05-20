$(document).ready(function () {

  var content = $('#content');

  // Commands
  content.append($('<span>').addClass('section-header').text('Commands').attr('id', 'header-commands'));
  var commands = documentation.commands;
  var c = $('<div>').attr('id', 'commands');
  for(var i = 0;i < commands.length;i ++) {
    var command = commands[i];
    var div = $('<div>').addClass('command');
    div.append($('<span>').addClass('command-rule').text(command.example));
    div.append($('<p>').addClass('command-description').text(command.description));
    c.append(div);
  }
  content.append(c);

  // Units
  content.append($('<span>').addClass('section-header').text('Units').attr('id', 'header-units'));
  var units = documentation.units;
  var u = $('<div>').attr('id', 'units');
  for(var i = 0;i < units.length;i ++) {
    var unit = units[i];
    var name = unit.name;
    var parameters = unit.parameters;
    var description = unit.description;

    var div = $('<div>').addClass('unit');

    div.append($('<span>').addClass('unit-name').text(name));
    div.append($('<p>').addClass('unit-description').text(description));

    var table = $('<table>').addClass('unit-parameters');
    for(var j = 0;j < parameters.length;j ++) {
      var parameter = parameters[j];
      var tr = $('<tr>');
      tr.append($('<td>').append($('<span>').addClass('unit-parameter-name').text(parameter.name)));
      tr.append($('<td>').append($('<span>').addClass('unit-parameter-value').text(parameter.value)));
      tr.append($('<td>').append($('<span>').addClass('unit-parameter-default').text(parameter.default)));
      table.append(tr);
    }
    div.append(table);

    u.append(div);
  }
  content.append(u);

  // Add awesum effects
  $('#header-commands').click(function () { $('#commands').slideToggle(); }).css({cursor: 'pointer'});
  $('#header-units').click(function () { $('#units').slideToggle(); }).css({cursor: 'pointer'});

});
