using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CPlayerController : MonoBehaviour
{
    private bool isLoadStation = false;
    Rigidbody rb;
    public float force = 1.0f;
    public float forceTurn = 1.0f;
    public GameObject tankPrefab;
    // Start is called before the first frame update
    void Start()
    {
        rb = GetComponent<Rigidbody>();
    }
    // Update is called once per frame
    void Update()
    {
        // Reads the [WSAD] input to control the player movement.
        if (Input.GetKey(KeyCode.W))
        {
            rb.AddForce(transform.forward * force);
        }
        if (Input.GetKey(KeyCode.S))
        {
            rb.AddForce(-transform.forward * force);
        }
        if (Input.GetKey(KeyCode.D))
        {
            rb.AddTorque(transform.up * forceTurn);
        }
        if (Input.GetKey(KeyCode.A))
        {
            rb.AddTorque(-transform.up * forceTurn);
        }
        if (Input.GetKeyDown(KeyCode.Space))
        {
            if (isLoadStation == true)
            {
                // pozycja względem ciężarówki
                Vector3 pos = transform.position + new Vector3(-0.5f, 2.0f, 0);
                // rotacja czołgu
                Quaternion rot = Quaternion.Euler(90.0f, 0, 0);
                // stworzenie nowego obiektu czołgu
                Instantiate(tankPrefab, pos, rot);
            }
        }
    }
    void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.CompareTag("tagLoadStation"))
        {
            isLoadStation = true;
            Debug.Log("We entered the Load Station");
        }
        else if (other.gameObject.CompareTag("tagUnloadStation"))
        {
            Debug.Log("We entered the Unload Station");
        }
    }

    void OnTriggerExit(Collider other)
    {
        if (other.gameObject.CompareTag("tagLoadStation"))
        {
            isLoadStation = false;
            Debug.Log("We left the Load Station");
        }
        else if (other.gameObject.CompareTag("tagUnloadStation"))
        {
            Debug.Log("We left the Unload Station");
        }
    }
}
