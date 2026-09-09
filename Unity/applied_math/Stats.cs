using UnityEngine;
using TMPro;
using DG.Tweening;
using UnityEngine.UI;

public class Stats : MonoBehaviour
{   
    [SerializeField] public int playerLevel;
    [SerializeField] public int health;
    [SerializeField] public int strength;
    [SerializeField] public int agility;
    [SerializeField] public float intelligence;

    public TMP_Text playerLevelText;
    public TMP_Text healthText;
    public TMP_Text strengthText;
    public TMP_Text agilityText;
    public TMP_Text intelligenceText;

    void Start()
    {
        playerLevelText.text = "Player Level: " + playerLevel.ToString();
        healthText.text = "Health: " +health.ToString();
        strengthText.text = "Stregth: " +strength.ToString();
        agilityText.text = "Agility: " +agility.ToString();
        intelligenceText.text = "Intelligence: " +intelligence.ToString();
    }
}
