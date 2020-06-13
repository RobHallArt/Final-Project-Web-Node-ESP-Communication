const WebSocket = require('ws')
const util = require('util')

const url = 'wss://robhallnode.a2hosted.com:8443'
const connection = new WebSocket(url)

connection.onopen = () => {
    console.log("CONNECTED");
}

connection.onerror = error => {
    console.log("WebSocket error:" + util.inspect(error))
}

connection.onmessage = e => {
    console.log(e.data)
}