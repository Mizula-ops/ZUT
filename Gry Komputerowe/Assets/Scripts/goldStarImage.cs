using UnityEngine;
using UnityEngine.UI;

public class goldStarImage : MonoBehaviour
{
    [SerializeField] private Movement player;  
    [SerializeField] private Image icon;       


    public Color noStarColor = Color.white;                          
    public Color blinkColorA = Color.yellow;             
    public Color blinkColorB = Color.black;                         
    public Color activeColor = Color.yellow;            

    public float blinkSpeed = 2f;                                   

    private void Update()
    {
        if (player == null || icon == null)
            return;

        
        if (player.IsNoCooldownActive())
        {
            icon.color = activeColor; 
        }
        else if (player.HasgoldStar)
        {
            float t = (Mathf.Sin(Time.time * blinkSpeed * Mathf.PI) + 1f) / 2f;
            icon.color = Color.Lerp(blinkColorA, blinkColorB, t); 
        }
       
        else
        {
            icon.color = noStarColor; 
        }
    }
}