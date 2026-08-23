using UnityEngine;

public class PlaneTouchDetector : MonoBehaviour
{
    [SerializeField] private string playerTag = "Player";
    
    private void OnTriggerEnter(Collider other)
    {
        if (other.CompareTag(playerTag))
        {
            Debug.Log("Dotknięto plane (trigger) – start kontaktu");
            Rigidbody _rb= other.GetComponent<Rigidbody>();
             Movement movement = other.GetComponent<Movement>();

             float moveForce =movement.moveForce;
            Vector2 mi = movement.MoveInput;
            Vector3 wishDir = new Vector3(mi.x, 0f, mi.y).normalized;
            _rb.AddForce(wishDir * moveForce * 10f, ForceMode.Impulse);
        }
    }

    private void OnTriggerStay(Collider other)
    {
        if (other.CompareTag(playerTag))
        {
          
        }
    }

    private void OnTriggerExit(Collider other)
    {
        if (other.CompareTag(playerTag))
        {
            Debug.Log("Koniec kontaktu z plane (trigger)");
        }
    }
}