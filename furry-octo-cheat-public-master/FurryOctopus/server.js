var readline = require('readline'),
	cInput = readline.createInterface(process.stdin, process.stdout),
  prefix = '> ';
  
var app = require('net')
, fs = require('fs')

function forceUnicodeEncoding(string) {
  return unescape(encodeURIComponent(string));
}
// 20 0000001 0x04A19DA4 0x00A77D3C 0x00002410 0x00000134 0x000000F0 0x000000FC 0x000000E9 0x00000064
var Offsets = 
[
	"0x4A19DA4", // EntityList : array of ???
	"0xA77D3C", // LocalPlayer : Player
	"0x00002410", // CrosshairIndex : int
	"0x00000134", // Entity Position : vec3
	"0x000000F0", // Player Team : int
	"0x000000FC", // Player Health : int
	"0x000000E9", // Is Dormant : ?
	"0x00000064", // Entity Index : int
];

var HackUser = (function()
{
	this.Username = "";
	this.Password = "";
	this.IPAddress = "";
	
	this.UID = "";
	
	this.CompareString = "";
	
	function HackUser(usr,psw,ip,id)
	{
		this.Username = usr;
		this.Password = psw;
		this.IPAddress = ip;
		this.UID = id;
		
		this.CompareString = this.IPAddress + ":" + this.Username + ":" + this.Password + "\r\n";
	};
	
	return HackUser;
})();

HackUser.prototype.SuccessfulLogin = function(socket)
{
	console.log(curDateTime() + " - - " + this.IPAddress + " - - Successful login" + " - " + this.Username + "(" + this.UID + ")");
	var reply = this.UID;
	Offsets.forEach(function(v,i,a)
	{
		reply += ":" + v;
	});
	socket.write(reply);
	socket.end();
};

var clients = 
[
	new HackUser("usertest", "usertest", "0.0.0.0","0000000"),
	new HackUser("usertest","usertest","0.0.0.0","0000001"),
	new HackUser("usertest","usertest","0.0.0.0","0000002"),
	new HackUser("usertest","usertest", "0.0.0.0","8234921"),
];

function curDateTime()
{
	var date = new Date(); 
	var currentdate = new Date(date.valueOf() + (date.getTimezoneOffset()+60) * 60000);
	var datetime = "[" + currentdate.getDate() + "/"
					+ (currentdate.getMonth()+1)  + "/" 
					+ currentdate.getFullYear() + " @ "  
					+ currentdate.getHours() + ":"  
					+ currentdate.getMinutes() + ":" 
					+ currentdate.getSeconds() + "]";
					
	return datetime;
}

var server = app.createServer(function (socket) {
  var ip = socket.remoteAddress;
  
  console.log(curDateTime() + " - - " + ip + ' - - CONNECTION ESTABLISHED');
  socket.pipe(socket);
  
  socket.on('data', function(data) 
  {
    var userpass = data.toString('utf8');
	var cl = ip + ":" + userpass;
	cl = cl.toString('utf8');
	var success = false;
	clients.forEach(function(user)
	{
		if(user.CompareString == cl)
		{
			user.SuccessfulLogin(socket);
			success = true;
		}
	});
	if(!success)
	{
		console.log(curDateTime() + " - - " + ip + " - - Unsuccessful login" + " - " + userpass);		
	}
  });
  
  socket.on('end', function () 
  {
	console.log(curDateTime() + " - - " + ip + " - - DISCONNECT");
  });
  socket.on('error',function(error){
	console.log(curDateTime() + " - - " + ip + " - - " + error.code);
  });
});

var io = require('socket.io').listen(server)

server.listen(3001);
console.log(curDateTime() + " - - " + "SERVER STARTED");