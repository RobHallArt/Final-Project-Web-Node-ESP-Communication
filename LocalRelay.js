const WebSocket = require('ws')
const util = require('util')


require('dns').lookup(require('os').hostname(), function (err, add, fam) {
    console.log('addr: '+add);
})


const wss = new WebSocket.Server({ port: 3000 })

wss.on('connection', ws => {

    const url = 'wss://robhallnode.a2hosted.com:8443'
    const connection = new WebSocket(url)

    //const wss = new WebSocket.Server({ port: 3000 })

    


    connection.onopen = () => {
        console.log("CONNECTED");
    }

    connection.onerror = error => {
        console.log("WebSocket error:" + util.inspect(error))
    }

    connection.onmessage = e => {
        console.log(e.data)
        ws.send(e.data)
    }



  ws.on('message', message => {
    console.log(`Received message => ${message}`)
  })
  //ws.send('ho!')
})
