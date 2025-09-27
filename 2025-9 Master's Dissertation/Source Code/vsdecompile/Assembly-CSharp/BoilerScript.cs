using System;
using UnityEngine;

// Token: 0x02000040 RID: 64
public class BoilerScript : MonoBehaviour
{
	// Token: 0x06000146 RID: 326 RVA: 0x00002C4B File Offset: 0x00000E4B
	public void BoilWave()
	{
		this.spreadScript.Spread(base.transform.position);
	}

	// Token: 0x06000147 RID: 327 RVA: 0x00002C63 File Offset: 0x00000E63
	public void BoilerStartup()
	{
		ManagerScript.instance.PlayShortSound(this.soundClip[this.index], 0);
		this.index++;
		if (this.index == this.soundClip.Length)
		{
			this.index = 0;
		}
	}

	// Token: 0x06000148 RID: 328 RVA: 0x00002CA3 File Offset: 0x00000EA3
	public void WipeEvent()
	{
		this.animator.SetTrigger("off");
		this.spreadScript.Wipe();
	}

	// Token: 0x04000114 RID: 276
	public BulletSpreadScript spreadScript;

	// Token: 0x04000115 RID: 277
	public Animator animator;

	// Token: 0x04000116 RID: 278
	public Animator[] sideAnimator;

	// Token: 0x04000117 RID: 279
	public AudioClip[] soundClip;

	// Token: 0x04000118 RID: 280
	private int index;
}
