using UnityEngine;

public class cube_break : MonoBehaviour
{
    [SerializeField] private string playerTag = "Player"; 
    [SerializeField] private int hitsToDestroy = 3;

    private int currentHits = 0;
    private Animator animator;
    private bool isBroken = false;

    private void Start()
    {
        animator = GetComponent<Animator>();
    }

    private void OnCollisionEnter(Collision collision)
    {
        if (isBroken) return;

        if (collision.gameObject.CompareTag(playerTag))
        {
            currentHits++;
            Debug.Log($"Kostka trafiona! Liczba uderzeń: {currentHits}/{hitsToDestroy}");

          
            if (currentHits < hitsToDestroy)
            {
                if (animator != null)
                    animator.SetTrigger("HitTrigger");
            }
            else
            {
                
                isBroken = true;

                Debug.Log("💥 Kostka zniszczona!");
                
        

                Destroy(gameObject, 0.2f);  
            }
        }
    }
}