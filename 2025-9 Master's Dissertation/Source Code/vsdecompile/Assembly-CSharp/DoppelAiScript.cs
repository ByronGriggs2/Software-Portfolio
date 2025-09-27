using System;
using UnityEngine;

// Token: 0x0200004A RID: 74
public class DoppelAiScript : GhostAiScript
{
	// Token: 0x06000187 RID: 391 RVA: 0x000076CC File Offset: 0x000058CC
	private void Update()
	{
		if (this.busy)
		{
			return;
		}
		float num = Vector2.Distance(base.transform.position, this.playerTransform.position);
		if (num < 2f && !this.snapReady)
		{
			this.animator.SetTrigger("isReady");
			ManagerScript.instance.PlayShortSound(this.openClip[UnityEngine.Random.Range(0, this.openClip.Length)], 0);
			this.snapReady = true;
			this.GrindEnd();
			return;
		}
		if (num > 4f && this.snapReady)
		{
			this.animator.SetTrigger("isUnready");
			this.snapReady = false;
		}
	}

	// Token: 0x06000188 RID: 392 RVA: 0x00007780 File Offset: 0x00005980
	protected override void InRangeAction()
	{
		this.animator.SetTrigger("isAttacking");
		this.GrindEnd();
		ManagerScript.instance.PlayShortSound(this.closeClip[UnityEngine.Random.Range(0, this.closeClip.Length)], 0);
		base.transform.position = this.playerTransform.position;
		this.rotationScript.Gameover(this.playerTransform, true, 1);
		base.StartCoroutine(this.PostAction(1.3f));
	}

	// Token: 0x06000189 RID: 393 RVA: 0x00002F66 File Offset: 0x00001166
	public void GrindEnd()
	{
		if (!this.isGrinding)
		{
			return;
		}
		this.isGrinding = false;
		if (this.grindSource == null)
		{
			return;
		}
		UnityEngine.Object.Destroy(this.grindSource.gameObject);
	}

	// Token: 0x0600018A RID: 394 RVA: 0x00007800 File Offset: 0x00005A00
	public void GrindAudio()
	{
		this.isGrinding = true;
		this.grindSource = ManagerScript.instance.PlayShortSound(this.grindClip[this.grindIndex], 0);
		this.grindIndex++;
		if (this.grindIndex == this.grindClip.Length)
		{
			this.grindIndex = 0;
		}
	}

	// Token: 0x04000152 RID: 338
	private bool snapReady;

	// Token: 0x04000153 RID: 339
	public AudioClip[] grindClip;

	// Token: 0x04000154 RID: 340
	public AudioClip[] openClip;

	// Token: 0x04000155 RID: 341
	public AudioClip[] closeClip;

	// Token: 0x04000156 RID: 342
	private AudioSource grindSource;

	// Token: 0x04000157 RID: 343
	private bool isGrinding;

	// Token: 0x04000158 RID: 344
	private int grindIndex;
}
