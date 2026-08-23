using UnityEngine;
using UnityEngine.UI;

public class DoubleJumpUI : MonoBehaviour
{
    [Header("Referencje")]
    public Movement player;   // kula z Movement.cs
    public Slider bar;        // Twój pasek (Slider)
    public Image fillImage;   // Fill (kolorowy pasek)
    public Image iconImage;   // Twoja ikonka double jumpa

    [Header("Kolory")]
    public Color readyColor = Color.green;
    public Color midColor = new Color(1f, 0.65f, 0f); // pomarańczowy
    public Color cooldownColor = Color.red;

    void Update()
    {
        if (player == null || bar == null)
            return;

        float charge = player.GetDoubleJumpCharge01();
        bar.value = charge;

        // zmiana koloru paska
        if (fillImage != null)
            fillImage.color = Color.Lerp(cooldownColor, readyColor, charge);

        // zmiana koloru ikony
        if (iconImage != null)
        {
            if (charge >= 0.99f)
                iconImage.color = readyColor;           // gotowy (zielony)
            else if (charge >= 0.5f)
                iconImage.color = midColor;             // w połowie (pomarańczowy)
            else
                iconImage.color = cooldownColor;        // świeżo po użyciu (czerwony)
        }
    }
}