using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
public class CTimer : MonoBehaviour
{
    public float mTime;
    public TMP_Text mTimer;
    [SerializeField] TextMeshProUGUI obj;
    public TMP_Text mCounterText;  
    public int totalTanks = 2;      
    public int transportedTanks = 0;
    public bool isGameOver;
    // Start is called before the first frame update
    void Start()
    {
        mTime = 0;
        isGameOver = false;
        mTimer = GetComponent<TextMeshProUGUI>();
        UpdateCounterText();
    }
    // Update is called once per frame
    void Update()
    {
        if (isGameOver == false)
        {
            mTime += Time.deltaTime;
            float minutes = Mathf.FloorToInt(mTime / 60);
            float seconds = Mathf.FloorToInt(mTime % 60);
            mTimer.text = string.Format("{0:00}:{1:00}", minutes, seconds);
        }
    }
    public void IncrementTankCount()
    {
        transportedTanks++;
        UpdateCounterText();

        if (transportedTanks >= totalTanks)
        {
            isGameOver = true;
            mTimer.text = "Game Over\nTime: " + mTimer.text;  // pokazujemy Game Over + czas
            mCounterText.text = ""; // możemy ukryć licznik
        }
    }

    void UpdateCounterText()
    {
        mCounterText.text = transportedTanks + " / " + totalTanks;
    }
}