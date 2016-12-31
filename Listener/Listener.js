var dgram = require('dgram');
//var couchbase = require('couchbase');

var PORT = 33333;
var HOST = '192.168.1.103';
var count = 1;
var key = '';

//var cluster = new couchbase.Cluster('127.0.0.1');
//var bucket = cluster.openBucket('acquire-demo');
    
var server = dgram.createSocket('udp4');

server.on('listening', function() {
    var address = server.address();
    console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

server.on('message', function(message, remote) {
    key = 'id_' + count++;
    console.log(message + '');

    //bucket.upsert(key, message + '', function (err, result) {console.log(message + '');});
});

server.bind(PORT, HOST);