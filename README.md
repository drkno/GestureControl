# Standing Desk Gesture Control

In theory it is possible to run this entire codebase on a Raspberry Pi - I've only tested the server in this scenario.

### Server
Controls the desk itself and provides a basic html UI for controlling the desk using REST.

To configure:
- Open `app/gpio.js` and change `ports = [21, 19, 15, 13]` to the GPIO ports you will be using.
- In the same file, alter `GPIO` to work as appropriate for your port wiring

Using node.js start `desk.js` using `nohup sudo node desk.js & disown`. 

### Client
Provides gesture controls to interface with the server using REST.

To configure:
- Open `DeskControl.cpp` and alter the class constructor to point at your server.
- Open `main.cpp` and change the video input to your camera.


Note:
- Without a server to connect to the program will crash
- Thresholds will need to be tuned to your camera.