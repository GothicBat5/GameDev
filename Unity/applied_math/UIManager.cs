using UnityEngine;
using TMPro;
using DG.Tweening;
using UnityEngine.UI;

public class UIManager : MonoBehaviour
{
    [Header("Animation settings")]
    public Ease easeType;
    public float duration;
    public RectTransform gameoverRect;

    public TextMeshProUGUI gameOverLabel;
    public TextMeshProUGUI scoreLabel;
    public TextMeshProUGUI highScoreLabel;
    public TextMeshProUGUI gameOverHighScoreLabel;
    public TextMeshProUGUI healthLabel;

    public Button restartBtn;



    private void Start()
    {
        restartBtn.onClick.AddListener(GameManager.Instance.RestartGame);
        //scoreLabel.text = "Score: " + GameManager.Instance.score;
        SetScoreLabel();
        SetHighScoreLabel();
        SetHealthLabel();
    }

    private void Update()
    {
        if (Input.GetKeyDown(KeyCode.B))
        {
            ShowGameOverUI();
        }

        //SetHealthLabel();
    }

    public void SetScoreLabel()
    {
        scoreLabel.text = "Score: " + GameManager.Instance.score;
    }

        //add the set high score label 
    public void SetHighScoreLabel()
    {
        highScoreLabel.text = "High Score: " + GameManager.Instance.highScore;
        gameOverHighScoreLabel.text = "High Score: " + GameManager.Instance.highScore;
    }

    public void SetHealthLabel()
    {
        healthLabel.text = "Health: " + GameManager.Instance.health;
    }

    public void ShowGameOverUI(bool isGameOver = true)
    {
        Sequence sequence = DOTween.Sequence();
        sequence.Append(gameoverRect.DOScale(Vector3.one,duration).SetEase(easeType));
        gameOverLabel.text = isGameOver ? "GAME OVER" : "GAME WON";
    }

    public void HideGameOverUI()
    {
        Sequence sequence = DOTween.Sequence();
        sequence.Append(gameoverRect.DOScale(Vector3.zero, duration).SetEase(easeType));
    }
}
