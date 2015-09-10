"use strict";

var scene = getScene();

function onFrameAdvance(delta) {
	//print("Time advance: ", delta);
}

function parseArgs(args) {
	//Usage prompt
	if (args.length < 2) {
		print("Usage: ", args[0], " <files ...>\n");
		exit(1);
	}

	for (var i = 1; i < args.length; i ++) {
		var interior = new GameInterior(args[i]);
		interior.generate();
		scene.addObject(interior);
	}
}

function onStart() {
	//Create a basic default camera
	scene.camera = new Camera();
	scene.addObject(scene.camera);

	//Create us a player at (0, 0, 60) radius 0.2
	scene.player = new Sphere(new vec3(0, 0, 20), 0.2);
	scene.addObject(scene.player);

	scene.setControlObject(scene.player);
}

function onKeyDown(key) {
	var chr = String.fromCharCode(key);
	print(scene.getControlObject() == scene.player);

	if (chr == "c" || chr == "C") {
		scene.setControlObject(scene.getControlObject() == scene.player ? scene.camera : scene.player);
	}
	
	if (chr == "m" || chr == "M") {
		var object = new Shape("cube.dae");
		scene.addObject(object);
		
		var x = Math.random() * 10.0;
		var y = Math.random() * 10.0;
		var z = Math.random() * -10.0;
		object.setPosition(new vec3(x, y, z));
	}
}

function onKeyUp(key) {
	var chr = String.fromCharCode(key);
}

function fire() {
	var proj = new Sphere(new vec3(0, 0, 30), 0.75);
	scene.addObject(proj);
}

function printThings(list) {
	print("Count: ", list.length);

	for (var i = 0; i < list.length; i ++) {
		print(list[i]);
	}
}

require("./test.js");
