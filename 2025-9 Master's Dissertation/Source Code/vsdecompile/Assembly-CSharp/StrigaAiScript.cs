using System;
using System.Collections;
using UnityEngine;

// Token: 0x02000056 RID: 86
public class StrigaAiScript : GhostAiScript
{
	// Token: 0x060001C9 RID: 457 RVA: 0x00003153 File Offset: 0x00001353
	protected override void InRangeAction()
	{
		if (this.specialReady)
		{
			base.StartCoroutine(this.Shatter());
			return;
		}
		base.StartCoroutine(this.Melee());
	}

	// Token: 0x060001CA RID: 458 RVA: 0x00003178 File Offset: 0x00001378
	private IEnumerator Melee()
	{
		this.specialReady = true;
		this.animator.SetTrigger("isAttacking");
		ManagerScript.instance.PlayShortSound(this.throwClip[UnityEngine.Random.Range(0, this.throwClip.Length)], 0);
		yield return new WaitForSeconds(0.2f);
		Vector2 normalized = (this.aim - this.rb.position).normalized;
		base.StartCoroutine(base.TripleThrow(normalized, 0.5f));
		this.rb.AddForce(normalized * this.dashPower);
		this.deathCollider.enabled = true;
		base.StartCoroutine(this.PostAction(0.7f));
		yield return new WaitForSeconds(0.15f);
		this.deathCollider.enabled = false;
		yield break;
	}

	// Token: 0x060001CB RID: 459 RVA: 0x00008100 File Offset: 0x00006300
	protected override void SingleThrow(Vector2 dir)
	{
		this.projectileArrayB[this.throwIndex].Throw(this.rb.position, dir);
		this.throwIndex++;
		if (this.throwIndex == this.projectileArrayB.Length)
		{
			this.throwIndex = 0;
		}
	}

	// Token: 0x060001CC RID: 460 RVA: 0x00003187 File Offset: 0x00001387
	private IEnumerator Shatter()
	{
		this.specialReady = false;
		this.animator.SetTrigger("isCasting");
		ManagerScript.instance.PlayShortSound(this.cannonClip[UnityEngine.Random.Range(0, this.cannonClip.Length)], 0);
		yield return new WaitForSeconds(0.4f);
		this.scrapBarrage[0].Aim(this.playerTransform.position);
		yield return new WaitForSeconds(0.2f);
		this.scrapBarrage[1].Aim(this.playerTransform.position);
		base.StartCoroutine(this.PostAction(0.7f));
		yield return new WaitForSeconds(0.1f);
		ManagerScript.instance.PlayShortSound(this.markClip, 0);
		yield return new WaitForSeconds(0.3f);
		ManagerScript.instance.PlayShortSound(this.markClip, 0);
		yield break;
	}

	// Token: 0x060001CD RID: 461 RVA: 0x00003196 File Offset: 0x00001396
	protected override void OutOfRangeAction()
	{
		base.PreAction();
		base.StartCoroutine(this.Teleport());
	}

	// Token: 0x060001CE RID: 462 RVA: 0x000031AB File Offset: 0x000013AB
	private IEnumerator Teleport()
	{
		this.animator.SetTrigger("isWarping");
		ManagerScript.instance.PlayShortSound(this.teleportClip, 0);
		this.phaser.Phase(this.playerTransform.position, base.transform.position, this.spriteTransform.localScale);
		base.transform.position = this.playerTransform.position;
		for (int i = 0; i < this.rotoScrap.Length; i++)
		{
			this.rotoScrap[i].ReCast(base.transform.position);
		}
		yield return new WaitForSeconds(0.6f);
		for (int j = 0; j < this.rotoScrap.Length; j++)
		{
			this.rotoScrap[j].gameObject.SetActive(false);
		}
		this.DistanceCheck(base.transform);
		this.frostNova.Spread(base.transform.position);
		base.StartCoroutine(this.PostAction(1.3f));
		int num = 0;
		for (int k = 0; k < this.minions.Length; k++)
		{
			num += this.minions[k].available;
		}
		if (num == this.minions.Length)
		{
			ManagerScript.instance.PlayShortSound(this.dogSummon, 0);
			for (int l = 0; l < this.minions.Length; l++)
			{
				this.minions[l].Spawn();
			}
		}
		yield break;
	}

	// Token: 0x060001CF RID: 463 RVA: 0x000031BA File Offset: 0x000013BA
	public void DistanceCheck(Transform killerTransform)
	{
		if (Vector3.Distance(this.playerTransform.position, killerTransform.position) < 0.5f)
		{
			this.rotationScript.Gameover(this.playerTransform, true, 1);
		}
	}

	// Token: 0x060001D0 RID: 464 RVA: 0x00008158 File Offset: 0x00006358
	public void SpawnIce(Vector2 target)
	{
		this.iceSpikeArray[this.spikeIndex].Spawn(target, this.spriteArray[UnityEngine.Random.Range(0, 16)]);
		this.spikeIndex++;
		if (this.spikeIndex == this.iceSpikeArray.Length)
		{
			this.spikeIndex = 0;
		}
	}

	// Token: 0x060001D1 RID: 465 RVA: 0x000081AC File Offset: 0x000063AC
	public override void WipeEvent()
	{
		for (int i = 0; i < this.projectileArray.Length; i++)
		{
			this.projectileArray[i].gameObject.SetActive(false);
		}
		for (int j = 0; j < this.scrapBarrage.Length; j++)
		{
			this.scrapBarrage[j].Destruction(true);
		}
		for (int k = 0; k < this.rotoScrap.Length; k++)
		{
			this.rotoScrap[k].gameObject.SetActive(false);
		}
		for (int l = 0; l < this.minions.Length; l++)
		{
			this.minions[l].gameObject.SetActive(false);
		}
		this.frostNova.gameObject.SetActive(false);
		base.WipeEvent();
	}

	// Token: 0x04000184 RID: 388
	public IcewaveScript[] projectileArray;

	// Token: 0x04000185 RID: 389
	public ThrowableScript[] projectileArrayB;

	// Token: 0x04000186 RID: 390
	public BulletSpreadScript frostNova;

	// Token: 0x04000187 RID: 391
	public ScrapBarrageScript[] scrapBarrage;

	// Token: 0x04000188 RID: 392
	public IceSpikeScript[] iceSpikeArray;

	// Token: 0x04000189 RID: 393
	public Sprite[] spriteArray;

	// Token: 0x0400018A RID: 394
	public Sprite[] shardSpriteArray;

	// Token: 0x0400018B RID: 395
	public RotoScrapScript[] rotoScrap;

	// Token: 0x0400018C RID: 396
	public PhaseScript phaser;

	// Token: 0x0400018D RID: 397
	public MinionAiScript[] minions;

	// Token: 0x0400018E RID: 398
	public AudioClip[] cannonClip;

	// Token: 0x0400018F RID: 399
	public AudioClip[] throwClip;

	// Token: 0x04000190 RID: 400
	public AudioClip markClip;

	// Token: 0x04000191 RID: 401
	public AudioClip teleportClip;

	// Token: 0x04000192 RID: 402
	public AudioClip dogSummon;

	// Token: 0x04000193 RID: 403
	private int spikeIndex;

	// Token: 0x04000194 RID: 404
	private bool specialReady;
}
