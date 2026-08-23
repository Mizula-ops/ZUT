using System.Collections;
using UnityEngine;
using UnityEngine.AI;

public class OffMeshJump : MonoBehaviour
{
    public NavMeshAgent agent;
    public float jumpDuration = 0.4f;
    public float jumpHeight = 0.6f;

    bool jumping = false;

    void Start()
    {
        if (!agent) agent = GetComponent<NavMeshAgent>();
        agent.autoTraverseOffMeshLink = false;
    }

    void Update()
    {
        if (!jumping && agent.isOnOffMeshLink)
            StartCoroutine(Jump());
    }

    IEnumerator Jump()
    {
        jumping = true;

        OffMeshLinkData data = agent.currentOffMeshLinkData;
        Vector3 start = transform.position;
        Vector3 end = data.endPos;

        float t = 0f;
        while (t < 1f)
        {
            t += Time.deltaTime / jumpDuration;
            Vector3 pos = Vector3.Lerp(start, end, t);
            pos.y += Mathf.Sin(t * Mathf.PI) * jumpHeight;
            transform.position = pos;
            yield return null;
        }

        agent.CompleteOffMeshLink();
        jumping = false;
    }
}