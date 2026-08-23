using UnityEngine;

public class DustFollow : MonoBehaviour
{
    public Transform player;
    public Vector3 offset;

    void LateUpdate()
    {
        transform.position = player.position + offset;
     
    }
}