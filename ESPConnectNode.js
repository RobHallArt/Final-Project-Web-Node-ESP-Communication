var fs = require('fs');

// read ssl certificate
var privateKey = fs.readFileSync('pk.pem', 'utf8');
var certificate = fs.readFileSync('cert.pem', 'utf8');
var caS = fs.readFileSync('CA.pem', 'utf8');

var credentials = { key: privateKey, cert: certificate, ca:caS };


/**************************websocket_example.js*************************************************/
var bodyParser = require("body-parser");
const express = require('express'); //express framework to have a higher level of methods
const app = express(); //assign app variable the express class/method
const appb = express(); //assign app variable the express class/method
var http = require('http');
var https = require('https');
var path = require("path");
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());
const server = http.createServer(app);//create a server
var httpsServer = https.createServer(credentials);
httpsServer.listen(8443);
//***************this snippet gets the local ip of the node.js server. copy this ip to the client side code and add ':3000' *****
//****************exmpl. 192.168.56.1---> var sock =new WebSocket("ws://192.168.56.1:3000");*************************************
require('dns').lookup(require('os').hostname(), function (err, add, fam) {
  console.log('addr: '+add);
})
/**********************websocket setup**************************************************************************************/
//var expressWs = require('express-ws')(app,server);

const WebSocket = require('ws');

const s = new WebSocket.Server({ server:httpsServer });

//when browser sends get request, send html file to browser
// viewed at http://localhost:30000
app.get('/', function(req, res) {
res.sendFile(path.join(__dirname + '/index.html'));
});
//*************************************************************************************************************************
//***************************ws chat server********************************************************************************
//app.ws('/echo', function(ws, req) {



s.on('connection',function(ws,req){
/******* when server receives messsage from client trigger function with argument message *****/
  ws.on('message',function(message){
    console.log("Received: "+message);
    s.clients.forEach(function(client){ //broadcast incoming message to all clients (s.clients)
      if(client!=ws && client.readyState ){ //except to the same client (ws) that sent this message
        client.send("broadcast: " +message);
      }
    });
// ws.send("From Server only to sender: "+ message); //send to client where message is from
  });
});

server.listen(50000);