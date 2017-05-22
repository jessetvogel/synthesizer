var connection = {

  webSocket: null,
  location: 'ws://192.168.2.2:1337',
  protocol: 'hammond-protocol',

  connect: function () {
    connection.webSocket = new WebSocket(connection.location, connection.protocol);
    log.print('Trying to connect...', log.color.yellow);

    connection.webSocket.onclose = function () {
      log.print('Connection closed or failed to connect. Attempt to reconnect in 5 sec.', log.color.red);
      setTimeout(connection.connect, 5000);
    };

    connection.webSocket.onopen = function () {
      log.print('Connection established.', log.color.green);

      // Load hammond
      connection.send('include synth/hammond.synth');
      connection.send('start');
    };

    connection.webSocket.onmessage = function (e) {
      log.print(e.data, log.color.blue);
    };
  },

  send: function (data) {
    try {
  		connection.webSocket.send(data + '\n');
  	} catch(e) {
  		log.print('Unable to send \'' + data + '\'');
  	}
  }
};
