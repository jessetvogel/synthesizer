const settings = require('./settings');
const http = require('http');

module.exports = {

  start: function (callback) {
    // Create http server object
    const server = http.createServer(callback);

    // Listen to port as defined in settings
    server.listen(settings.port, function (error) {
      if(error)
        console.log(error);
    });
  },

  stop: function () {

  }

};
