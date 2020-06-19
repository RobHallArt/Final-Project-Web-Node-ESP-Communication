const WebSocket = require('ws') // Import Websocket Library
const util = require('util')


require('dns').lookup(require('os').hostname(), function (err, add, fam) { // Use DNS Library to find the local IP of the local machine (to be input into the ESP8266)
    console.log('Use this address for connecting from the ESP8266: '+ add +':3000'); // Log this to the console
})


const ws = new WebSocket.Server({ port: 3000 }) // Start the Websocket server that the ESP8266 will connect to

ws.on('connection', ws => { // Function called when we have a connection from the ESP8266
    
    console.log("ServerStarted");  // Log to console
    const url = "wss://43c2341cc28c.eu.ngrok.io:443" // The hostname of the secure websocket we want to relay to the ESP8266. Herein are functions called when different network events happen.
    const connection = new WebSocket(url) // Initiate the connection to the secure websocket.

    connection.onopen = () => { // When the connection to the secure server is established.
        console.log("CONNECTED"); // Log to console
    }

    connection.onerror = error => { // If there is an error log it.
        console.log("WebSocket error:" + util.inspect(error))
    }

    connection.onmessage = e => {   // When we recieve a message from the secure websocket...
        console.log(e.data)     // Log the message to console.
        ws.send(e.data) // Send the message via the server we are inside to the ESP8266 (UNSECURE WEBSOCKET)
    }



  ws.on('message', message => {         // If we get a message from the ESP8266
    console.log('MESSAGE FROM ESP ' + message)   // Log it
  })
})
