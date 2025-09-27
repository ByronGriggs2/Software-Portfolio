using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// Token: 0x02000042 RID: 66
public class CutwireAiScript : GhostAiScript
{
	// Token: 0x0600014F RID: 335 RVA: 0x00002D2E File Offset: 0x00000F2E
	protected override void InRangeAction()
	{
		if (this.attackRange < 3f)
		{
			base.StartCoroutine(this.CutterThrow());
		}
	}

	// Token: 0x06000150 RID: 336 RVA: 0x00002D4A File Offset: 0x00000F4A
	public IEnumerator CutterThrow()
	{
		this.animator.SetTrigger("isOut");
		if (this.spawning)
		{
			ManagerScript.instance.PlayShortSound(this.otherSounds[2], 0);
			this.spawning = false;
		}
		else
		{
			ManagerScript.instance.PlayShortSound(this.otherSounds[1], 0);
		}
		this.attackRange *= 3f;
		yield return new WaitForSeconds(0.6f);
		int cutterCombo = 0;
		while (base.playerInRange() && cutterCombo < 2)
		{
			this.animator.SetTrigger("isAttacking");
			ManagerScript.instance.PlayShortSound(this.soundArray[UnityEngine.Random.Range(0, this.soundArray.Length)], 0);
			Vector2 throwDir = (this.aim - this.rb.position).normalized;
			float num = Mathf.Atan2(throwDir.y, throwDir.x) * 57.29578f;
			if (num < 0f)
			{
				num += 360f;
			}
			this.holoTransform.eulerAngles = new Vector3(0f, 0f, num);
			this.holoAnimator.SetTrigger("cast");
			yield return new WaitForSeconds(0.3f);
			this.rb.AddForce(throwDir * 400f);
			base.StartCoroutine(base.TripleThrow(throwDir, 1f));
			yield return new WaitForSeconds(1.2f);
			int num2 = cutterCombo;
			cutterCombo = num2 + 1;
			throwDir = default(Vector2);
		}
		this.animator.SetTrigger("isAlive");
		ManagerScript.instance.PlayShortSound(this.otherSounds[0], 0);
		GameObject gameObject = UnityEngine.Object.Instantiate<GameObject>(this.trap, base.transform.position, Quaternion.identity);
		this.traps.Add(gameObject);
		TrapScript component = gameObject.GetComponent<TrapScript>();
		component.rotationScript = this.rotationScript;
		component.randomSprite = this.trapSprite[this.trapIndex];
		this.trapIndex++;
		if (this.trapIndex > 2)
		{
			this.trapIndex = 0;
		}
		this.attackRange /= 3f;
		base.StartCoroutine(this.PostAction(1.3f));
		this.rb.AddForce((this.aim - this.rb.position).normalized * this.dashPower);
		yield break;
	}

	// Token: 0x06000151 RID: 337 RVA: 0x00006BAC File Offset: 0x00004DAC
	protected override void SingleThrow(Vector2 dir)
	{
		this.projectileArray[this.throwIndex].Throw(this.rb.position, dir);
		this.throwIndex++;
		if (this.throwIndex == this.projectileArray.Length)
		{
			this.throwIndex = 0;
		}
	}

	// Token: 0x06000152 RID: 338 RVA: 0x00006C04 File Offset: 0x00004E04
	public override void WipeEvent()
	{
		for (int i = 0; i < this.projectileArray.Length; i++)
		{
			this.projectileArray[i].gameObject.SetActive(false);
		}
		for (int j = 0; j < this.traps.Count; j++)
		{
			UnityEngine.Object.Destroy(this.traps[j]);
		}
		base.WipeEvent();
	}

	// Token: 0x0400011F RID: 287
	public ThrowableScript[] projectileArray;

	// Token: 0x04000120 RID: 288
	public CutterLineScript[] cutterLines;

	// Token: 0x04000121 RID: 289
	public GameObject trap;

	// Token: 0x04000122 RID: 290
	public Sprite[] trapSprite;

	// Token: 0x04000123 RID: 291
	public Transform holoTransform;

	// Token: 0x04000124 RID: 292
	public Animator holoAnimator;

	// Token: 0x04000125 RID: 293
	public AudioClip[] soundArray;

	// Token: 0x04000126 RID: 294
	public AudioClip[] otherSounds;

	// Token: 0x04000127 RID: 295
	private List<GameObject> traps = new List<GameObject>();

	// Token: 0x04000128 RID: 296
	private int trapIndex;

	// Token: 0x04000129 RID: 297
	private bool spawning = true;
}
