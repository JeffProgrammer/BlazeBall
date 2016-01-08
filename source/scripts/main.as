void main() {
	echo("Hello World");
	
	//if (loadLevel("test.json"))
	//if (loadLevel("level.json"))
	//	echo("Level level.json loaded successfully.");
	//else
	//	echo("Level file level.json not found!");
	
	// create script object
	ScriptObject @obj = createObject("ScriptObject");
	obj.setName("Bob");
	string name = obj.getName();
	echo(name);
}

float get5() {
	return 5;
}

float add(float a, float b) {
	return a + b;
}