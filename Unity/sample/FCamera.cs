using UnityEngine;

public class FCamera : MonoBehaviour
{
    [Header("References")]
    public Camera playerCamera;
    public Camera cctvCamera;

    [Header("Settings")]
    public float maxViewTime = 10f;

    private bool viewing = false;
    private float timer = 0f;

    void Start()
    {
        cctvCamera.enabled = false;
    }

    void Update()
    {
        //interaction key
        if (Input.GetKeyDown(KeyCode.E))
        {
            if (!viewing) EnterMonitor();
            else ExitMonitor();
        }

        if (viewing)
        {
            timer += Time.deltaTime;

            if (timer >= maxViewTime)
            {
                Debug.Log("You spent too long watching the cameras!");
                ExitMonitor();

                // Optional event
                // EnemyManager.Instance.SpawnEnemy();
            }
        }

        if(Input.GetKeyDown(KeyCode.Q))
        {
            cameras[currentCamera].enabled = false;

            currentCamera--;
            
            if(currentCamera < 0) currentCamera = cameras.Length - 1;

              cameras[currentCamera].enabled = true;
        }

        if(Input.GetKeyDown(KeyCode.R))
        {
            cameras[currentCamera].enabled = false;

            currentCamera++;
            
            if(currentCamera >= cameras.Length) currentCamera = 0;

            cameras[currentCamera].enabled = true;
        }
    }

    void EnterMonitor()
    {
        viewing = true;
        timer = 0f;

        playerCamera.enabled = false;
        cctvCamera.enabled = true;

        Cursor.lockState = CursorLockMode.None;
        Cursor.visible = true;
    }

    void ExitMonitor()
    {
        viewing = false;

        playerCamera.enabled = true;
        cctvCamera.enabled = false;

        Cursor.lockState = CursorLockMode.Locked;
        Cursor.visible = false;
    }
}
