function onFrameAdvance(delta) {
	//print("Time advance: ", delta);
}

function parseArgs(args) {
	//Usage prompt
	if (args.length < 2) {
		print("Usage: ", args[0], " <files ...>\n");
		exit(1);
	}

	print(args.length, " Args:");

	for (var i = 0; i < args.length; i ++) {
		print("Arg: \"", args[i], "\"");
		//createInterior(args[i]);
	}
}

function onStart() {
	//Create a basic default camera
	createCamera(0, 0, 30);
	//Create us a player at (0, 0, 60) radius 0.2
	createPlayer(0, 0, 60, 0.2);
}