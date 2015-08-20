function testit() {
	var t = new Test(2);

	print(t.getThing());
	t.setThing(4);
	print(t.getThing());
}

testit();