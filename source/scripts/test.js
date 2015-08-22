"use strict";

class Thing extends Sphere {
	test(a) {
		print(a)
	}
}

var v = new Thing(new vec3(0, 0, 0), 0.2)
v.test("yes")