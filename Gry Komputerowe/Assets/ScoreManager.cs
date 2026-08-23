using UnityEngine;

public class ScoreManager : MonoBehaviour
{
    public static ScoreManager Instance { get; private set; }

    public int PlatformsPassed { get; private set; }

    void Awake()
    {
        if (Instance != null && Instance != this)
        {
            Destroy(gameObject);
            return;
        }
        Instance = this;
        DontDestroyOnLoad(gameObject);
    }

    public void ResetScore()
    {
        PlatformsPassed = 0;
    }

    public void AddPlatformPassed()
    {
        PlatformsPassed++;
    }
}