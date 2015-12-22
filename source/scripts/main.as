void main() {
	echo("Hello World");
	
	if (loadLevel("test.json"))
		echo("Level test.json loaded successfully.");
	else
		echo("Level file test.json not found!");
}

float get5() {
	return 5;
}

float add(float a, float b) {
	return a + b;
}