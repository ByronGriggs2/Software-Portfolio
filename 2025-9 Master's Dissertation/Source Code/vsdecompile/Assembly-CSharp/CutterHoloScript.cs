using System;
using UnityEngine;

// Token: 0x0200000E RID: 14
public class CutterHoloScript : MonoBehaviour
{
	// Token: 0x06000042 RID: 66 RVA: 0x00004224 File Offset: 0x00002424
	public void HoloCast(Vector3 origin, Vector2 dir)
	{
		base.transform.position = origin;
		float num = Mathf.Atan2(dir.y, dir.x) * 57.29578f;
		if (num < 0f)
		{
			num += 360f;
		}
		base.transform.eulerAngles = new Vector3(0f, 0f, num);
		this.animator.SetTrigger("cast");
	}

	// Token: 0x04000027 RID: 39
	public Animator animator;
}
