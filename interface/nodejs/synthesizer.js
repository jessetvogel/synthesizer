const settings = require('./settings');
const childProcess = require('child_process');

var child = null;
var response = null;

module.exports = {

  start: function () {
    // Spawn executable
    child = childProcess.spawn(settings.executablePath);

    child.stdout.on('data', parse);
  },

  command: function (command, r) {
    // Check if the executable is running
    if(child == null) {
      console.log('Cannot execute command as program has not yet started');
      return false;
    }

    // Send command, and save response
    child.stdin.write(command + '\n');
    response = r;
    return true;
  }

};

function parse(output) {
  // If no response, print it to console. TODO: save it, and return it whenever it is asked for?
  if(response == null) {
    console.log(JSON.parse(output));
    return;
  }

  // Send output to response
  response.write(output);
  response.end();
  response = null;
}
