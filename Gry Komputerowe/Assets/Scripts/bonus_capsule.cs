using UnityEngine;
using UnityEngine.UI;

public class CapsuleIconUI : MonoBehaviour
{
    [SerializeField] private Movement player;  
    [SerializeField] private Image icon;       

    [Header("Kolory")]
    public Color noCapsuleColor = Color.white;  
    public Color blinkColorA = Color.white;     
    public Color blinkColorB = Color.black;    

    [Header("Parametry migania")]
    public float blinkSpeed = 2f;             

    private void Update()
    {
        
        if (player.HasCooldownCapsule)
        {
            float t = (Mathf.Sin(Time.time * blinkSpeed * Mathf.PI) + 1f) / 2f;
            icon.color = Color.Lerp(blinkColorA, blinkColorB, t);
        }
        else
        {
            icon.color = noCapsuleColor;
        }
    }
}