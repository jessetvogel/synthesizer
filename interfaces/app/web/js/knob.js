var knob = {

  create: function (id) { return new function () {
    // Variables
    this.element = $(document.getElementById(id));
    this.midiCC = -1;
    this.value = 0.0;
    this.detecting = false;

    this.onChangeValue = function () {};

    // Visuals
    this.element.addClass('knob');

    var container = $('<div>').addClass('knob-container');
    this.element.append(container);

    var circle = $('<div>').addClass('circle');
    (function (_) { circle.click(function () { if(!_.detecting) _.midiCC = -1; _.detect(!_.detecting); }); })(this);
    container.append(circle);

    var popup = $('<div>').addClass('popup')
      .css({ left: (this.element.width() + 8) + 'px', top: (this.element.height() / 2 - 32) + 'px' });
    container.append(popup);

    container.append($('<div>').addClass('text'));

    // Methods
    this.setValue = function (value) {
      this.value = value;
      this.element.find('.circle').css({ transform: 'rotate(' + (-150 + 300 * value) + 'deg)' });
      this.onChangeValue();
      return this;
    }

    this.setText = function (text) {
      this.element.find('.text').text(text);
      return this;
    }

    this.detect = function (x) {
      if(x === true || x === undefined) {
        this.detecting = true;
        this.element.find('.circle').addClass('detecting');
        popup.text('...');
      }
      else {
        this.detecting = false;
        this.element.find('.circle').removeClass('detecting');
        popup.text('' + (this.midiCC == -1 ? 'none' : this.midiCC));
      }
    }

    this.setValue(0.0);
    this.detect(false);

    // Let it respond to midi
    (function (_) { midi.onEvent(function (event) {
      if(event.type == midi.CONTROL_CHANGE && _.detecting) {
          _.midiCC = event.data1;
          _.detect(false);
      }

      if(event.type == midi.CONTROL_CHANGE && event.data1 == _.midiCC) {
        _.setValue(event.data2 / 127.0);
      }
    }); })(this);

  };},

};
