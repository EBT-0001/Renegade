--[[
	Renegade  Copyright (C) 2026  Temperlius
	This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
	This is free software, and you are welcome to redistribute it
	under certain conditions; type `show c' for details.
]]--

x = getValue(parent, "x");
y = getValue(parent, "y");
width = getValue(parent, "width");
height = getValue(parent, "height");
hp = getValue(parent, "hp");

if (y > 1080 or y < 0) then
	setValue(parent, "hp", 0);
end
if (hp == 0) then
	loadSave("../data/saves/save1.json");
end
