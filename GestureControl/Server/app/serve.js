var express = require('express'),
	fs = require('fs'),
	bodyParser = require('body-parser'),
    path = require('path'),
    mime = require('mime'),
    url = require('url');

var Serve = module.exports = function(port) {
  this.htmlRoot = path.join(__dirname, './../www/');
  this.htmlRoot = path.resolve(this.htmlRoot);
  this.serverPort = port ? port : 8080;
  this.server = express();

  this.server.use(bodyParser.json());

  var x = this;
  this.server.use(function(req, res, next) {
      res.header('Cache-Control', 'private, no-cache, no-store, must-revalidate');
      res.header('Expires', '-1');
      res.header('Pragma', 'no-cache');
      res.header("X-Powered-By", "Knoxius Servius");
      res.header("Access-Control-Allow-Origin", "*");
      res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");

      var p = url.parse(req.url).path;

      if (p.indexOf("/api") < 0) {
          if (p === "/") {
              p = "/index.html";
          }

          var file = path.join(x.htmlRoot, p);
          file = path.resolve(file);

          if (!file.startsWith(x.htmlRoot)) {
            res.contentType("application/json");
            res.status(400).send('{"complete":false, "message":"bad request"}');
            return;
          }

          var type = mime.lookup(file);
          fs.readFile(file, function (err, data) {
              if (err) {
                  fs.readFile(path.join(x.htmlRoot, '404.html'), function(err, data) {
                      if (err) {
                          res.contentType("application/json");
                          res.status(404).send('{"complete":false, "message":"file not found"}');
                      } else {
                          res.status(404);
                          res.contentType("text/html");
                          res.send(data);
                      }
                  });

                  return;
              }
              res.contentType(type);
              res.send(data);
          });
      } else {
          next();
      }
  });
};

Serve.prototype.apiGet = function (api, func) {
  this.server.get('/api/' + api, func);
};

Serve.prototype.apiPost = function (api, func) {
  this.server.post('/api/' + api, func);
};

Serve.prototype.start = function () {
  this.server.listen(this.serverPort);
};
