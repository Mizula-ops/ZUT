using UnityEngine;
using UnityEngine.AI;

public class CubeAI : MonoBehaviour
{
    public NavMeshAgent agent;
    public Transform player;

    [Header("Chase")]
    public float chaseDistance = 10f;

    [Header("Patrol")]
    public Transform pointA;
    public Transform pointB;
    private Transform currentTarget;

    [Header("Attack / Knockback")]
    public float attackDistance = 1.8f;     
    public float attackCooldown = 0.8f;    
    public float knockbackForce = 25f;      
    public float knockbackUpForce = 2f;     
    public bool zeroPlayerVelocityBeforeHit = true;

    private float nextAttackTime;

    void Start()
    {
        if (agent == null)
            agent = GetComponent<NavMeshAgent>();

        currentTarget = pointA;
    }

    void Update()
    {
        if (player != null)
        {
            float dist = Vector3.Distance(transform.position, player.position);

        
            if (dist <= attackDistance)
            {
                Attack();
                return;
            }

        
            if (dist <= chaseDistance)
            {
                Chase();
                return;
            }
        }

     
        Patrol();
    }

    void Chase()
    {
        agent.isStopped = false;
        agent.SetDestination(player.position);
    }

    void Patrol()
    {
        if (currentTarget == null) return;

        agent.isStopped = false;
        agent.SetDestination(currentTarget.position);

        if (!agent.pathPending && agent.remainingDistance <= agent.stoppingDistance)
        {
            currentTarget = (currentTarget == pointA) ? pointB : pointA;
        }
    }

    void Attack()
    {
     
        agent.isStopped = true;

        
        Vector3 lookDir = player.position - transform.position;
        lookDir.y = 0f;
        if (lookDir.sqrMagnitude > 0.001f)
            transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.LookRotation(lookDir), 12f * Time.deltaTime);

   
        if (Time.time < nextAttackTime) return;
        nextAttackTime = Time.time + attackCooldown;

  
        Rigidbody rb = player.GetComponent<Rigidbody>();
        if (rb == null) return;

      
        rb.constraints = RigidbodyConstraints.FreezeRotation;

        if (zeroPlayerVelocityBeforeHit)
        {
           
            rb.linearVelocity = Vector3.zero;
        }

        Vector3 dir = (player.position - transform.position).normalized;
        dir.y = 0f;

        Vector3 force = dir * knockbackForce + Vector3.up * knockbackUpForce;
        rb.AddForce(force, ForceMode.Impulse);
    }
}