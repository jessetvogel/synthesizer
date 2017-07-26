const settings = require('./settings');
const childProcess = require('child_process');

var child = null;
var buffer = "";
var responses = [];

module.exports = {

  start: function () {
    // Spawn executable
    child = childProcess.spawn(settings.executablePath);

    child.stdout.on('data', parse);
  },

  command: function (command, r) {
    if(r == null) // TODO: remove this. it is for debug reasons
      console.log('WARNING!');

    // Check if the executable is running
    if(child == null) {
      console.log('Cannot execute command as program has not yet started');
      return false;
    }

    // Send command, and save response
    responses.push(r);
    child.stdin.write(command + '\n');
    return true;
  }

};

function parse(data) {
  // Convert data to string
  output = data.toString();

  // If no response, print it to console. TODO: save it, and return it whenever it is asked for?
  if(responses.length == 0) {
    console.log('No response set!');
    console.log(output);
    return;
  }

  // Append output to buffer
  buffer += output;

  // Send output to current response
  responses[0].write(output);

  try { // TODO: fix this, really. It should check whether this is the full response, or a partial response. Check for some newline maybe?
    JSON.parse(buffer);
    responses[0].end();
    responses.splice(0, 1);
    buffer = "";
  }
  catch(e) {

  }
}
