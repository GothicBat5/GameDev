using Unity.Netcode;
using UnityEngine;

public class NetworkLauncher : MonoBehaviour
{
    void OnGUI()
    {
        if (GUILayout.Button("Host"))
        {
            NetworkManager.Singleton.StartHost();
        }

        if (GUILayout.Button("Client"))
        {
            NetworkManager.Singleton.StartClient();
        }
    }
}
