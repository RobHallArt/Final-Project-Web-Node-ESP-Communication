var fs = require('fs'); // Import filesystem library

// Read ssl certificate
var privateKey = fs.readFileSync('pk.pem', 'utf8');
var certificate = fs.readFileSync('cert.pem', 'utf8');
var caS = fs.readFileSync('CA.pem', 'utf8');

var credentials = { key: privateKey, cert: certificate, ca:caS }; // put the certificates into a credentials object



var bodyParser = require("body-parser");  // Import Body-parser library
const express = require('express'); // Import Express server library
const app = express(); // Assign app variable the express class/method

// Import methods to serve http, https and read paths.
var http = require('http'); 
var https = require('https');
var path = require("path");

// Set up bodyParser
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

const server = http.createServer(app); // Create a server which will serve the website
var httpsServer = https.createServer(credentials);  // Create a server which will handle the secure websockets over https

// Set Ports for the servers to bind to (listen on)
server.listen(80);
httpsServer.listen(443);

console.log('Server Setup Succeeded');

const WebSocket = require('ws'); // Import websockets Library

const s = new WebSocket.Server({ server:httpsServer }); // Bind a new websocket server to the https server.

app.get('/', function(req, res) {   // When there is a get request for the webpage
  res.sendFile(path.join(__dirname + '/indexLocal.html'));  // Send the file 'indexLocal.html'
});


s.on('connection',function(ws,req){ // When we recieve a new connection from the websocket client (in this case the served webpage or the Local Relay node.js program)

  ws.on('message',function(message){ // When we get a message from a client (the served webpage)

    console.log("Received: "+message); // Log the message to the console

    s.clients.forEach(function(client){ // Broadcast incoming message to all clients (s.clients)

      if(client!=ws && client.readyState ){ // Check if the client we are sending to is not the originator of the message, don't send it there to avoid a loop.

        client.send("broadcast: " + message); // Send the message

      }
    });
  });
});

