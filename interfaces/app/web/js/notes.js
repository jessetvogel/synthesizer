var notes = {

  name: function (note) {
    var notes = ['C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B'];
    var name = notes[Math.round(note) % 12];
    var octave = Math.floor(Math.round(note) / 12);
    return name + octave;
  },

  fromName: function (name) {
    var semitones = -1;
    switch(name.toUpperCase().replace(/\d/, '')) {
      case 'C': semitones = 0; break;
      case 'C#': case 'DB': semitones = 1; break;
      case 'D': semitones = 2; break;
      case 'D#': case 'EB': semitones = 3; break;
      case 'E': semitones = 4; break;
      case 'F': semitones = 5; break;
      case 'F#': case 'GB': semitones = 6; break;
      case 'G': semitones = 7; break;
      case 'G#': case 'AB': semitones = 8; break;
      case 'A': semitones = 9; break;
      case 'A#': case 'BB': semitones = 10; break;
      case 'B': semitones = 11; break;
    }
    if(semitones == -1) return -1;
    var octave = parseInt(name.replace(/\D+/, ''));
    if(octave != octave) return -1; // Check for NaN
    return 12 + 12 * octave + semitones;
  },

  isSharp: function (note) {
    var _ = note % 12;
    return _ == 1 || _ == 3 || _ == 6 || _ == 8 || _ == 10;
  }

};
