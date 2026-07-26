function OnPostSpawn()
{
    printl("Intro sequence script loaded\n");
    EntFireByHandle(self, "RunScriptCode", "StartIntro()", 2.0, null, null);
}

function StartIntro()
{
    printl("Starting intro...\n");
    local door = Entities.FindByName(null, "exit_door");
    local speaker = Entities.FindByName(null, "announcer_speaker");
    
    if (door)
    {
        EntFireByHandle(door, "Lock", "", 0, null, null);
        printl("Door locked\n");
    }
    
    // Play a sound
    if (speaker)
    {
        EntFireByHandle(speaker, "PlaySound", "", 0, null, null);
    }
    
    // Open the door after 5 more seconds
    EntFireByHandle(self, "RunScriptCode", "OpenDoor()", 5.0, null, null);
}

function OpenDoor()
{
    local door = Entities.FindByName(null, "exit_door");
    if (door)
    {
        EntFireByHandle(door, "Unlock", "", 0, null, null);
        EntFireBy
    }
}
