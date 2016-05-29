var Serve = require('./serve.js'),
	GpioManager = require('./gpio.js'),
	manager = null,
	dns = require('dns');

var logRequest = function(ip, action) {
	dns.reverse(ip, function (err, domains) {
		if (err || domains.length === 0) {
			console.log(ip + ': ' + action);
		}
		else {
			console.log(domains[0] + ': ' + action);
		}
	});
};

exports.run = function(config) {
	var server = new Serve(config.port);
	manager = new GpioManager();

	server.apiGet('abort', function (req, res) {
		logRequest(req.ip, 'abort');
		manager.abort();
		res.send('{"complete":true}');
	});

	server.apiPost('up', function (req, res) {
		res.contentType("application/json");
		try {
			var json = req.body;
			if (!json.timeout || json.timeout > 20) {
				json.timeout = 20;
			}
			if (json.timeout < 0) {
				json.timeout = 0;
			}
			
			logRequest(req.ip, 'up ' + json.timeout);
			manager.up(json.timeout);
			res.send('{"complete":true}');

		}
		catch(e) {
			console.log(e.stack);
			res.status(400).send('{"complete":false}');
		}
	});

	server.apiPost('down', function (req, res) {
        res.contentType("application/json");
        try {
            var json = req.body;
            if (!json.timeout || json.timeout > 14) {
                json.timeout = 14;
            }
            if (json.timeout < 0) {
                json.timeout = 0;
            }
			
            logRequest(req.ip, 'down ' + json.timeout);
            manager.down(json.timeout);
            res.send('{"complete":true}');

        }
        catch(e) {
            console.log(e.stack);
            res.status(400).send('{"complete":false}');
        }
     });

	server.start(config.port);
};

process.on('exit', function() {
	manager.cleanup();
});

process.on('SIGINT', function() {
	manager.cleanup();
	process.exit();
});
