using UnityEngine;

public class AboutMenu : MonoBehaviour
{
    [Header("Panels")]
    public GameObject aboutPanel;      // ten Canvas / Panel ABOUT
    public GameObject startMenuPanel;  // panel START

    void Start()
    {
        // Na wszelki wypadek: ABOUT wyłączony na starcie
        if (aboutPanel)
            aboutPanel.SetActive(false);
    }

    public void OnBackButton()
    {
        if (aboutPanel)
            aboutPanel.SetActive(false);

        if (startMenuPanel)
            startMenuPanel.SetActive(true);
    }
}