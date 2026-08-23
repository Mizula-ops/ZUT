using UnityEngine;
using UnityEngine.AI;

public class ObstacleTrigger : MonoBehaviour
{
    public NavMeshObstacle obstacle;
    public Transform player;

    public float reenableDelay = 0f; // 0 = nie włączaj ponownie

    void Start()
    {
        if (obstacle == null)
            obstacle = GetComponent<NavMeshObstacle>();
    }

    void OnTriggerEnter(Collider other)
    {
        if (other.transform == player && obstacle.enabled)
        {
            DisableObstacle();

            if (reenableDelay > 0f)
                Invoke(nameof(EnableObstacle), reenableDelay);
        }
    }

    void DisableObstacle()
    {
        obstacle.enabled = false;
        Debug.Log($"[NavMeshObstacle] WYŁĄCZONY → {gameObject.name}");
    }

    void EnableObstacle()
    {
        obstacle.enabled = true;
        Debug.Log($"[NavMeshObstacle] WŁĄCZONY → {gameObject.name}");
    }
}