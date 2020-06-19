Robert Hall Final Project DAC 2020 Year 3 Goldsmiths

Some of this project is a bit of a bodge but it works, it just requires a bit of set up if you want to use it yourself.
It was much simpler but then each hosting service had it's own difficulties. The complexities of all of this are because I don't pay for NGROK basically.

There are 3 batch files.

launchNGROK -   This starts NGROK with a config file that is also included to make two tunnesls with different ports, on 80 and 443
                This will give you two hostnames, the hostname with port 443 should be replace the websocket URL in localIndex.html and localRelay.js
                The other is the url that will be used to access the artwork. This must be accesed with https:// at the start.

ServerGoFP -    This launches the main server which everything connects to.

LaunchReplay -  This launches the server that the ESP8266 will connect to that relays messages from the main secure server.
                This Server returns an IP that should be pasted in the websocket server url for the ESP8266 using as per the instruction.

ESPConnectESP Arduino Sketch -  This should be uploaded to the ESP8266. It turns each pin on and off breifly in response to specific websocket messages.
                                The local relay won't connect to the main server until it connects to the ESP... This is a strange quirk that I imagine would require a big rewrite to fix.

ControllerBlink Arduino Sketch - This is used for testing the solenoid control boards I'm using. It just switches the relevant pins on and off in a pattern.
