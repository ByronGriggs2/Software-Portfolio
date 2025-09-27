using System;
using System.Collections;
using UnityEngine;

// Token: 0x02000053 RID: 83
public class NikitaAiScript : GhostAiScript
{
	// Token: 0x060001B8 RID: 440 RVA: 0x000030BF File Offset: 0x000012BF
	protected override void InRangeAction()
	{
		if (this.waveReady)
		{
			base.StartCoroutine(this.FlameWave());
			return;
		}
		base.StartCoroutine(this.Fireball());
	}

	// Token: 0x060001B9 RID: 441 RVA: 0x000030E4 File Offset: 0x000012E4
	public IEnumerator FlameWave()
	{
		this.waveReady = false;
		this.animator.SetTrigger("isBlasting");
		ManagerScript.instance.PlayShortSound(this.waveClip[UnityEngine.Random.Range(0, this.waveClip.Length)], 0);
		this.holoAnimator.SetTrigger("fire");
		yield return new WaitForSeconds(0.8f);
		if (Vector3.Distance(this.playerTransform.position, base.transform.position) < 0.5f)
		{
			this.rotationScript.Gameover(this.playerTransform, true, 1);
		}
		this.spreadPoolScript.Ignite(base.transform.position);
		base.StartCoroutine(this.PostAction(1.3f));
		yield break;
	}

	// Token: 0x060001BA RID: 442 RVA: 0x000030F3 File Offset: 0x000012F3
	public IEnumerator Fireball()
	{
		this.waveReady = true;
		this.animator.SetTrigger("isAttacking");
		ManagerScript.instance.PlayShortSound(this.birdClip[UnityEngine.Random.Range(0, this.birdClip.Length)], 0);
		Vector2 direction = (this.aim - this.rb.position).normalized;
		float num = Mathf.Atan2(direction.y, direction.x) * 57.29578f;
		if (num < 0f)
		{
			num += 360f;
		}
		this.holoTransform.eulerAngles = new Vector3(0f, 0f, num);
		this.holoAnimator.SetTrigger("bird");
		yield return new WaitForSeconds(0.4f);
		this.rb.AddForce(direction * this.dashPower);
		this.deathCollider.enabled = true;
		this.fireball.Throw(this.rb.position, direction);
		base.StartCoroutine(this.PostAction(1.3f));
		yield return new WaitForSeconds(0.15f);
		this.deathCollider.enabled = false;
		yield break;
	}

	// Token: 0x060001BB RID: 443 RVA: 0x00003102 File Offset: 0x00001302
	public override void WipeEvent()
	{
		this.holoAnimator.SetTrigger("wipe");
		this.fireball.Destruction();
		base.WipeEvent();
	}

	// Token: 0x04000176 RID: 374
	public BulletSpreadPoolScript spreadPoolScript;

	// Token: 0x04000177 RID: 375
	public FireballScript fireball;

	// Token: 0x04000178 RID: 376
	public bool waveReady;

	// Token: 0x04000179 RID: 377
	public Animator holoAnimator;

	// Token: 0x0400017A RID: 378
	public Transform holoTransform;

	// Token: 0x0400017B RID: 379
	public AudioClip[] waveClip;

	// Token: 0x0400017C RID: 380
	public AudioClip[] birdClip;
}
