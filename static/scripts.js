//Format mask for CCN input form
    //Ref: http://www.igorescobar.com/blog/2012/05/06/masks-with-jquery-mask-plugin/
//Three second timer for CCN validity flash messages
    //Ref: https://stackoverflow.com/questions/31176402/how-to-hide-flash-message-after-few-seconds
$(document).ready(function(){
    $('.form-control').mask('0000-0000-0000-0000-000');
       setTimeout(function() {
            $('#luhnMessage').fadeOut('fast');
            }, 3000);
});

//Restrict user CCN input to numeric characters
//Ref: https://stackoverflow.com/questions/26948459/javascript-return-error-message-on-invalid-character-input
function validateLetter() {
    var textInput = document.getElementById("ccn").value;
    var replacedInput = textInput.replace(/[^0-9]/g, "");
    if(textInput != replacedInput)
        alert("Error message");

    document.getElementById("ccn").value = replacedInput;
  }