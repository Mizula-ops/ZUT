using UnityEngine;

public class goldStar : MonoBehaviour
{
    [SerializeField] private string playerTag = "Player";
    private void OnTriggerEnter(Collider other)
    {
        if (other.CompareTag(playerTag))
        {
            Debug.Log("Podniesiono kapsułkę resetującą cooldowny!");

           
            Movement movement = other.GetComponent<Movement>();

            if (movement != null)
            {
                
                movement.HasgoldStar = true;
                Debug.Log("Gracz ma teraz kapsułkę w ekwipunku");
            }

            Destroy(gameObject);
        }
    }
}
