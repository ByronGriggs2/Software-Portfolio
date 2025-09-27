using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// Token: 0x02000038 RID: 56
public class BigStrigaScript : MonoBehaviour
{
	// Token: 0x06000107 RID: 263 RVA: 0x00005E84 File Offset: 0x00004084
	public void BossStart()
	{
		this.animator.enabled = true;
		if (this.wendigoCountdown == 0)
		{
			ManagerScript.instance.PlayShortSound(this.phaseClip[0], 0);
			base.StartCoroutine(this.FirstBossRotation());
			return;
		}
		ManagerScript.instance.PlayShortSound(this.bigClips[0], 0);
	}

	// Token: 0x06000108 RID: 264 RVA: 0x00002AB9 File Offset: 0x00000CB9
	public IEnumerator FirstBossRotation()
	{
		yield return new WaitForSeconds(1f);
		if (this.firstRotation)
		{
			this.firstRotation = false;
		}
		else
		{
			ManagerScript.instance.PlayShortSound(this.phaseClip[2], 0);
		}
		if (this.summonIndex != 2)
		{
			this.DogCheck();
		}
		if (this.summonIndex == 0)
		{
			yield return base.StartCoroutine(this.SummonDogs());
		}
		if (this.summonIndex == 0 || this.summonIndex == 2)
		{
			yield return base.StartCoroutine(this.SummonMarch());
		}
		if (this.summonIndex == 1 || this.summonIndex == 2)
		{
			yield return base.StartCoroutine(this.SummonSkull());
		}
		if (this.summonIndex == 1)
		{
			yield return base.StartCoroutine(this.SummonDogs());
		}
		this.summonIndex++;
		if (this.summonIndex == 3)
		{
			this.summonIndex = 0;
		}
		ManagerScript.instance.PlayShortSound(this.phaseClip[1], 0);
		yield return new WaitForSeconds(3f);
		base.StartCoroutine(this.FirstBossRotation());
		yield break;
	}

	// Token: 0x06000109 RID: 265 RVA: 0x00005EDC File Offset: 0x000040DC
	public void AniBossRotation()
	{
		if (this.playerTransform.position.x < 4f && this.playerTransform.position.x > -4f)
		{
			this.animator.SetTrigger("smash");
			this.currentBigAudio = ManagerScript.instance.PlayShortSound(this.bigClips[2], 0);
			if (this.playerTransform.position.x > 2f)
			{
				base.transform.position = new Vector2(2f, base.transform.position.y);
				return;
			}
			if (this.playerTransform.position.x < -2f)
			{
				base.transform.position = new Vector2(-2f, base.transform.position.y);
				return;
			}
		}
		else
		{
			base.StartCoroutine(this.PlayerBombard());
		}
	}

	// Token: 0x0600010A RID: 266 RVA: 0x00002AC8 File Offset: 0x00000CC8
	public void AniRecovery()
	{
		base.transform.position = new Vector2(0f, base.transform.position.y);
	}

	// Token: 0x0600010B RID: 267 RVA: 0x00005FD8 File Offset: 0x000041D8
	public void AniDoubleSummon()
	{
		if (this.summonIndex != 2)
		{
			this.DogCheck();
		}
		this.currentBigAudio = ManagerScript.instance.PlayShortSound(this.bigClips[1], 0);
		if (this.summonIndex == 0)
		{
			this.DogSummoner();
		}
		if (this.summonIndex == 0 || this.summonIndex == 2)
		{
			this.SummonMarchDuo();
		}
		if (this.summonIndex == 1 || this.summonIndex == 2)
		{
			this.SummonSkullDuo();
		}
		if (this.summonIndex == 1)
		{
			this.DogSummoner();
		}
		this.summonIndex++;
		if (this.summonIndex == 3)
		{
			this.summonIndex = 0;
		}
	}

	// Token: 0x0600010C RID: 268 RVA: 0x00006078 File Offset: 0x00004278
	public void DogCheck()
	{
		int num = 0;
		for (int i = 0; i < this.minions.Length; i++)
		{
			num += this.minions[i].available;
		}
		if (num != this.minions.Length)
		{
			this.summonIndex = 2;
		}
	}

	// Token: 0x0600010D RID: 269 RVA: 0x00002AF4 File Offset: 0x00000CF4
	public IEnumerator RotationCD(string trigger = "")
	{
		this.animator.SetTrigger(trigger);
		yield return new WaitForSeconds(0.8f);
		yield break;
	}

	// Token: 0x0600010E RID: 270 RVA: 0x00002B0A File Offset: 0x00000D0A
	public IEnumerator SummonDogs()
	{
		if (this.wendigoCountdown == 0)
		{
			ManagerScript.instance.PlayShortSound(this.dogClip[0], 0);
		}
		else
		{
			ManagerScript.instance.PlayShortSound(this.dogClip[2], 0);
		}
		yield return base.StartCoroutine(this.RotationCD("dogs"));
		this.DogSummoner();
		yield return new WaitForSeconds(this.cooldown);
		yield break;
	}

	// Token: 0x0600010F RID: 271 RVA: 0x000060BC File Offset: 0x000042BC
	public void DogSummoner()
	{
		if (this.wendigoCountdown == 0)
		{
			ManagerScript.instance.PlayShortSound(this.dogClip[1], 0);
			this.rotation.GhostIntro();
		}
		else
		{
			ManagerScript.instance.PlayShortSound(this.dogClip[3], 0);
			for (int i = 0; i < this.minions.Length; i++)
			{
				this.minions[i].Spawn();
			}
		}
		this.wendigoCountdown--;
	}

	// Token: 0x06000110 RID: 272 RVA: 0x00002B19 File Offset: 0x00000D19
	public IEnumerator SummonMarch()
	{
		ManagerScript.instance.PlayShortSound(this.minionClip[0], 0);
		yield return base.StartCoroutine(this.RotationCD("march"));
		base.StartCoroutine(this.SkeliChargeSoundDelay());
		if (this.playerTransform.position.x > 0f)
		{
			this.skeliWalls[1].WallSpread();
			this.holoTransform[0].localScale = new Vector2(1f, 1f);
		}
		else
		{
			this.skeliWalls[0].WallSpread();
			this.holoTransform[0].localScale = new Vector2(-1f, 1f);
		}
		this.holoAnimator[0].enabled = true;
		this.holoAnimator[0].SetTrigger("cast");
		yield return new WaitForSeconds(this.cooldown);
		yield break;
	}

	// Token: 0x06000111 RID: 273 RVA: 0x00006134 File Offset: 0x00004334
	public void SummonMarchDuo()
	{
		base.StartCoroutine(this.SkeliChargeSoundDelay());
		for (int i = 0; i < this.skeliWalls.Length; i++)
		{
			this.skeliWalls[i].WallSpread();
			this.holoAnimator[i].enabled = true;
			this.holoAnimator[i].SetTrigger("cast");
		}
	}

	// Token: 0x06000112 RID: 274 RVA: 0x00002B28 File Offset: 0x00000D28
	public IEnumerator SkeliChargeSoundDelay()
	{
		ManagerScript.instance.PlayShortSound(this.skeliSummonClip[UnityEngine.Random.Range(0, this.skeliSummonClip.Length)], 0);
		yield return new WaitForSeconds(1f);
		ManagerScript.instance.PlayShortSound(this.skeliChargeClip[UnityEngine.Random.Range(0, this.skeliChargeClip.Length)], 0);
		yield break;
	}

	// Token: 0x06000113 RID: 275 RVA: 0x00002B37 File Offset: 0x00000D37
	public IEnumerator SummonSkull()
	{
		ManagerScript.instance.PlayShortSound(this.minionClip[1], 0);
		yield return base.StartCoroutine(this.RotationCD("skull"));
		ManagerScript.instance.PlayShortSound(this.skullClip[UnityEngine.Random.Range(0, this.skullClip.Length)], 0);
		this.skulls[this.skullIndex].Spawn();
		this.skullIndex++;
		if (this.skullIndex > this.skeliWalls.Length)
		{
			this.skullIndex = 0;
		}
		yield return new WaitForSeconds(this.cooldown);
		yield break;
	}

	// Token: 0x06000114 RID: 276 RVA: 0x00006190 File Offset: 0x00004390
	public void SummonSkullDuo()
	{
		ManagerScript.instance.PlayShortSound(this.skullClip[UnityEngine.Random.Range(0, this.skullClip.Length)], 0);
		this.skulls[this.skullIndex].Spawn();
		this.skullIndex++;
		this.skulls[this.skullIndex].Spawn();
		this.skullIndex++;
		if (this.skullIndex > this.skeliWalls.Length)
		{
			this.skullIndex = 0;
		}
	}

	// Token: 0x06000115 RID: 277 RVA: 0x00002B46 File Offset: 0x00000D46
	private IEnumerator PlayerBombard()
	{
		this.animator.SetTrigger("bomb");
		this.currentBigAudio = ManagerScript.instance.PlayShortSound(this.bigcannonClip[UnityEngine.Random.Range(0, this.bigcannonClip.Length)], 0);
		yield return new WaitForSeconds(this.cooldown);
		int num;
		for (int i = 0; i < this.scrapBarrage.Length; i = num + 1)
		{
			this.scrapBarrage[i].Aim(this.playerTransform.position);
			yield return new WaitForSeconds(0.3f);
			ManagerScript.instance.PlayShortSound(this.targetMarkClip, 0);
			num = i;
		}
		yield return new WaitForSeconds(0.1f);
		yield break;
	}

	// Token: 0x06000116 RID: 278 RVA: 0x00006218 File Offset: 0x00004418
	public void AniSmashKill()
	{
		this.rotation.camScript.Shakedown(0.3f, 0.3f);
		if (this.firstRotation)
		{
			this.firstRotation = false;
			ManagerScript.instance.PlayShortSound(this.bigClips[3], 0);
			this.glassCracks.SetActive(true);
		}
		ManagerScript.instance.PlayShortSound(this.bigslamClip[UnityEngine.Random.Range(0, this.bigslamClip.Length)], 0);
		this.spreadPool.Ignite(new Vector2(base.transform.position.x, -2f));
	}

	// Token: 0x06000117 RID: 279 RVA: 0x00002B55 File Offset: 0x00000D55
	public void AniSmallShake()
	{
		this.rotation.camScript.Shakedown(0.2f, 0.2f);
	}

	// Token: 0x06000118 RID: 280 RVA: 0x000062BC File Offset: 0x000044BC
	public void ElectroWipe()
	{
		if (this.currentBigAudio != null)
		{
			UnityEngine.Object.Destroy(this.currentBigAudio.gameObject);
		}
		for (int i = 0; i < this.minions.Length; i++)
		{
			this.minions[i].Death();
		}
		for (int j = 0; j < this.skulls.Length; j++)
		{
			this.skulls[j].Dmg(false);
		}
		for (int k = 0; k < this.skeliWalls.Length; k++)
		{
			this.skeliWalls[k].Wipe();
		}
		for (int l = 0; l < this.scrapBarrage.Length; l++)
		{
			this.scrapBarrage[l].Destruction(false);
		}
		this.wendogo.Waved(this.playerTransform.position);
		this.spreadPool.WipeEvent();
		base.StopAllCoroutines();
		this.rotation.camScript.Shakedown(0.2f, 0.2f);
		this.animator.SetTrigger("stun");
	}

	// Token: 0x06000119 RID: 281 RVA: 0x000063C0 File Offset: 0x000045C0
	public void WipeEvent()
	{
		for (int i = 0; i < this.minions.Length; i++)
		{
			this.minions[i].gameObject.SetActive(false);
		}
		for (int j = 0; j < this.skulls.Length; j++)
		{
			this.skulls[j].Wipe();
		}
		for (int k = 0; k < this.skeliWalls.Length; k++)
		{
			this.skeliWalls[k].gameObject.SetActive(false);
		}
		for (int l = 0; l < this.holoTransform.Length; l++)
		{
			this.holoTransform[l].gameObject.SetActive(false);
		}
		for (int m = 0; m < this.scrapBarrage.Length; m++)
		{
			this.scrapBarrage[m].Destruction(true);
		}
		this.spreadPool.gameObject.SetActive(false);
		base.StopAllCoroutines();
		this.animator.SetTrigger("end");
	}

	// Token: 0x0600011A RID: 282 RVA: 0x00002B71 File Offset: 0x00000D71
	public void LastScream()
	{
		ManagerScript.instance.PlayLongSound(this.bigClips[4]);
	}

	// Token: 0x040000DD RID: 221
	public Animator animator;

	// Token: 0x040000DE RID: 222
	public RotationScript rotation;

	// Token: 0x040000DF RID: 223
	public MinionAiScript[] minions;

	// Token: 0x040000E0 RID: 224
	public BulletSpreadScript[] skeliWalls;

	// Token: 0x040000E1 RID: 225
	public SkullbombScript[] skulls;

	// Token: 0x040000E2 RID: 226
	public Transform playerTransform;

	// Token: 0x040000E3 RID: 227
	public ScrapBarrageScript[] scrapBarrage;

	// Token: 0x040000E4 RID: 228
	public Vector2[] scrapTarget;

	// Token: 0x040000E5 RID: 229
	public BulletSpreadPoolScript spreadPool;

	// Token: 0x040000E6 RID: 230
	public SkullbombScript[] wailSkulls;

	// Token: 0x040000E7 RID: 231
	public DogoAiScript wendogo;

	// Token: 0x040000E8 RID: 232
	public Transform[] holoTransform;

	// Token: 0x040000E9 RID: 233
	public Animator[] holoAnimator;

	// Token: 0x040000EA RID: 234
	public int wendigoCountdown;

	// Token: 0x040000EB RID: 235
	public GameObject arm;

	// Token: 0x040000EC RID: 236
	private List<GameObject> arms = new List<GameObject>();

	// Token: 0x040000ED RID: 237
	public GameObject glassCracks;

	// Token: 0x040000EE RID: 238
	private float cooldown = 1f;

	// Token: 0x040000EF RID: 239
	private int skullIndex;

	// Token: 0x040000F0 RID: 240
	private int summonIndex;

	// Token: 0x040000F1 RID: 241
	private bool firstRotation = true;

	// Token: 0x040000F2 RID: 242
	public AudioClip[] phaseClip;

	// Token: 0x040000F3 RID: 243
	public AudioClip[] dogClip;

	// Token: 0x040000F4 RID: 244
	public AudioClip[] minionClip;

	// Token: 0x040000F5 RID: 245
	public AudioClip[] skullClip;

	// Token: 0x040000F6 RID: 246
	public AudioClip[] skeliSummonClip;

	// Token: 0x040000F7 RID: 247
	public AudioClip[] skeliChargeClip;

	// Token: 0x040000F8 RID: 248
	public AudioClip[] bigClips;

	// Token: 0x040000F9 RID: 249
	public AudioClip[] bigslamClip;

	// Token: 0x040000FA RID: 250
	public AudioClip[] bigcannonClip;

	// Token: 0x040000FB RID: 251
	public AudioClip targetMarkClip;

	// Token: 0x040000FC RID: 252
	private AudioSource currentBigAudio;
}
