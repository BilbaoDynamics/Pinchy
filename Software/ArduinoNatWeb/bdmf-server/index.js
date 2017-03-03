// Imports
var express     = require('express');
var app         = express();
var server      = require('http').Server(app);
var fs          = require('fs')
var bodyParser  = require('body-parser');
var io          = require('socket.io')(server);

// Init & Vars
var lastCon     = null;
server.listen(9999);

app.use(bodyParser.json());
app.use(express.static(__dirname + '/public'));

// Code
app.get('/data/:data', function(req, res){
    console.log('got from client: ' + req.params.data);
    res.send('ok');
    lastCon.emit('message', req.params.data);
});

io.on('connection', function(socket) {

    lastCon = socket;

    socket.on('close', function() {
        console.log('stopping client');
    });

    console.log('started client'); 

}); 