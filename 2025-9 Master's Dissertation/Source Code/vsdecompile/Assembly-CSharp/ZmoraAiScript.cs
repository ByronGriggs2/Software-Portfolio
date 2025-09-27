using System;
using System.Collections;
using UnityEngine;

// Token: 0x0200005A RID: 90
public class ZmoraAiScript : GhostAiScript
{
	// Token: 0x060001E5 RID: 485 RVA: 0x00003231 File Offset: 0x00001431
	protected override void InRangeAction()
	{
		if (!this.specialReady)
		{
			base.StartCoroutine(this.Melee());
			return;
		}
		if (this.lvl == 2)
		{
			base.StartCoroutine(this.Slam(1));
			return;
		}
		base.StartCoroutine(this.SuperSlam());
	}

	// Token: 0x060001E6 RID: 486 RVA: 0x0000326E File Offset: 0x0000146E
	private IEnumerator Melee()
	{
		this.animator.SetTrigger("isAttacking");
		ManagerScript.instance.PlayShortSound(this.dashClip[UnityEngine.Random.Range(0, this.dashClip.Length)], 0);
		yield return new WaitForSeconds(0.3f);
		if (this.lvl != 1)
		{
			this.specialReady = true;
		}
		Vector2 normalized = (this.aim - this.rb.position).normalized;
		this.rb.AddForce(normalized * this.dashPower);
		this.deathCollider.enabled = true;
		if (this.lvl < 3)
		{
			base.StartCoroutine(this.PostAction(1.3f));
		}
		else
		{
			this.shock[0].Emit(base.transform.position, true, normalized);
		}
		yield return new WaitForSeconds(0.2f);
		this.deathCollider.enabled = false;
		yield return new WaitForSeconds(0.2f);
		if (this.lvl == 3)
		{
			base.StartCoroutine(this.Slam(3));
		}
		yield break;
	}

	// Token: 0x060001E7 RID: 487 RVA: 0x0000327D File Offset: 0x0000147D
	private IEnumerator Slam(int slams)
	{
		this.slamCount += slams;
		if (this.lvl == 2)
		{
			this.specialReady = false;
		}
		this.animator.SetTrigger("isJumping");
		ManagerScript.instance.PlayShortSound(this.slamClip[UnityEngine.Random.Range(0, this.slamClip.Length)], 0);
		base.transform.position = this.playerTransform.position;
		yield return new WaitForSeconds(this.slamSpeed);
		if (Vector3.Distance(this.playerTransform.position, base.transform.position) < 0.5f)
		{
			this.rotationScript.Gameover(this.playerTransform, true, 1);
		}
		for (int i = 0; i < this.shock.Length; i++)
		{
			this.shock[i].Emit(base.transform.position, false, this.playerTransform.position);
		}
		this.slamCount--;
		if (this.slamCount > 0)
		{
			yield return new WaitForSeconds(0.2f);
			base.StartCoroutine(this.Slam(0));
		}
		else
		{
			base.StartCoroutine(this.PostAction(1.3f));
		}
		yield break;
	}

	// Token: 0x060001E8 RID: 488 RVA: 0x00003293 File Offset: 0x00001493
	private IEnumerator SuperSlam()
	{
		this.specialReady = false;
		this.animator.SetTrigger("isRaging");
		ManagerScript.instance.PlayShortSound(this.rageClip, 0);
		yield return new WaitForSeconds(0.6f);
		if (Vector3.Distance(this.playerTransform.position, base.transform.position) < 0.5f)
		{
			this.rotationScript.Gameover(this.playerTransform, true, 1);
		}
		for (int i = 0; i < this.shock.Length; i++)
		{
			this.shock[i].Emit(base.transform.position, false, this.playerTransform.position);
		}
		yield return new WaitForSeconds(0.4f);
		for (int j = 0; j < this.shockAlt.Length; j++)
		{
			this.shockAlt[j].Emit(base.transform.position, false, this.playerTransform.position);
		}
		base.StartCoroutine(this.PostAction(1.3f));
		int num;
		for (int x = 0; x < 2; x = num + 1)
		{
			ManagerScript.instance.PlayShortSound(this.thunderClip[UnityEngine.Random.Range(0, this.thunderClip.Length)], 0);
			for (int k = 0; k < this.thunder.Length; k++)
			{
				this.thunder[k].Bolt();
			}
			yield return new WaitForSeconds(1.6f);
			num = x;
		}
		yield break;
	}

	// Token: 0x060001E9 RID: 489 RVA: 0x000020D2 File Offset: 0x000002D2
	protected override void OutOfRangeAction()
	{
	}

	// Token: 0x060001EA RID: 490 RVA: 0x000032A2 File Offset: 0x000014A2
	private IEnumerator Thunder()
	{
		base.transform.position = this.playerTransform.position;
		this.animator.SetTrigger("isCasting");
		yield return new WaitForSeconds(0.8f);
		for (int i = 0; i < this.thunder.Length; i++)
		{
			this.thunder[i].Bolt();
		}
		base.StartCoroutine(this.PostAction(1.3f));
		int num;
		for (int x = 0; x < 2; x = num + 1)
		{
			yield return new WaitForSeconds(1.6f);
			for (int j = 0; j < this.thunder.Length; j++)
			{
				this.thunder[j].Bolt();
			}
			num = x;
		}
		yield break;
	}

	// Token: 0x060001EB RID: 491 RVA: 0x000086A0 File Offset: 0x000068A0
	public override void WipeEvent()
	{
		for (int i = 0; i < this.shock.Length; i++)
		{
			this.shock[i].Destruction();
		}
		for (int j = 0; j < this.shockAlt.Length; j++)
		{
			this.shockAlt[j].Destruction();
		}
		for (int k = 0; k < this.thunder.Length; k++)
		{
			UnityEngine.Object.Destroy(this.thunder[k].gameObject);
		}
		base.WipeEvent();
	}

	// Token: 0x0400019E RID: 414
	public int lvl;

	// Token: 0x0400019F RID: 415
	public ShockwaveScript[] shock;

	// Token: 0x040001A0 RID: 416
	public ShockwaveScript[] shockAlt;

	// Token: 0x040001A1 RID: 417
	public ThunderScript[] thunder;

	// Token: 0x040001A2 RID: 418
	private bool specialReady;

	// Token: 0x040001A3 RID: 419
	private int slamCount;

	// Token: 0x040001A4 RID: 420
	public float slamSpeed = 0.6f;

	// Token: 0x040001A5 RID: 421
	public AudioClip[] dashClip;

	// Token: 0x040001A6 RID: 422
	public AudioClip[] slamClip;

	// Token: 0x040001A7 RID: 423
	public AudioClip rageClip;

	// Token: 0x040001A8 RID: 424
	public AudioClip[] thunderClip;
}
