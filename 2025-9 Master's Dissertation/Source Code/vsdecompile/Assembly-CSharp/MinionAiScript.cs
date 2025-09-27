using System;
using System.Collections;
using UnityEngine;

// Token: 0x02000050 RID: 80
public class MinionAiScript : AiScript, IVulnerable
{
    // Token: 0x060001A2 RID: 418 RVA: 0x0000303E File Offset: 0x0000123E
    protected override IEnumerator ChaseRoutine()
	{
		while (!this.busy)
		{
			yield return base.StartCoroutine(base.ChaseRoutine());
		}
		yield break;
	}

	// Token: 0x060001A3 RID: 419 RVA: 0x0000304D File Offset: 0x0000124D
	protected override void InRangeAction()
	{
		base.StartCoroutine(this.Dash());
	}

	// Token: 0x060001A4 RID: 420 RVA: 0x0000305C File Offset: 0x0000125C
	private IEnumerator Dash()
	{
		this.immortal = false;
		this.animator.SetTrigger("isAttacking");
		ManagerScript.instance.PlayShortSound(this.soundArray[UnityEngine.Random.Range(0, this.soundArray.Length)], 0);
		yield return new WaitForSeconds(0.3f);
		Vector2 normalized = (this.aim - this.rb.position).normalized;
		this.rb.AddForce(normalized * this.dashPower);
		this.deathCollider.enabled = true;
		base.StartCoroutine(this.PostAction(0.9f));
		yield return new WaitForSeconds(0.3f);
		this.deathCollider.enabled = false;
		yield break;
	}

	// Token: 0x060001A5 RID: 421 RVA: 0x00007A8C File Offset: 0x00005C8C
	protected override void FixedUpdate()
	{
		base.FixedUpdate();
		Collider2D collider2D = Physics2D.OverlapCircle(base.transform.position, 0.1f);
		if (collider2D != null)
		{
			Vector2 a = base.transform.position - collider2D.transform.position;
			this.rb.AddForce(a * this.moveSpeed * 0.8f);
		}
	}

	// Token: 0x060001A6 RID: 422 RVA: 0x00007B08 File Offset: 0x00005D08
	private void OnCollisionEnter2D(Collision2D collision)
	{
		if (collision.relativeVelocity.magnitude > 2f && !this.immortal)
		{
			this.immortal = true;
			this.Death();
		}
	}

	// Token: 0x060001A7 RID: 423 RVA: 0x0000306B File Offset: 0x0000126B
	public void Attacked(Vector2 dir)
	{
		if (this.hitable)
		{
			this.Death();
		}
	}

	// Token: 0x060001A8 RID: 424 RVA: 0x00007B40 File Offset: 0x00005D40
	public void Death()
	{
		if (base.gameObject.activeSelf)
		{
			this.hitable = false;
			ManagerScript.instance.PlayShortCommonSound(this.deathSoundArray[UnityEngine.Random.Range(0, this.deathSoundArray.Length)]);
			UnityEngine.Object.Instantiate<GameObject>(this.deathVfx, base.transform.position, base.transform.rotation);
			this.available = 1;
			base.gameObject.SetActive(false);
		}
	}

	// Token: 0x060001A9 RID: 425 RVA: 0x00007BB8 File Offset: 0x00005DB8
	public override void Spawn()
	{
		this.available = 0;
		this.hitbox.target = this;
		if (this.movingSummoner == null)
		{
			this.hitable = true;
			base.transform.position = this.origin;
			if (this.origin.x < 0f)
			{
				this.spriteTransform.localScale = new Vector3(-1f, 1f, 1f);
			}
			else
			{
				this.spriteTransform.localScale = new Vector3(1f, 1f, 1f);
			}
			this.deathCollider.enabled = false;
			base.Spawn();
			return;
		}
		this.immortal = true;
		base.transform.position = this.movingSummoner.gameObject.transform.position;
		Vector2 vector = UnityEngine.Random.insideUnitCircle.normalized;
		if (Physics2D.Raycast(base.transform.position, vector, 0.5f, 256).collider != null)
		{
			vector = -vector;
		}
		this.deathCollider.enabled = false;
		base.Spawn();
		this.rb.AddForce(vector * UnityEngine.Random.Range(600f, 800f));
		if (this.gameObject.tag == "Deadly")
			AwariaInterface.MainInterface.Instance.addHazard(this);
		else
			Debug.Log("Not deadly");
	}

	// Token: 0x04000167 RID: 359
	public GameObject deathVfx;

	// Token: 0x04000168 RID: 360
	public BigStrigaScript summoner;

	// Token: 0x04000169 RID: 361
	public StrigaAiScript movingSummoner;

	// Token: 0x0400016A RID: 362
	public Vector2 origin;

	// Token: 0x0400016B RID: 363
	public int available = 1;

	// Token: 0x0400016C RID: 364
	public HitboxScript hitbox;

	// Token: 0x0400016D RID: 365
	public AudioClip[] soundArray;

	// Token: 0x0400016E RID: 366
	public AudioClip[] deathSoundArray;

	// Token: 0x0400016F RID: 367
	private bool immortal = true;
}
