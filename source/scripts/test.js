var scene = getScene();

for (var i = 0; i < 100; i ++) {
	var test = new Sphere();
	test.setPosition(new vec3(i, 0, 20));
	scene.addObject(test);
}

