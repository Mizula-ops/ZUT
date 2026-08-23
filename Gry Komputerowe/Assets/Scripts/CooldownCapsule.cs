using UnityEngine;

public class CooldownCapsule : MonoBehaviour
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
               
                movement.HasCooldownCapsule = true;
                Debug.Log("Gracz ma teraz gwiazdke w ekwipunku");
            }

            
            Destroy(gameObject);
        }
    }
}