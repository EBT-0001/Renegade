--[[
	Renegade  Copyright (C) 2026  Temperlius
	This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
	This is free software, and you are welcome to redistribute it
	under certain conditions; type `show c' for details.
]]--

mouseX = getValue(0, "mouseX");
mouseY = getValue(0, "mouseY");
leftClick = getValue(0, "leftClick");

x = getValue(parent, "x");
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

		loadAnimation(Idle2, 1, "../assets/spritesheets/load.png", "../data/animations/load.json");
		setValue(parent, "animationPlaying", 1);
	end
elseif (clicked) then
	setValue(parent, "animationPlaying", 0);
	loadSave("../data/saves/save1.json");
end
