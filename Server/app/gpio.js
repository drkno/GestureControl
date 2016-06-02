var ports = [21, 19, 15, 13],           // the Raspberry Pi GPIO ports to toggle
	pi_gpio = require('rpi-gpio'),
	currTimeout = null;

var GPIO = {                            // imitator of the Python GPIO interface for Raspberry Pi
	_openPorts: [],                     // made to allow code to be more portable during experimentation
	HIGH: true,
	LOW: false,
	OUT: pi_gpio.DIR_OUT,
	setup: function(port, mode, callback) {
		pi_gpio.setup(port, mode, function(err) {
			if (err) {
				throw err;
			}
			this._openPorts.push(port);
			callback();
		}.bind(this));
	},
	output: function(port, output) {
		pi_gpio.write(port, output, function(err) {
			if (err) {
				throw err;
			}
		});
	},
	cleanup: function() {
		pi_gpio.destroy();
		this._openPorts = [];
	}
};

var GpioManager = module.exports = function() {
	for (var i = 0; i < ports.length; i++) {
		GPIO.setup(ports[i], GPIO.OUT, function(p) {
			GPIO.output(p, GPIO.HIGH);                  // initialise all ports to active low
		}.bind(this, ports[i]));
	}
};

GpioManager.prototype.up = function(timeout) {
    this.abort();

	for (var i = ports.length-1; i >= 0; i--) {
         GPIO.output(ports[i], GPIO.LOW);               // start desk movement up
    }
	
	currTimeout = setTimeout(function() {
		for (var i = 0; i < ports.length; i++) {
			GPIO.output(ports[i], GPIO.HIGH);           // reset
		}
		currTimeout = null;
	}, timeout*1000);
};

GpioManager.prototype.down = function(timeout) {
    this.abort();

	for (var i = ports.length/2-1; i >= 0; i--) {
		GPIO.output(ports[i], GPIO.LOW);                // start desk movement down
	}

	currTimeout = setTimeout(function() {
		for (var i = 0; i < ports.length; i++) {
            GPIO.output(ports[i], GPIO.HIGH);           // reset
        }
		currTimeout = null;
    }, timeout*1000);
};

GpioManager.prototype.abort = function() {
    if (currTimeout && currTimeout != null) {
        clearTimeout(currTimeout);
        currTimeout = null;

        for (var i = 0; i < ports.length; i++) {
            GPIO.output(ports[i], GPIO.HIGH);           // reset
        }
    }
};

GpioManager.prototype.cleanup = function() {
	GPIO.cleanup();
};
