using UnityEngine;
using UnityEngine.UI;

public class HugeSlider : MonoBehaviour
{
    [Header("Referencje")]
    public Movement player;   
    public Slider bar;      
    public Image fillImage;  
    public Image iconImage;  

    [Header("Kolory")]
    public Color readyColor = Color.green;
    public Color midColor = new Color(1f, 0.65f, 0f); 
    public Color cooldownColor = Color.red;

    void Update()
    {
        if (player == null || bar == null)
            return;

        float charge = player.GetHugeCharge01();
        bar.value = charge;

     
        if (fillImage != null)
            fillImage.color = Color.Lerp(cooldownColor, readyColor, charge);

       
        if (iconImage != null)
        {
            if (charge >= 0.99f)
                iconImage.color = readyColor;          
            else if (charge >= 0.5f)
                iconImage.color = midColor;            
            else
                iconImage.color = cooldownColor;       
        }
    }
}