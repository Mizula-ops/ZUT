using UnityEngine;
using UnityEngine.InputSystem;


public class Movement : MonoBehaviour
{
    //animacje
    public Animator dustAnimator;
      //publiczne gettery
    public Vector2 MoveInput { get; private set; }
    public bool IsNoCooldownActive()
    {
        return _noCooldownActive;
    }
    [Header("Ruch kuli")]
    public float moveForce = 200f;
    public float maxSpeed = 50f;

    [Header("Sprint")]
    public float sprintMultiplier = 1000f;  
    private bool _isSprinting = false;

    [Header("Skok")]
    public float groundJumpForceImpulse = 10f;
    public float doubleJumpForceImpulse = 5f;
    public float doubleJumpCooldown = 30f;

    [Header("Tiny")]
    public float howTiny = 2f;       
    public float tinyDuration = 5f;    
    public float tinyCooldown = 30f;    
    private bool _is_Tiny = false;

    [Header("Huge")]
    public float howHUGE = 2f;        
    public float hugeDuration = 5f;     
    public float hugeCooldown = 30f;    
    private bool _is_Huge = false;

    [Header("Power-upy")]
    public bool HasCooldownCapsule = false;
    public bool HasgoldStar = false;
    private bool _noCooldownActive = false; 
    private float _noCooldownEndTime = 0f;  

    private Rigidbody _rb;
    private Vector2 _moveInput;

    private bool _jumpQueued = false;
    private bool _isGrounded = false;
    private bool _canJump = false;
    private bool _doubleJumpUsedThisAir = false;

    // Timery
    private float _doubleJumpReadyTime = 0f;
    private float _tinyReadyTime = 0f;
    private float _hugeReadyTime = 0f;
    private float _tinyEndTime = 0f;
    private float _hugeEndTime = 0f;

    // Baza do przywracania
    private Vector3 _baseScale;
    private float _baseMass;


  
    void Start()
    {
        _rb = GetComponent<Rigidbody>();
        _rb.freezeRotation = false;
        _baseScale = transform.localScale;
        _baseMass  = _rb.mass;
         
    }

    void FixedUpdate()
    {
  
        Vector3 wishDir = new Vector3(_moveInput.x, 0f, _moveInput.y).normalized;
        MoveInput = _moveInput;
        float currentMoveForce = moveForce * (_isSprinting ? sprintMultiplier : 1f);
        float currentMaxSpeed  = maxSpeed  * (_isSprinting ? sprintMultiplier : 1f);

        Vector3 horizontalVel = new Vector3(_rb.linearVelocity.x, 0f, _rb.linearVelocity.z);

        if (horizontalVel.magnitude < currentMaxSpeed)
        {
            _rb.AddForce(wishDir * currentMoveForce *5f , ForceMode.Force);
        }

        if (_jumpQueued)
        {
            if (_isGrounded && _canJump)
            {
                DoGroundJump();
            }
            else
            {
                TryDoubleJump();
            }
        }
        _jumpQueued = false;

        // --- ZARZĄDZANIE CZASEM TRWANIA PERKÓW ---
        if (_is_Tiny && Time.time >= _tinyEndTime)
            EndTiny();

        if (_is_Huge && Time.time >= _hugeEndTime)
            EndHuge();

       
        if (_noCooldownActive && Time.time >= _noCooldownEndTime)
        {
            _noCooldownActive = false;
        }
        if (dustAnimator != null)
            dustAnimator.SetBool("run", _isSprinting);
    }

    private void DoGroundJump()
    {
        Vector3 v = _rb.linearVelocity;
        v.y = 0f;
        _rb.linearVelocity = v;
        _rb.AddForce(Vector3.up * groundJumpForceImpulse , ForceMode.Impulse);

        _canJump = false;
        _doubleJumpUsedThisAir = false;
    }

    private void TryDoubleJump()
    {
        bool cooldownReady = _noCooldownActive || Time.time >= _doubleJumpReadyTime;

        if (!_isGrounded && !_doubleJumpUsedThisAir && cooldownReady)
        {
            Vector3 v = _rb.linearVelocity;
            v.y = 0f;
            _rb.linearVelocity = v;
            _rb.AddForce(Vector3.up * doubleJumpForceImpulse, ForceMode.Impulse);

            _doubleJumpUsedThisAir = true;
             if (!_noCooldownActive)
                _doubleJumpReadyTime = Time.time + doubleJumpCooldown;
           
        }
        
    }

    // ---------------- INPUTY ----------------

    public void OnMove(InputAction.CallbackContext ctx)
    {
        _moveInput = ctx.ReadValue<Vector2>();
    }

    public void OnJump(InputAction.CallbackContext ctx)
    {
        if (ctx.performed)
        {
            _jumpQueued = true;
        }
    }

    public void OnSprint(InputAction.CallbackContext ctx)
    {
        if (ctx.performed)
        {
            _isSprinting = true;
            Debug.Log("SPRINT");
        }
        else if (ctx.canceled)
        {
            _isSprinting = false;
        }
    }
    public void OnUseCooldownCapsule(InputAction.CallbackContext ctx)
    {   
        if (!ctx.performed) return;
        if (!HasCooldownCapsule) return;

        Debug.Log("Użyto kapsułki — reset cooldownów!");
        ResetAllCooldowns();
        HasCooldownCapsule = false;
    }

    public void OnUseGoldStar(InputAction.CallbackContext ctx)
    {
        if (!ctx.performed) return;
        if (!HasgoldStar) return;

        Debug.Log("Użyto goldstar — unlimited!");
        ResetAllCooldowns();
        BlockCooldowns(30f);
        HasgoldStar = false;
    }
   
    public void OnTiny(InputAction.CallbackContext ctx)
    {
        if (!ctx.performed) return;
        TryActivateTiny();
    }

   
    public void OnHuge(InputAction.CallbackContext ctx)
    {
        if (!ctx.performed) return;
        TryActivateHuge();
    }
    //`------------POWERUPY------------
    public void ResetAllCooldowns()
    {
    
    _doubleJumpReadyTime = Time.time;
    _tinyReadyTime = Time.time;
    _hugeReadyTime = Time.time;

    Debug.Log("Wszystkie cooldowny zostały zresetowane!");
    }
    public void BlockCooldowns(float duration)
    {
        _noCooldownActive = true;
        _noCooldownEndTime = Time.time + duration;
        Debug.Log($"Cooldowny zablokowane na {duration} sekund!");
    }  


    // ---------------- Tiny / Huge: logika ----------------

    private void TryActivateTiny()
    {
         if (!_noCooldownActive && Time.time < _tinyReadyTime) return;
               
        if (_is_Huge) EndHuge();

        if (_is_Tiny) return;
        

        _is_Tiny = true;
        _tinyEndTime = Time.time + Mathf.Max(0f, tinyDuration);

        float s = Mathf.Max(0.01f, howTiny);
        transform.localScale = _baseScale / s;



        if (!_noCooldownActive)
            _tinyReadyTime = Time.time + Mathf.Max(0f, tinyCooldown);

    
   
       
    }

    private void EndTiny()
    {
        _is_Tiny = false;
        transform.localScale = _baseScale;

    }

    private void TryActivateHuge()
    {
        if (!_noCooldownActive && Time.time < _hugeReadyTime) return;
        
        
    
        if (_is_Tiny) EndTiny();

        if (_is_Huge) return;
        
      
        _is_Huge = true;
        _hugeEndTime = Time.time + Mathf.Max(0f, hugeDuration);

        float s = Mathf.Max(0.01f, howHUGE);
        transform.localScale = _baseScale * s;

        _rb.mass = _baseMass * (s * s * s);

        if (!_noCooldownActive)
        _hugeReadyTime = Time.time + Mathf.Max(0f, hugeCooldown);


    }

    private void EndHuge()
    {
        _is_Huge = false;
        transform.localScale = _baseScale;
        _rb.mass = _baseMass;
        Debug.Log("Huge OFF");
    }

    // -------------- KOLIZJE / GROUND --------------

    void OnCollisionEnter(Collision collision)
    {
        bool touchedGroundFromBelow = false;

        foreach (ContactPoint contact in collision.contacts)
        {
            if (Vector3.Dot(contact.normal, Vector3.up) > 0.5f)
            {
                touchedGroundFromBelow = true;
                break;
            }
        }

        if (touchedGroundFromBelow)
        {
            _isGrounded = true;
            _canJump = true;
            _doubleJumpUsedThisAir = false;
        }
    }

    void OnCollisionStay(Collision collision)
    {
        bool touchingGround = false;

        foreach (ContactPoint contact in collision.contacts)
        {
            if (Vector3.Dot(contact.normal, Vector3.up) > 0.5f)
            {
                touchingGround = true;
                break;
            }
        }

        _isGrounded = touchingGround;
    }

    void OnCollisionExit(Collision collision)
    {
        _isGrounded = false;
    }

    // -------------- FUNKCJE DLA UI --------------
     
    public float GetDoubleJumpCooldownRemaining()
    {
        float remain = _doubleJumpReadyTime - Time.time;
        return Mathf.Max(remain, 0f);
    }

    public float GetDoubleJumpCharge01()
    {
        float remain = GetDoubleJumpCooldownRemaining();
        if (doubleJumpCooldown <= 0f) return 1f;
        float charge = 1f - (remain / doubleJumpCooldown);
        return Mathf.Clamp01(charge);
    }

    public float GetTinyCooldownRemaining()
    {
        float remain = _tinyReadyTime - Time.time;
        return Mathf.Max(remain, 0f);
    }

    public float GetTinyCharge01()
    {
        if (tinyCooldown <= 0f) return 1f;
        float remain = GetTinyCooldownRemaining();
        float charge = 1f - (remain / tinyCooldown);
        return Mathf.Clamp01(charge);
    }

    public float GetHugeCooldownRemaining()
    {
        float remain = _hugeReadyTime - Time.time;
        return Mathf.Max(remain, 0f);
    }

    public float GetHugeCharge01()
    {
        if (hugeCooldown <= 0f) return 1f;
        float remain = GetHugeCooldownRemaining();
        float charge = 1f - (remain / hugeCooldown);
        return Mathf.Clamp01(charge);
    }
    
}