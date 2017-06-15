const settings = require('./settings');
const fs = require('fs');

module.exports = {

  handle: function (request, response) {
    // Check if valid request url
    const pattern = /^(\/|\/(css|fonts|img|js)\/.+)$/;
    if(!pattern.test(request.url))
      return false;

    // Construct path to file
    var path = settings.webDirectory + (request.url == '/' ? '/index.html' : request.url);

    // Check if the file exists
    if(!fs.existsSync(path))
      return false;

    // Read the file and send it as a response
    fs.readFile(path, 'binary', function(error, file) {
      // If an error occured while reading, send 500
      if(error) {
        response.writeHead(500, {'Content-Type': 'text/plain'});
        response.write(error + '\n');
        response.end();
        return false;
      }

      response.writeHead(200); // TODO: send correct Content-Type
      response.write(file, 'binary');
      response.end();
      return true;
    });

    return true;
  }

};
