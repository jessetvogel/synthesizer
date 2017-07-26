const settings = require('./settings');
const server = require('./server');
const fileServer = require('./file_server');
const apiServer = require('./api_server');
const synthesizer = require('./synthesizer');

// Start executable
synthesizer.start();

server.start(function (request, response) {

  // Try to see who can handle the request
  if(fileServer.handle(request, response))
    return;

  if(apiServer.handle(request, response, synthesizer))
    return;

  // If request not recognised, respond with 404
  response.writeHead(404, {'Content-Type': 'text/plain'});
  response.write(settings.error.BAD_REQUEST);
  response.end();

});
