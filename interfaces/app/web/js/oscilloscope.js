var oscilloscope = {

  create: function (id) { return new function () {
    // Variables
    this.element = $(document.getElementById(id));
    this.width = this.element.width();
    this.height = this.element.height();

    // Structure
    this.element.addClass('oscilloscope');

    var canvas = $('<canvas>');
    canvas[0].width = this.width;
    canvas[0].height = this.height;
    this.element.append(canvas);
    this.ctx = canvas[0].getContext('2d');

    // Methods
    this.setData = function (data) {
      if(data === undefined) return false;
      if(data.length <= 1) return false;
      var dx = this.width / (data.length - 1);

      this.ctx.clearRect(0, 0, this.width, this.height);
      this.ctx.beginPath();
      this.ctx.moveTo(0, (this.height / 2) * (1 - data[0]));
      for(var i = 1;i < data.length;i ++) {
          this.ctx.lineTo(i * dx, (this.height / 2) * (1 - data[i]));
      }
      this.ctx.strokeStyle = 'black';
      this.ctx.lineWidth = 3;
      this.ctx.stroke();

      return true;
    };

  }}

};
