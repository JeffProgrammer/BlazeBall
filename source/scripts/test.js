var scene = getScene();

for (var x = 0; x < 10; x ++) {
	for (var y = 0; y < 10; y ++) {
		var test = new Sphere(new vec3(x, y, 20), ((x + y) % 7) / 9);
		scene.addObject(test);
	}
}

