using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.SocialPlatforms.Impl;


public class GameManager : MonoBehaviour
{
    public static GameManager Instance;
    public int health;
    public int score;
    public int highScore; //PlayerPref my reference

    public int killedEnemyCount;

    public GameState currentGameState;
    public UIManager uimanager;
    public EnemySpawner enemySpawner;

    private void Awake()
    {
        Instance = this;
        highScore = PlayerPrefs.GetInt("HighScore", 0);
    }

    private void Update()
    {
        if (health <= 0)
        {
            uimanager.ShowGameOverUI();
        }

        if (killedEnemyCount >= enemySpawner.maxEnemyToSpawn && health > 0)
        {
            uimanager.ShowGameOverUI(false);
        }
    }

    void Start()
    {
        highScore = PlayerPrefs.GetInt("HighScore", 0); 
        SetGameState(GameState.Playing);
        
        uimanager.SetScoreLabel(); //PlyPf
        uimanager.SetHighScoreLabel();
        uimanager.SetHealthLabel();
    }

    // First method added
    public void AddScore(int points)
    {
        score += points;

        uimanager.SetScoreLabel();

        if (score > highScore)
        {
            highScore = score;

            PlayerPrefs.SetInt("HighScore", highScore);
            PlayerPrefs.Save();

            uimanager.SetHighScoreLabel();
        }
    }

    //Second Method
    public void CheckHighScore()
    {
        if (score > highScore)
        {
            highScore = score;
            PlayerPrefs.SetInt("HighScore", highScore);
            PlayerPrefs.Save();
            uimanager.SetHighScoreLabel();
        }
    }

    public void RestartGame()
    {
        
        SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex);
    }

    public void SetGameState(GameState newState)
    {
        currentGameState = newState;
    }
}

public enum GameState
{
    Start,
    Playing,
    GameOver
}
