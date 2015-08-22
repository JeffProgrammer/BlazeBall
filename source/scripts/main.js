"use strict";

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
		createInterior(args[i]);
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

function printThings(list) {
	print("Count: ", list.length);

	for (var i = 0; i < list.length; i ++) {
		print(list[i]);
	}
}

require("./test.js");
