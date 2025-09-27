using System;
using UnityEngine;

// Token: 0x02000041 RID: 65
public class CoilboxScript : MonoBehaviour
{
	// Token: 0x0600014A RID: 330 RVA: 0x00002CC0 File Offset: 0x00000EC0
	private void Start()
	{
		this.origin = new Vector2(base.transform.position.x, base.transform.position.y - 0.27f);
	}

	// Token: 0x0600014B RID: 331 RVA: 0x00002CF3 File Offset: 0x00000EF3
	public void BoilWave()
	{
		this.shock.Emit(this.origin, true, Vector2.down);
	}

	// Token: 0x0600014C RID: 332 RVA: 0x00002D11 File Offset: 0x00000F11
	public void WipeEvent()
	{
		this.animator.SetTrigger("off");
		this.shock.Destruction();
	}

	// Token: 0x0600014D RID: 333 RVA: 0x00006B60 File Offset: 0x00004D60
	public void SoundPlay()
	{
		if (this.audible)
		{
			ManagerScript.instance.PlayShortSound(this.soundClip[this.index], 0);
			this.index++;
			if (this.index == 2)
			{
				this.index = 0;
			}
		}
	}

	// Token: 0x04000119 RID: 281
	public ShockwaveScript shock;

	// Token: 0x0400011A RID: 282
	public Animator animator;

	// Token: 0x0400011B RID: 283
	private Vector2 origin;

	// Token: 0x0400011C RID: 284
	public bool audible;

	// Token: 0x0400011D RID: 285
	public AudioClip[] soundClip;

	// Token: 0x0400011E RID: 286
	public int index;
}
