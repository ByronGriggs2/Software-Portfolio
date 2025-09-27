using System;
using UnityEngine;

// Token: 0x02000012 RID: 18
public class FireballScript : ThrowableScript
{
	// Token: 0x0600004D RID: 77 RVA: 0x000022F0 File Offset: 0x000004F0
	public override void OnCollisionEnter2D()
	{
		this.spreadPoolScript.Ignite(base.transform.position);
		ManagerScript.instance.PlayShortSound(this.soundClip[UnityEngine.Random.Range(0, this.soundClip.Length)], 0);
		this.Destruction();
	}

	// Token: 0x0400002C RID: 44
	public BulletSpreadPoolScript spreadPoolScript;

	// Token: 0x0400002D RID: 45
	public AudioClip[] soundClip;
}
