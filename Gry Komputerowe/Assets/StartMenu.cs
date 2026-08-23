using UnityEngine;
using UnityEngine.SceneManagement;

public class StartMenu : MonoBehaviour
{
    [Header("UI")]
    public GameObject startMenuPanel;   // Panel START
    public GameObject aboutPanel;       // Panel ABOUT
    public GameObject hudPanel;         // HUD w trakcie gry
    public GameObject gameOverPanel;    // Panel GAME OVER (opcjonalnie)

    [Header("Gameplay scripts (ONLY gameplay!)")]
    public Behaviour[] gameplayScripts;

    [Header("Objects hidden in menu")]
    public GameObject[] hideInMenu;

    private bool started = false;

    void Start()
    {
        started = false;

        // --- UI ---
        if (startMenuPanel) startMenuPanel.SetActive(true);
        if (aboutPanel) aboutPanel.SetActive(false);
        if (hudPanel) hudPanel.SetActive(false);
        if (gameOverPanel) gameOverPanel.SetActive(false);

        // --- wyłącz gameplay ---
        foreach (var b in gameplayScripts)
            if (b) b.enabled = false;

        // --- ukryj obiekty 3D ---
        foreach (var go in hideInMenu)
            if (go) go.SetActive(false);

        Time.timeScale = 0f;
    }

    // ================= START =================
    public void OnStartButton()
    {
        if (started) return;
        started = true;

        if (ScoreManager.Instance) ScoreManager.Instance.ResetScore();

        Time.timeScale = 1f;

        if (startMenuPanel) startMenuPanel.SetActive(false);
        if (aboutPanel) aboutPanel.SetActive(false);
        if (gameOverPanel) gameOverPanel.SetActive(false);
        if (hudPanel) hudPanel.SetActive(true);

        foreach (var b in gameplayScripts)
            if (b) b.enabled = true;

        foreach (var go in hideInMenu)
            if (go) go.SetActive(true);
    }

    // ================= RESTART (GAME OVER) =================
    public void OnRestartButton()
    {
        Time.timeScale = 1f;

        if (ScoreManager.Instance) ScoreManager.Instance.ResetScore();

        SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex);
    }

    // ================= ABOUT =================
    public void OnAboutButton()
    {
        if (startMenuPanel) startMenuPanel.SetActive(false);
        if (gameOverPanel) gameOverPanel.SetActive(false);
        if (aboutPanel)
        {
            aboutPanel.SetActive(true);
            Debug.Log("AboutPanel ON: " + aboutPanel.activeSelf);
        }
        else
        {
            Debug.LogError("AboutPanel IS NULL");
        }
        Debug.Log("startMenuPanel = " + startMenuPanel.name);
    Debug.Log("aboutPanel = " + aboutPanel.name);
    Debug.Log("startMenuPanel parent = " + startMenuPanel.transform.parent.name);
    Debug.Log("aboutPanel parent = " + aboutPanel.transform.parent.name);
    }

    public void OnBackFromAboutButton()
    {
        if (aboutPanel) aboutPanel.SetActive(false);
        if (started)
        {
            if (hudPanel) hudPanel.SetActive(true);
        }
        else
        {
            if (startMenuPanel) startMenuPanel.SetActive(true);
        }
    }

    // ================= QUIT =================
    public void OnQuitButton()
    {
#if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false;
#else
        Application.Quit();
#endif
    }
}