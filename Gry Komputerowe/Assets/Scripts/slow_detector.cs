using UnityEngine;

public class slow_detector : MonoBehaviour
{
    [SerializeField] private string playerTag = "Player";

    [Header("Parametry spowolnienia")]
    public float slowMultiplier = 0.1f; 
    public float maxSpeedOnPlatform = 3f; 

    private Rigidbody _rb;

    private void OnTriggerEnter(Collider other)
    {
        if (other.CompareTag(playerTag))
        {
            _rb = other.GetComponent<Rigidbody>();

            if (_rb != null)
            {
                Debug.Log("Kulka wjechała na spowalniającą platformę");

                _rb.linearVelocity = _rb.linearVelocity * slowMultiplier;
                _rb.angularVelocity = _rb.angularVelocity * slowMultiplier;
            }
        }
    }

    private void OnTriggerStay(Collider other)
    {
        if (other.CompareTag(playerTag))
        {
            if (_rb != null)
            {
                
                if (_rb.linearVelocity.magnitude > maxSpeedOnPlatform)
                {
                    _rb.linearVelocity = _rb.linearVelocity.normalized * maxSpeedOnPlatform;
                }
            }
        }
    }

    private void OnTriggerExit(Collider other)
    {
        if (other.CompareTag(playerTag))
        {
            if (_rb != null)
            {
                Debug.Log("Kulka opuściła spowalniającą platformę");
                _rb = null;
            }
        }
    }
}