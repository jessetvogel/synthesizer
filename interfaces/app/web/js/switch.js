var Switch = {

  create: function (id) { return new function () {
    // Variables
    this.type = 'switch';
    this.element = $(document.getElementById(id));
    this.midiCC = -1;
    this.value = false;
    this.detecting = false;
    this.id = null;

    this.handlerOn = function () {};
    this.handlerOff = function () {};

    // Add this switch to the list of parameters
    parameters.list.push(this);

    // Visuals
    this.element.addClass('switch');

    var container = $('<div>').addClass('switch-container');
    this.element.append(container);

    var box = $('<div>').addClass('box').append($('<div>').addClass('light'));
    (function (_) { box.click(function () { _.setMIDICC(-1); _.detect(!_.detecting); }); })(this);
    container.append(box);

    var popup = $('<div>').addClass('popup')
      .css({ left: (this.element.width() + 8) + 'px', top: (this.element.height() / 2 - 32) + 'px' });
    container.append(popup);

    container.append($('<div>').addClass('caption'));

    // Methods
    this.setId = function (id) {
      this.id = id;
      return this;
    }

    this.setMIDICC = function (midiCC) {
      this.midiCC = midiCC;
      popup.text('' + (this.midiCC == -1 ? 'none' : this.midiCC));
      return this;
    }

    this.setValue = function (value) {
      this.value = value;
      if(value) {
        this.handlerOn();
        this.element.find('.light').addClass('on');
      }
      else {
        this.handlerOff();
        this.element.find('.light').removeClass('on');
      }
      return this;
    }

    this.setCaption = function (caption) {
      this.element.find('.caption').text(caption);
      return this;
    }

    this.detect = function (x) {
      if(x === true || x === undefined) {
        this.detecting = true;
        this.element.find('.box').addClass('detecting');
        popup.text('...');
      }
      else {
        this.detecting = false;
        this.element.find('.box').removeClass('detecting');
      }
      return this;
    }

    this.onOn = function (handler) { this.handlerOn = handler; return this; };
    this.onOff = function (handler) { this.handlerOff = handler; return this; };

    this.setValue(0.0);
    this.setMIDICC(-1);

    // Let it respond to midi
    (function (_) { midi.onEvent(function (event) {
      if(event.type == midi.CONTROL_CHANGE && _.detecting) {
          _.setMIDICC(event.data1);
          _.detect(false);
      }

      if(event.type == midi.CONTROL_CHANGE && event.data1 == _.midiCC) {
        if(event.data2 == 127)
          _.setValue(!_.value);
      }
    }); })(this);

  }}

};
