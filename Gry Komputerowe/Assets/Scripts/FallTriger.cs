using UnityEngine;
using Unity.Cinemachine;

public class FallTriger : MonoBehaviour
{
    [SerializeField] private string playerTag = "Player";

    [SerializeField] private CinemachineCamera mainCam;
    [SerializeField] private CinemachineCamera fallCam;

    [SerializeField] private Canvas hudCanvas;        
    [SerializeField] private Canvas gameOverCanvas;   

    [SerializeField] private int mainPriority = 10;
    [SerializeField] private int fallPriority = 20;

    private bool _alreadySwitched = false;

    private void OnTriggerEnter(Collider other)
    {
        if (_alreadySwitched) return;

        if (other.CompareTag(playerTag))
        {
            
            if (fallCam != null) fallCam.Priority = fallPriority;
            if (mainCam != null) mainCam.Priority = mainPriority;

         
            if (hudCanvas != null)
                hudCanvas.gameObject.SetActive(false);

            if (gameOverCanvas != null)
                gameOverCanvas.gameObject.SetActive(true);

            _alreadySwitched = true;
        }
        Debug.Log("GAME OVER CANVAS: " + gameOverCanvas.gameObject.name + " active=" + gameOverCanvas.gameObject.activeSelf);
    }
}