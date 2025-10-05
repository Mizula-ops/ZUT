using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CFollowPlayer : MonoBehaviour
{
    public GameObject player;
    public float offset = 10;

    void Start()
    {
        // Ustawienie kąta patrzenia kamery
        transform.eulerAngles = new Vector3(80, 0, 0);
    }

    void Update()
    {
        transform.position = player.transform.position + new Vector3(0, offset, 0);
        transform.eulerAngles = new Vector3(80, player.transform.eulerAngles.y, 0);
    }
}
