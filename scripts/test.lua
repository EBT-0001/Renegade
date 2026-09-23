mouseX = getValue(nil, "mouseX");
mouseY = getValue(nil, "mouseY");
leftClick = getValue(nil, "leftClick");

X = getValue(parent, "x");
y = getValue(parent, "y");
width = getValue(parent, "width");
height = getValue(parent, "height");

if (leftClick) then
	if (
		mouseX > x and mouseX < x + width and
		mouseY > y and mouseY < y + height
	) then
		loadSave("../data/saves/save1.json");
	end
end
