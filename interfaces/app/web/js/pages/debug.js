var monitor;
var monitorValue;
var interval;

$(document).ready(function () {
  // Create oscilloscope
  monitor = oscilloscope.create('oscilloscope');

  // Monitor input
  $('#input-monitor').change(function () {
    monitorValue = $(this).val();
  });

  // Monitor controls
  $('#monitor-play').click(monitorStart);
  $('#monitor-stop').click(monitorStop).addClass('disabled');

  // Nodes list
  refreshNodes();

  // Command prompt
  $('#command-prompt input').keypress(function (e) {
    if(e.which == 13) {
      $('#command-prompt input').prop('disabled', 'disabled');
      api.command($(this).val(), function (data) {
        $('#command-result').text(JSON.stringify(data, null, 2));
        $('#command-prompt input').prop('disabled', false).val("");
      });

    }
  });

});

function monitorUpdate() {
  api.command('monitor ' + monitorValue, function (data) {
    if(!monitor.setData(data.monitor)) monitorStop();
  }, monitorStop);
}

function monitorStart() {
  clearInterval(interval);
  monitorUpdate();
  interval = setInterval(monitorUpdate, 500);
  $('#monitor-play').addClass('disabled');
  $('#monitor-stop').removeClass('disabled');
}

function monitorStop() {
  clearInterval(interval);
  $('#monitor-play').removeClass('disabled');
  $('#monitor-stop').addClass('disabled');
}

function refreshNodes() {
  api.command('status nodes', function (data) {
    var nodes = data.nodes;
    for(var i = 0;i < nodes.length;i ++) {
      var div = $('<div>').text(nodes[i].id);
      (function (id) { div.click(function() { setNodeInfo(id) }); })(nodes[i].id);
      $('#node-list').append(div);
    }
  });
}

function setNodeInfo(id) {
  api.command('node_info ' + id, function (data) {
    var nodeInfo = $('#node-info').html('');
    var node = data.node;

    nodeInfo.append($('<div>').addClass('header').text(id));
    nodeInfo.append($('<div>').addClass('header').text('Inputs'));
    for(var i = 0;i < node.inputs.length;i ++)
      nodeInfo.append($('<div>').css({ display: 'flex' })
        .append($('<div>').css({ width: '128px'}).text(node.inputs[i].label))
        .append($('<div>').css({ width: '96px', textAlign: 'right' }).text(node.inputs[i].value)));

    nodeInfo.append($('<div>').addClass('header').text('Outputs'));
    for(var i = 0;i < node.outputs.length;i ++)
      nodeInfo.append($('<div>').text(node.outputs[i].label));
  });
}
