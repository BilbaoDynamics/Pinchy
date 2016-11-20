// Imports
var io = require('socket.io-client')
var http = require('http');
var get = require('simple-get');

// Config
var server = "http://eldoctordeldesierto.com:9999/";

// Init
var socket = io.connect(server, {reconnect: true});

// Code
socket.on('open', function(){
    console.log('connected to server');
});

socket.on('message', function (data) {
    
    data = JSON.parse(data);

    let url = "http://192.168.10." + (Number(data.id)+Number(100)) + "/" + generateResponse(data);
    get(url, function (err, res) {
        console.log(res);
        if (err) ;
    });

});


// Functions
function generateResponse(data){
    console.log(data);
    if(data.type == "led"){
        response = `LED?color=${data.data}`;
    }
    else if(data.type == "buzzer"){
        if      (data.data == "on")  response = "BUZZER?sound=ON";
        else if (data.data == "off") response = "BUZZER?sound=OFF";
    }
    else if(data.type == "matriz"){
        response = "MATRIX"+data.data;
    }
    else if(data.type == "lcd"){
        response = "LCD?Number="+data.data;
    }

    return response;

}