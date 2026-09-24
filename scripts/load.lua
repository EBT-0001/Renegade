--[[
	Renegade  Copyright (C) 2026  Temperlius
	This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
	This is free software, and you are welcome to redistribute it
	under certain conditions; type `show c' for details.
]]--

mouseX = getValue(nil, "mouseX");
mouseY = getValue(nil, "mouseY");
leftClick = getValue(nil, "leftClick");

X = getValue(parent, "x");
y = getValue(parent, "y");
width = getValue(parent, "width");
height = getValue(parent, "height");

clicked = getFlag(parent, 0);

if (leftClick) then
	if (
		mouseX > x and mouseX < x + width and
		mouseY > y and mouseY < y + height
	) then
		setFlag(parent, 0, true);
		loadAnimation(Idle, 0, "../assets/spritesheets/loadB.png", "../data/animations/load.json");
	end
elseif (clicked) then
	loadAnimation(Idle, 0, "../assets/spritesheets/loadA.png", "../data/animations/load.json");
	loadSave("../data/saves/save1.json");
end
