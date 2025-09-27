using System;
using UnityEngine;

// Token: 0x02000019 RID: 25
public class MinionDeathScript : MonoBehaviour
{
	// Token: 0x0600006C RID: 108 RVA: 0x00002458 File Offset: 0x00000658
	private void Start()
	{
		this.a.SetInteger("explo", UnityEngine.Random.Range(1, 4));
		UnityEngine.Object.Destroy(base.gameObject, 1f);
	}

	// Token: 0x04000040 RID: 64
	public Animator a;
}
