using UnityEngine;

public class RandomSpawner : MonoBehaviour
{
    public GameObject enemyPrefab;
    public int enemyCount = 20;

    void Start()
    {
        for (int i = 0; i < enemyCount; i++)
        {
            float x = Random.Range(-10f, 10f);
            float z = Random.Range(-10f, 10f);

            Vector3 spawnPos = new Vector3(x, 0f, z);

            Instantiate(enemyPrefab, spawnPos, Quaternion.identity);
        }
    }
}
