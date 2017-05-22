// Child process
var spawn = require('child_process').spawn;

synthesizer = spawn('./bin/synthesizer');

synthesizer.stdout.on('data', function (data) {
  process.stdout.write(data.toString());
});

synthesizer.stdout.on('end', function(){
  process.stdout.write('Program ended\n');
  process.exit(0);
});

// Websocket server
var WebSocketServer = require('websocket').server;
var http = require('http');

// Define options, i.e. ports, adressess etc.
var portWebsocket = 1337;
var portHTTP = 8080;
var protocolWebsocket = 'hammond-protocol';

// Create a HTTP server for websocket server
var server = http.createServer(function (request, response) {
    response.writeHead(404);
    response.end();
}).listen(portWebsocket, function () {
    serverLog('Server is listening on port ' + portWebsocket);
});

// Create a websocket server
serverWebSocket = new WebSocketServer({
    httpServer: server,
    autoAcceptConnections: false
});

serverWebSocket.on('request', function (request) {
	// Make sure we only accept requests from an allowed origin
    if(!requestAllowed(request.origin)) {
      request.reject();
      serverLog('Connection from origin ' + request.origin + ' rejected');
      return;
    }

	// Try establishing a connection
	try {
		var connection = request.accept(protocolWebsocket, request.origin);
		serverLog('Connection accepted from origin ' + request.origin);

		connection.on('message', function (message) {
			// So far only support UTF8
			if(message.type === 'utf8') {
				serverLog(message.utf8Data);
        synthesizer.stdin.write(message.utf8Data + '\n');
        // synthesizer.stdin.end();
			}
		});

		connection.on('close', function (reasonCode, description) {
			serverLog('Peer ' + connection.remoteAddress + ' disconnected');
		});
	} catch(err) {
		serverLog(err);
	}
});

function serverLog(message) {
  // console.log('[SERVER] ' + message);
}

function requestAllowed(origin) {
  // Check whether the request is allowed
  return true; // TODO: make criterion, maybe if source is local?
}
