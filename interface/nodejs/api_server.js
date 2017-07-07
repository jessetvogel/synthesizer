const settings = require('./settings');
const url = require('url');

module.exports = {

  handle: function (request, response, synthesizer) {
    // Check if valid request url
    const mUrl = url.parse(request.url, true);
    const pattern = /^\/api\/command$/;
    var match = pattern.exec(mUrl.pathname);
    if(match == null)
      return false;

    // Find command (sent as ?q=<command>)
    var query = mUrl.query;
    var command = query.q;
    if(command == null) {
      bad_request(response);
      return true;
    }

    // Execute command
    synthesizer.command(command, response);
    return true;
  }

};

function bad_request(response) {
  response.writeHead(400, {'Content-Type': 'text/plain'});
  response.write(settings.error.BAD_REQUEST);
  response.end();
}
