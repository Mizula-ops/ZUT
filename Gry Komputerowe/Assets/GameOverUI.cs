using TMPro;
using UnityEngine;

public class GameOverUI : MonoBehaviour
{
    public TMP_Text gameOverText;   // tu wrzuć TMP Text z "GAME OVER"

    void OnEnable()
    {
        int passed = ScoreManager.Instance ? ScoreManager.Instance.PlatformsPassed : 0;
        gameOverText.text = $"PLATFORMS PASSED: {passed}";
    }
}