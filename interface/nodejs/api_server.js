const settings = require('./settings');
const url = require('url');

module.exports = {

  handle: function (request, response, synthesizer) {
    // Check if valid request url
    const mUrl = url.parse(request.url, true);
    const pattern = /^\/api\/(\w+)$/;
    var match = pattern.exec(mUrl.pathname);
    if(match == null)
      return false;

    var query = mUrl.query;

    switch(match[1]) {

      case 'unit_create':
        [type, id, arguments] = [query.type, query.id, query.arguments == null ? [] : query.arguments.split(',')];
        if(type == null || id == null || arguments == null) { bad_request(response); break; }
        synthesizer.command('unit_create ' + type + ' ' + id + ' ' + arguments.join(' '), response);
        break;

      case 'unit_rename':
        [oldId, newId] = [query.old_id, query.new_id];
        if(oldId == null || newId == null) { bad_request(response); break; }
        synthesizer.command('unit_rename ' + oldId + ' ' + newId, response);
        break;

      case 'unit_delete':
        id = query.id;
        if(id == null) { bad_request(response); break; }
        synthesizer.command('unit_delete ' + id, response);
        break;

      case 'unit_set_value':
        [id, label, value] = [query.id, query.label, query.value];
        if(id == null || label == null || value == null) { bad_request(response); break; }
        synthesizer.command('unit_set_value ' + id + ' ' + label + ' ' + value, response);
        break;

      case 'status':
        info = query.info == null ? null : query.info.split(',');
        if(info == null) { bad_request(response); break; }
        synthesizer.command('status ' + info.join(' '), response);
        break;

      case 'start':
        outputDevice = query.output_device;
        if(outputDevice == null) outputDevice = '';
        synthesizer.command('start ' + outputDevice, response);
        break;

      case 'stop':
        synthesizer.command('stop', response);
        break;

      case 'midi_add_input_device':
        midiDevice = query.midi_device;
        if(midiDevice == null) { bad_request(response); break; }
        synthesizer.command('midi_add_input_device ' + midiDevice, response);
        break;

      case 'midi_remove_input_device':
        midiDevice = query.midi_device;
        if(midiDevice == null) { bad_request(response); break; }
        synthesizer.command('midi_remove_input_device ' + midiDevice, response);
        break;

      default:
        bad_request(response);
        break;
    }

    return true;
  }

};

function bad_request(response) {
  response.writeHead(400, {'Content-Type': 'text/plain'});
  response.write(settings.error.BAD_REQUEST);
  response.end();
}
