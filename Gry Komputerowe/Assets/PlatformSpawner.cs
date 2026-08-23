using System.Collections.Generic;
using UnityEngine;

public class PlatformSpawner : MonoBehaviour
{
    [Header("Platform Prefabs (kolejność ma znaczenie)")]
    public List<GameObject> platformPrefabs;

    [Header("Spawn")]
    public int startCount = 4;
    public float segmentLength = 160f;

    [Header("Movement")]
    public float speed = 6f;

    [Header("Warning Effect")]
    public float warningDistance = 30f;
    public Color normalColor = Color.white;
    public Color warningColor = Color.red;
    public bool blinkWarning = false;
    public float blinkSpeed = 6f;

    [Header("Score")]
    public bool resetScoreOnStart = true; // opcjonalnie

    private Queue<Transform> activePlatforms = new Queue<Transform>();
    private Transform lastPlatform;
    private int prefabIndex = 0;

    void Start()
    {
        if (platformPrefabs == null || platformPrefabs.Count == 0)
        {
            Debug.LogError("platformPrefabs jest puste!");
            return;
        }

        // (opcjonalnie) reset wyniku przy starcie
        if (resetScoreOnStart && ScoreManager.Instance != null)
            ScoreManager.Instance.ResetScore();

        float z = 0f;
        prefabIndex = 0;

        for (int i = 0; i < startCount; i++)
        {
            GameObject prefab = platformPrefabs[prefabIndex];
            prefabIndex = (prefabIndex + 1) % platformPrefabs.Count;

            GameObject obj = Instantiate(prefab, new Vector3(0f, 0f, z), Quaternion.identity);

            // upewnij się, że platforma ma Rigidbody (kinematic)
            EnsureKinematicRb(obj);

            activePlatforms.Enqueue(obj.transform);
            SetPlaneColor(obj.transform, normalColor);

            lastPlatform = obj.transform;
            z += segmentLength;
        }
    }

    void FixedUpdate()
    {
        if (activePlatforms.Count == 0) return;

        // 1) ruch w kroku fizyki
        float dz = -speed * Time.fixedDeltaTime;
        Vector3 delta = new Vector3(0f, 0f, dz);

        foreach (Transform p in activePlatforms)
        {
            Rigidbody rb = p.GetComponent<Rigidbody>();
            if (rb != null)
                rb.MovePosition(rb.position + delta);
            else
                p.position += delta; // awaryjnie (ale lepiej mieć RB)
        }

        // 2) warning + recykling
        Transform first = activePlatforms.Peek();
        float firstEndZ = first.position.z + segmentLength;

        // ostrzeżenie
        if (firstEndZ <= warningDistance && firstEndZ > 0f)
        {
            if (blinkWarning)
            {
                float t = Mathf.PingPong(Time.time * blinkSpeed, 1f);
                Color blink = Color.Lerp(normalColor, warningColor, t);
                SetPlaneColor(first, blink);
            }
            else
            {
                SetPlaneColor(first, warningColor);
            }
        }
        else
        {
            SetPlaneColor(first, normalColor);
        }

        // 3) recykling + SCORE
        if (firstEndZ <= 0f)
        {
            // +1 platforma zaliczona
            if (ScoreManager.Instance != null)
                ScoreManager.Instance.AddPlatformPassed();

            Transform recycled = activePlatforms.Dequeue();

            float newZ = lastPlatform.position.z + segmentLength;
            Vector3 newPos = new Vector3(recycled.position.x, recycled.position.y, newZ);

            Rigidbody rb = recycled.GetComponent<Rigidbody>();
            if (rb != null)
                rb.MovePosition(newPos);
            else
                recycled.position = newPos;

            SetPlaneColor(recycled, normalColor);

            lastPlatform = recycled;
            activePlatforms.Enqueue(recycled);
        }
    }

    void SetPlaneColor(Transform platform, Color c)
    {
        var vis = platform.GetComponent<PlatformVisual>();
        if (vis) vis.SetColor(c);
    }

    void EnsureKinematicRb(GameObject platformObj)
    {
        Rigidbody rb = platformObj.GetComponent<Rigidbody>();
        if (rb == null) rb = platformObj.AddComponent<Rigidbody>();

        rb.isKinematic = true;
        rb.useGravity = false;
        rb.interpolation = RigidbodyInterpolation.Interpolate;
    }
}