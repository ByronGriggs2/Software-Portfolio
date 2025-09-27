using System;
using System.Collections;
using UnityEngine;

// Token: 0x0200004B RID: 75
public abstract class GhostAiScript : AiScript
{
	public Vector3 byronGetGhostPosition()
	{
		return this.rb.transform.position;
	}
	protected override void Start()
	{
		base.Start();
		AwariaInterface.MainInterface.Instance.addGhostReference(this);
	}
	// Token: 0x0600018C RID: 396
	protected override IEnumerator ChaseRoutine()
	{
		while (!this.busy)
		{
			yield return base.StartCoroutine(base.ChaseRoutine());
			if (this.busy)
			{
				yield break;
			}
			this.generalCooldown -= this.cooldownTic;
			if (this.generalCooldown <= 0f)
			{
				this.generalCooldown = 2f;
				this.OutOfRangeAction();
			}
		}
		yield break;
	}

	// Token: 0x0600018D RID: 397
	protected virtual void OutOfRangeAction()
	{
	}

	// Token: 0x0600018E RID: 398
	protected IEnumerator TripleThrow(Vector2 dir, float multiplier = 1f)
	{
		this.SingleThrow(dir);
		float angle = Mathf.Atan2(dir.y, dir.x) * 57.29578f;
		if (angle < 0f)
		{
			angle += 360f;
		}
		angle += 10f * multiplier;
		float f = angle * 0.017453292f;
		dir = new Vector2(Mathf.Cos(f), Mathf.Sin(f));
		yield return new WaitForSeconds(0.05f);
		this.SingleThrow(dir);
		angle -= 20f * multiplier;
		f = angle * 0.017453292f;
		dir = new Vector2(Mathf.Cos(f), Mathf.Sin(f));
		this.SingleThrow(dir);
		yield break;
	}

	// Token: 0x0600018F RID: 399
	protected virtual void SingleThrow(Vector2 dir)
	{
	}

	// Token: 0x06000190 RID: 400
	public virtual void WipeEvent()
	{
		this.busy = true;
		this.animator.SetTrigger("isExorcised");
		base.StopAllCoroutines();
	}

	// Token: 0x04000159 RID: 345
	public RotationScript rotationScript;

	// Token: 0x0400015A RID: 346
	public float generalCooldown = 2f;

	// Token: 0x0400015B RID: 347
	public float cooldownTic = 0.2f;

	// Token: 0x0400015C RID: 348
	protected int throwIndex;
}
