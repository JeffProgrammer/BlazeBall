function test() {
	var p = new Point(2, 2);
	print(p.getX());
	
	var x = new Point3(3, 4, 5);
	print(x.getZ());
	print(x.getX());
	
	x.y = 100;
	print(x.y);
	
	var added = add(2, 3);
	print(added);
	
	echo("hello world");
}