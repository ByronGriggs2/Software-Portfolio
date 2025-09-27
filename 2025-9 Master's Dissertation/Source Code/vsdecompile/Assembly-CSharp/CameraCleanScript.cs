using System;
using UnityEngine;

// Token: 0x0200002E RID: 46
public class CameraCleanScript : MonoBehaviour
{
	// Token: 0x060000D3 RID: 211 RVA: 0x0000537C File Offset: 0x0000357C
	private void OnPreCull()
	{
		if (Application.isEditor)
		{
			return;
		}
		Rect rect = Camera.main.rect;
		Rect rect2 = new Rect(0f, 0f, 1f, 1f);
		Camera.main.rect = rect2;
		GL.Clear(true, true, Color.black);
		Camera.main.rect = rect;
	}
}
