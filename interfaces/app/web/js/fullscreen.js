var fullscreen = {

  state: false,

  toggle: function (element) {
    if(!fullscreen.state)
      fullscreen.enter(element);
    else
      fullscreen.cancel();

      fullscreen.state = !fullscreen.state;
  },

  enter: function (element) {
    if(element.requestFullscreen) {
      element.requestFullscreen();
    } else if(element.msRequestFullscreen) {
      element.msRequestFullscreen();
    } else if(element.mozRequestFullScreen) {
      element.mozRequestFullScreen();
    } else if(element.webkitRequestFullscreen) {
      element.webkitRequestFullscreen();
    }
  },

  cancel: function () {
    if(document.exitFullscreen) {
        document.exitFullscreen();
    } else if(document.webkitExitFullscreen) {
        document.webkitExitFullscreen();
    } else if(document.mozCancelFullScreen) {
        document.mozCancelFullScreen();
    } else if(document.msExitFullscreen) {
        document.msExitFullscreen();
    }
  }

};
