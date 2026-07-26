using UnityEngine;

public class EnemySpawner : MonoBehaviour
{
    public GameObject enemyPrefab;

    void Start()
    {
    //For loop script in Unity ! 
        for (int i = 0; i < 10; i++)
        {
            Vector3 spawnPosition = new Vector3(i * 2f, 0f, 0f);

            Instantiate(enemyPrefab, spawnPosition, Quaternion.identity);
        }
    }
}
