using System;
using System.Collections;
using UnityEngine;

// Token: 0x02000044 RID: 68
public class DogoAiScript : GhostAiScript, IVulnerable
{
	// Token: 0x0600015A RID: 346 RVA: 0x00002D8A File Offset: 0x00000F8A
	protected override void InRangeAction()
	{
		if (!this.sliding)
		{
			base.StartCoroutine(this.Slide(0f));
		}
	}

	// Token: 0x0600015B RID: 347 RVA: 0x00002DA6 File Offset: 0x00000FA6
	protected override IEnumerator PostAction(float actionLenght = 1.3f)
	{
		yield return new WaitForSeconds(actionLenght);
		this.busy = false;
		this.hitable = true;
		this.sliding = false;
		if (this.mandatorySlide)
		{
			this.mandatorySlide = false;
			base.StartCoroutine(this.Slide(base.transform.position.x));
		}
		else if (!base.playerInRange())
		{
			if ((double)base.transform.position.x < -6.5 || (double)base.transform.position.x > 6.5 || (double)base.transform.position.y > -0.4)
			{
				base.StartCoroutine(this.Slide(base.transform.position.x));
			}
			else
			{
				this.animator.SetTrigger("isMoving");
				base.StartCoroutine(this.ChaseRoutine());
			}
		}
		yield break;
	}

	// Token: 0x0600015C RID: 348 RVA: 0x00006F68 File Offset: 0x00005168
	protected override void OutOfRangeAction()
	{
		if (this.playerTransform.position.x > 4f && base.transform.position.x > 4f)
		{
			base.StartCoroutine(this.Slide(base.transform.position.x));
			return;
		}
		if (this.playerTransform.position.x < -4f && base.transform.position.x < -4f)
		{
			base.StartCoroutine(this.Slide(base.transform.position.x));
		}
	}

	// Token: 0x0600015D RID: 349 RVA: 0x00002DBC File Offset: 0x00000FBC
	private IEnumerator Dash()
	{
		this.animator.SetTrigger("isAttacking");
		ManagerScript.instance.PlayShortSound(this.soundArrayStart[UnityEngine.Random.Range(0, this.soundArrayStart.Length)], 0);
		yield return new WaitForSeconds(0.3f);
		if (Vector3.Distance(this.playerTransform.position, base.transform.position) < 1.2f)
		{
			this.rotationScript.Gameover(this.playerTransform, true, 1);
		}
		Vector3 origin = base.transform.position;
		this.dogDirection = (this.aim - this.rb.position).normalized;
		this.rb.AddForce(this.dogDirection * this.dashPower);
		this.deathCollider.enabled = true;
		base.StartCoroutine(this.PostAction(1.4f));
		yield return new WaitForSeconds(0.4f);
		this.deathCollider.enabled = false;
		if (origin == base.transform.position)
		{
			this.mandatorySlide = true;
		}
		yield break;
	}

	// Token: 0x0600015E RID: 350 RVA: 0x00002DCB File Offset: 0x00000FCB
	public void Landing()
	{
		ManagerScript.instance.PlayShortSound(this.soundArrayLand[UnityEngine.Random.Range(0, this.soundArrayLand.Length)], 0);
	}

	// Token: 0x0600015F RID: 351 RVA: 0x00002DEE File Offset: 0x00000FEE
	public void Shake()
	{
		ManagerScript.instance.PlayShortSound(this.shakeClip, 0);
	}

	// Token: 0x06000160 RID: 352 RVA: 0x0000700C File Offset: 0x0000520C
	private void OnCollisionEnter2D(Collision2D collision)
	{
		if (collision.relativeVelocity.magnitude > 10f && !this.sliding && this.hitable)
		{
			this.rotationScript.camScript.Shakedown(0.1f, 0.1f);
			this.sliding = true;
			this.animator.SetTrigger("isHurt");
			ManagerScript.instance.PlayShortSound(this.soundArraySlam[UnityEngine.Random.Range(0, this.soundArraySlam.Length)], 0);
			this.deathCollider.enabled = false;
			this.rb.AddForce(-this.dogDirection * (this.dashPower / 15f));
		}
	}

	// Token: 0x06000161 RID: 353 RVA: 0x00002E02 File Offset: 0x00001002
	private IEnumerator Slide(float target = 0f)
	{
		this.dogDirection = (this.aim - this.rb.position).normalized;
		if (target != 0f)
		{
			base.PreAction();
		}
		else if (this.dogDirection.y > 0f && ((double)base.transform.position.x < -6.5 || (double)base.transform.position.x > 6.5))
		{
			target = base.transform.position.x;
		}
		this.sliding = true;
		this.animator.SetTrigger("isSliding");
		ManagerScript.instance.PlayShortSound(this.soundArraySlide[UnityEngine.Random.Range(0, this.soundArraySlide.Length)], 0);
		if (target < 0f)
		{
			this.dogDirection = (this.slideRight - this.rb.position).normalized;
		}
		else if (target > 0f)
		{
			this.dogDirection = (this.slideLeft - this.rb.position).normalized;
		}
		this.rb.AddForce(this.dogDirection * (this.dashPower / 3f));
		yield return new WaitForSeconds(0.3f);
		if (base.playerInRange())
		{
			base.StartCoroutine(this.Dash());
		}
		else
		{
			base.StartCoroutine(this.PostAction(0.5f));
		}
		this.sliding = false;
		yield break;
	}

	// Token: 0x06000162 RID: 354 RVA: 0x00002E18 File Offset: 0x00001018
	public void Waved(Vector2 dir)
	{
		this.nova.Wipe();
		if (!this.hitable)
		{
			return;
		}
		this.pushforce = 100;
		this.Attacked(-dir);
	}

	// Token: 0x06000163 RID: 355 RVA: 0x00002E42 File Offset: 0x00001042
	public void Attacked(Vector2 dir)
	{
		if (!this.hitable)
		{
			return;
		}
		this.busy = true;
		this.hitable = false;
		base.StopAllCoroutines();
		this.deathCollider.enabled = false;
		base.StartCoroutine(this.Death(dir));
	}

	// Token: 0x06000164 RID: 356 RVA: 0x00002E7B File Offset: 0x0000107B
	private IEnumerator Death(Vector2 dir)
	{
		this.animator.SetTrigger("isPushed");
		ManagerScript.instance.PlayShortSound(this.soundArrayDeath[UnityEngine.Random.Range(0, this.soundArrayDeath.Length)], 0);
		UnityEngine.Object.Instantiate<GameObject>(this.dmgVfx, base.transform.position, base.transform.rotation);
		this.rb.AddForce(dir * (float)this.pushforce);
		yield return new WaitForSeconds(0.9f);
		UnityEngine.Object.Instantiate<GameObject>(this.deathVfx, base.transform.position, base.transform.rotation);
		this.nova.Spread(base.transform.position);
		yield return new WaitForSeconds(0.4f);
		base.gameObject.SetActive(false);
		yield break;
	}

	// Token: 0x06000165 RID: 357 RVA: 0x00002E91 File Offset: 0x00001091
	public override void Spawn()
	{
		this.hitbox.target = this;
		base.Spawn();
		base.StartCoroutine(this.HitableSet());
	}

	// Token: 0x06000166 RID: 358 RVA: 0x00002EB2 File Offset: 0x000010B2
	private IEnumerator HitableSet()
	{
		yield return new WaitForSeconds(0.2f);
		this.hitable = true;
		yield break;
	}

	// Token: 0x06000167 RID: 359 RVA: 0x00002EC1 File Offset: 0x000010C1
	public override void WipeEvent()
	{
		this.nova.gameObject.SetActive(false);
		base.gameObject.SetActive(false);
	}

	// Token: 0x0400012F RID: 303
	public Vector2 slideLeft;

	// Token: 0x04000130 RID: 304
	public Vector2 slideRight;

	// Token: 0x04000131 RID: 305
	public BulletSpreadScript nova;

	// Token: 0x04000132 RID: 306
	private bool sliding;

	// Token: 0x04000133 RID: 307
	private Vector2 dogDirection;

	// Token: 0x04000134 RID: 308
	private bool mandatorySlide;

	// Token: 0x04000135 RID: 309
	public HitboxScript hitbox;

	// Token: 0x04000136 RID: 310
	public GameObject deathVfx;

	// Token: 0x04000137 RID: 311
	public GameObject dmgVfx;

	// Token: 0x04000138 RID: 312
	public AudioClip[] soundArrayStart;

	// Token: 0x04000139 RID: 313
	public AudioClip[] soundArrayLand;

	// Token: 0x0400013A RID: 314
	public AudioClip[] soundArraySlam;

	// Token: 0x0400013B RID: 315
	public AudioClip[] soundArraySlide;

	// Token: 0x0400013C RID: 316
	public AudioClip[] soundArrayDeath;

	// Token: 0x0400013D RID: 317
	public AudioClip shakeClip;

	// Token: 0x0400013E RID: 318
	private int pushforce = 1000;
}
