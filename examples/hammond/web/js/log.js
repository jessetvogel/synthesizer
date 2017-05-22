var log = {
	color: {
		white: '#F9F9F9',
		red: '#E94949',
		orange: '#E99949',
		yellow: '#E9E949',
		green: '#49E949',
		aqua: '#49E9E9',
		blue: '#4949E9',
		violet: '#E949E9'
	},

	print : function (message, c = this.color.white) {
		$('#log div').append('<span style=\'color: ' + c + ';\'>' + message + '</span><br/>');
		$('#log div').scrollTop($('#log div')[0].scrollHeight);
	},

  initialize: function () {
    $('#log').on('mouseenter', function() {
  		$(this).stop(true, false).animate({opacity: 1.0}, 200);
  		$(this).find('div').stop(true, false).animate({height: "128px"}, 200);
  	}).on('mouseleave', function() {
  		$(this).stop(true, false).animate({opacity: 0.5}, 200);
  		$(this).find('div').stop(true, false).animate({height: "32px"}, 200);
  	});
  }
};
