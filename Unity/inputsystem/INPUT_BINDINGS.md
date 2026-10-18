> Key Bindings in Unity (Intro)
### How would you implement a custom input in Unity?

```csharp
    // MOVEMENT 
    private void Update()
    {
        Vector2 movement = moveAction.ReadValue<Vector2>();

        Vector3 direction = new Vector3(movement.x, 0f, movement.y);
    }

    private void Start()
    {
        moveAction = CustomBindings.Instance.GetAction("Move");
        moveAction.Enable();
    }
```

### Custom inputs are based: 

Keybindings calls the method -> update the appropriate rebinding: 

```csharp
    CustomBindings.Instance.SaveBindings();
```

Then for the UI 

```csharp
  action.GetBindingDisplayString(bindingIndex);
```
