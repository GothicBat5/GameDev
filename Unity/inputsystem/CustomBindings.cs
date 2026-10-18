
using UnityEngine;
using UnityEngine.InputSystem;

public class CustomBindings : MonoBehaviour
{
    public static CustomBindings Instance { get; private set; }

    [SerializeField] private InputActionAsset inputActions;

    private const string SaveKey = "CustomBindings";

    public InputActionAsset Actions => inputActions;

    private void Awake()
    {
        if (Instance != null && Instance != this)
        {
            Destroy(gameObject);
            return;
        }

        Instance = this;
        DontDestroyOnLoad(gameObject);

        LoadBindings();
    }

    public void SaveBindings()
    {
        string json = inputActions.SaveBindingOverridesAsJson();
        PlayerPrefs.SetString(SaveKey, json);
        PlayerPrefs.Save();
    }

    public void LoadBindings()
    {
        if (PlayerPrefs.HasKey(SaveKey))
        {
            string json = PlayerPrefs.GetString(SaveKey);
            inputActions.LoadBindingOverridesFromJson(json);
        }
    }

    public void ResetBindings()
    {
        inputActions.RemoveAllBindingOverrides();
        SaveBindings();
    }

    public InputAction GetAction(string actionName)
    {
        return inputActions.FindAction(actionName, true);
    }
}
