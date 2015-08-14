//Create us a player at (0, 0, 60) radius 0.2
createPlayer(0, 0, 60, 0.2);

function onFrameAdvance(delta) {
	print("Time advance: ", delta);
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

}
