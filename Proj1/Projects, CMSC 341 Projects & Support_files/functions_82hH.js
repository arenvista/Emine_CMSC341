// this toggles the headers to resemble accordion
// at the load everything is collapsed
// user can click to close a header or collapse it
// show() collapse all at load
// if we use hide(), everything will be closed at load

jQuery(document).ready(function(){
    $('#content h2').click(function() {
        $(this).next().toggle('slow');
        return false;
    }).next().show();
  });

