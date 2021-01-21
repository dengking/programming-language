window.onload = function() {

  var animation_direction  = null;

  function animateHeight(obj, height, delay) {
    var obj_height = obj.clientHeight;
    if(delay == undefined) { delay = 5; }

    if(obj_height == height) { return; }

    if (delay == 0) {
      obj.style.height = height + "px";
      return;
    }

    if(obj_height <= height) {
      if(animation_direction == "up") { animation_direction = null; return; }
      else if(!animation_direction)   { animation_direction = "down"; }
      obj.style.height = (obj_height + obj_height/50) + "px";
      setTimeout(function(){
        animateHeight(obj, height);
      }, 5)
    }
    else {
      if(animation_direction == "down") { animation_direction = null; return; }
      else if(!animation_direction)     { animation_direction = "up"; }
      obj.style.height = (obj_height - obj_height/50) + "px";
      setTimeout(function(){
        animateHeight(obj, height);
      }, 5)
    }
  }

  // Use both class and ids to identify widgets
  var w       = document.getElementById('gear-widget');
  var widgets = document.getElementsByClassName('gear-widget');
  var widgets_collection = [];
  for (var i = 0, len = widgets.length; i < len; i++) {
    widgets_collection.push(widgets[i]);
  }
  if (w) widgets_collection.push(w);
  widgets = widgets_collection;

  for (var i = 0, len = widgets.length; i < len; i++) {
    var widget = widgets[i];
    var target = widget.getAttribute('src');
    widget.contentWindow.postMessage({ eventName: 'source-widget-height-change' }, target);
  }

  window.addEventListener('message', function(event) {
    if(event.data.eventName == 'gear-widget-height-change') {
      for (var i = 0, len = widgets.length; i < len; i++) {
        var widget = widgets[i];
        animateHeight(widget, event.data.newHeight, event.data.animationDelay);
      }
    }
    if(event.data.eventName == 'redirect') {
      window.location = event.data.location;
    }
  });
};
