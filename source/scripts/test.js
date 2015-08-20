function testit() {
	var t = new Test(2);

	print(t.getThing());
	t.setThing(4);
	print(t.getThing());
}

testit();

var vec = new vec3(1, 2, 3);
print(vec.getX());
