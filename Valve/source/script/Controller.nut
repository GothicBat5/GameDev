
class Controller 
{
    constructor() 
    {
        print("AI Controller initialized for: " + self.GetName());
        self.SetHealth(100);
    }

    function TakeDamage(amount) 
    {
        local newHealth = self.GetHealth() - amount;
        self.SetHealth(newHealth);
        
        if (newHealth <= 0) 
        {
            self.Kill();
            print("AI Controller died.");
        }
    }

    function OnSpawn() 
    {
        // Called when the entity spawns
        self.SetModel("models/player/bot.mdl");
    }
}

// Instantiate the class for this entity
LocalAI <- MyAIController();
