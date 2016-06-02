# Standing Desk Gesture Control

|Item      |Value       |
|---------:|------------|
|Date      | 2016-06-03 |
|Author    |Matthew Knox|
|Student ID|  81118603  |

This program uses a Logitech C170 Webcamera located on a standing desk to control the height of the desk. 

In theory it is possible to run this entire codebase on a Raspberry Pi - I've only tested the server in this scenario.
This project consists of two parts. A client which is responsable for all computer vision related aspects, and a server which receives commands and controls a desk.

## Server
Controls the desk itself and provides a REST API for doing so. A basic html UI for controlling the desk using REST is also provided. The server is located in the `/Server` directory.

#### Hardware
The server requires a hardware relay switch to be attached to the GPIO port of the Raspberry Pi. During this project, a SainSmart 8 Channel DC 5V Relay Module was used for this purpose.
This must be wired appropriately into the desk.

#### Installation and Configuration
At minimum, `node.js` version 4 must be installed along with the version of `npm` that ships with that release.

To install:
```
cd Server
npm install
```

To configure:

- Open `app/gpio.js` and change `ports = [21, 19, 15, 13]` to the GPIO ports you will be using.
- In the same file, alter `GPIO` to work as appropriate for your port wiring

The node.js webserver can then be started using `node desk.js`, or alternatively to start as a background process run `nohup sudo node desk.js & disown`.

#### Code Overview

- `package.json` - file describing 3rd party node.js dependencies
- `desk.js` - main entry point for the program. Loads optional configuration file can calls `app/main.js`.
- `www/` - directory containing HTML files
    - `index.html` - basic HTML UI for controlling the desk. Can be accessed at `http://yourserver:yourport/`.
- `app/` - directory containing server files
    - `main.js` - Sets up the REST end points and does basic handling of incoming requests.
    - `serve.js` - The actual webserver behind the REST and HTML endpoints.
    - `gpio.js` - The code that controls the desk. Wrapper around the GPIO ports.

## Client
Provides gesture controls to interface with the server using REST. The client is located in the `/GestureControl/Client` directory (although VS solution is in higher directories).

#### Hardware
This project is only tested with a Logitech C170 Webcamera. In any other conditions the outcome is untested.

#### Configuration
To configure:
- Open `DeskControl.cpp` and alter the class constructor to point at your server.
- Open `main.cpp` and change the video input to your camera.

#### Building
Building is only tested in `Microsoft Visual Studio 2015` on the x64 architecture. `OpenCV 3.1` or higher is required.

To build:
- Open `.sln` in Visual Studio.
- Restore packages in `packages.config`. This is for the REST SDK that is used to control the desk (`cpprestsdk`).
- Update linker to use your local version of OpenCV. Alternatively, setup the `%OPENCV_DIR%` environment variable.
- Ensure your language level target is `C++14` or later.
- Build, it should automatically run.

#### Code Overview

- `main.cpp` - main entry point and core logic of program
- `BackgoundSubtractor.*` - handles background subtraction related activities
- `CannyEdgeTuner.*` - handles canny edge detection
- `ConvexHull.*` - finds convex hulls around detected edged
- `cp.h` - provides cross platform stdio functions
- `DeskControl.*` - controls the standing desk using rest queries
- `FingerCounter.*` - uses convexity defects to count fingers and determine pose of a hand
- `Windows.h` - currently unused. Hook into all create window methods so that a window manager can be implemented.

#### Notes

- Without a server to connect to the program will crash
- Thresholds will need to be tuned to your camera.
- If not using a live camera feed, disable background subtraction in the code base.

## Sample/Demo Data
Sample and demo data is located in the `/Sample` directory.
