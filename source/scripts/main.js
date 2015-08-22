"use strict";

var scene = getScene();
scene.interiors = [];

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
		scene.interiors.push(interior);
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
}

function onKeyUp(key) {
	var chr = String.fromCharCode(key);
}

function printThings(list) {
	print("Count: ", list.length);

	for (var i = 0; i < list.length; i ++) {
		print(list[i]);
	}
}

require("./test.js");
