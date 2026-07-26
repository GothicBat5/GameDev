
fe.layout.width = 640;
fe.layout.height = 480;

local bg = fe.add_image("bg.png", 0, 0, fe.layout.width, fe.layout.height);
local title = fe.add_text("[Title]", 0, 10, fe.layout.width, 35);
title.set_rgb(0, 255, 0); 
local snap = fe.add_artwork("snap", 338, 70, 287, 214);
local list = fe.add_listbox(9, 75, 311, 385);
list.rows = 20;
list.charsize = 16;

class UI </ help="Layout Options" /> {
    </ label="Color Theme", help="Choose your theme color", options="Green,Red,Blue" order=1 />
    color = "Green";
}

local config = fe.get_config();

if (config["color"] == "Red") 
{
    bg.set_rgb(170, 10, 50);
    title.set_rgb(255, 0, 0);
    list.set_selbg_rgb(180, 30, 30);
} 
else if (config["color"] == "Blue") 
{
    bg.set_rgb(50, 10, 170);
    title.set_rgb(0, 0, 255);
    list.set_selbg_rgb(30, 30, 180);
} 
else {
    // Default Green
    bg.set_rgb(10, 170, 50);
    title.set_rgb(0, 255, 0);
    list.set_selbg_rgb(30, 180, 30);
}
