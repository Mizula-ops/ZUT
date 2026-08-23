using UnityEngine;

public class PlatformVisual : MonoBehaviour
{
    public Renderer warningRenderer; // tu wrzucasz Plane (Renderer)

    private MaterialPropertyBlock mpb;

    void Awake()
    {
        mpb = new MaterialPropertyBlock();
    }

    public void SetColor(Color c)
    {
        if (!warningRenderer) return;

        warningRenderer.GetPropertyBlock(mpb);

        // URP
        if (warningRenderer.sharedMaterial && warningRenderer.sharedMaterial.HasProperty("_BaseColor"))
            mpb.SetColor("_BaseColor", c);
        // Standard
        else
            mpb.SetColor("_Color", c);

        warningRenderer.SetPropertyBlock(mpb);
    }
}