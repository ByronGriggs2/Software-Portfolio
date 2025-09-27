using System;
using System.Collections;
using System.Linq;
using TMPro;
using UnityEngine;

// Token: 0x02000063 RID: 99
public class EngineScript : MonoBehaviour, IHighlightable
{
	// Token: 0x0600021B RID: 539 RVA: 0x0000929C File Offset: 0x0000749C
	private void Awake()
	{
        for (int i = 0; i < this.sparks.Length; i++)
		{
			this.sparks[i].Stop();
		}
	}

	// Token: 0x0600021C RID: 540 RVA: 0x00003376 File Offset: 0x00001576
	private void Start()
	{
        this.soundSide = this.xFlip;
		if (this.soundSide < 0)
		{
			this.soundSide = 0;
		}
		this.animator = base.GetComponent<Animator>();
	}

	// Token: 0x0600021D RID: 541 RVA: 0x000092CC File Offset: 0x000074CC
	public void Highlight(bool active)
	{
        if (active && this.wanting && this.want.Intersect(this.playerScript.hand).Count<int>() == 0)
		{
			active = false;
		}
		if (this.interactible != active)
		{
			this.highlight.enabled = active;
			this.interactible = active;
		}
	}

	// Token: 0x0600021E RID: 542 RVA: 0x00009320 File Offset: 0x00007520
	public void DefectStart(int[] randomWant, float time)
	{
		this.timePassed = 0f;
		this.requiredTime = time;
		base.StartCoroutine(this.Counter());
		this.want[0] = randomWant[0];
		this.want[1] = randomWant[1];
		this.junk[0].sprite = this.playerScript.junkSpriteArray[0];
		this.secondBubble.SetActive(false);
		this.animator.SetTrigger("break");
		ManagerScript.instance.PlayShortSound(this.dmgClip[this.soundSide], 0);
		for (int i = 0; i < this.sparks.Length; i++)
		{
			this.sparks[i].Play();
		}
		AwariaInterface.MainInterface.Instance.damageGenerator(this.gameObject.name);
		base.StartCoroutine(this.Broken(true));
	}

	// Token: 0x0600021F RID: 543 RVA: 0x000033A0 File Offset: 0x000015A0
	private IEnumerator Counter()
	{
		yield return new WaitForSeconds(this.timerTic);
		this.sparkTimer++;
		if (this.sparkTimer == this.nextSparkTime)
		{
			this.sparkTimer = 0;
			this.sparkAnimator.SetTrigger(this.sparkTrigger[this.sparkIndex]);
			this.sparkIndex++;
			if (this.sparkIndex == this.sparkTrigger.Length)
			{
				this.sparkIndex = 0;
			}
			this.nextSparkTime += UnityEngine.Random.Range(2, 6);
			if (this.nextSparkTime > 14)
			{
				this.nextSparkTime = 3;
			}
		}
		if (this.requiredTime != 99f)
		{
			this.timePassed += this.timerTic;
		}
		float num = Mathf.Clamp01(this.timePassed / this.requiredTime);
		this.barfillTransform.localPosition = new Vector3(this.barfillTransform.localPosition.x, num * this.emptyBarPosition, 0f);
		this.barfillTransform.localScale = new Vector3(this.barfillTransform.localScale.x, (1f - num) * this.filledBarScale, 0f);
		int num2 = (int)(this.requiredTime - this.timePassed);
		if (num2 == 3 && !this.isLate)
		{
			this.lateSource = ManagerScript.instance.PlayShortSound(this.lateClip[this.soundSide], 0);
			this.isLate = true;
		}
		this.timerText.text = num2.ToString();
		if (num != 1f)
		{
			base.StartCoroutine(this.Counter());
		}
		else
		{
			ManagerScript.instance.PlayLongSound(this.exploClip[this.soundSide]);
			this.rotationScript.Gameover(base.transform, false, this.xFlip);
		}
		yield break;
	}

	// Token: 0x06000220 RID: 544 RVA: 0x000033AF File Offset: 0x000015AF
	private IEnumerator Broken(bool broken)
	{
		if (broken)
		{
			yield return new WaitForSeconds(0.5f);
			this.boxCollider.enabled = true;
		}
		else
		{
			yield return new WaitForSeconds(2f);
			this.PostRepair();
			this.sparkAnimator.SetTrigger("end");
		}
		this.uiObject.SetActive(broken);
		yield break;
	}

	// Token: 0x06000221 RID: 545 RVA: 0x000093E0 File Offset: 0x000075E0
	public void Interaction()
	{
		if (!this.interactible)
		{
			return;
		}
		if (!this.wanting)
		{
			if (this.want[1] != 10)
			{
				this.secondBubble.SetActive(true);
			}
			for (int i = 0; i < 2; i++)
			{
				if (this.want[i] == 2 && this.redBats)
				{
					this.want[i] = UnityEngine.Random.Range(2, 4);
				}
				this.junk[i].sprite = this.playerScript.junkSpriteArray[this.want[i]];
				this.junkAnimator[i].SetTrigger("appear");
				this.animator.SetTrigger("repair");
				if (this.want[1] == 10)
				{
					i++;
				}
			}
			this.wanting = true;
			ManagerScript.instance.PlayShortSound(this.viewClip[this.soundSide], 0);
			AwariaInterface.MainInterface.Instance.checkDamagedGenerator(this.gameObject.name, this.want[0], this.want[1]);
		}
		else
		{
			for (int j = 0; j < 2; j++)
			{
				if (this.want[j] != 10)
				{
					for (int k = 0; k < 2; k++)
					{
						if (this.playerScript.hand[k] == this.want[j])
						{
							this.junk[j].sprite = this.checkerSprite;
							this.junkAnimator[j].SetTrigger("appear");
							ManagerScript.instance.PlayShortSound(this.insertClip[this.want[j] - 1 + this.soundSide * 5], 0);
							if (this.want[j] == 3)
							{
								this.want[j] = 2;
							}
							this.rotationScript.junkPool.Add(this.want[j]);
							this.want[j] = 10;
							this.playerScript.JunkDeposit(k);
						}
					}
				}
			}
			if (this.want[0] == 10 && this.want[1] == 10)
			{
                AwariaInterface.MainInterface.Instance.fixGenerator(this.gameObject.name);
                for (int l = 0; l < this.sparks.Length; l++)
				{
					this.sparks[l].Stop();
				}
				this.boxCollider.enabled = false;
				this.rotationScript.ScoreUpdate(base.transform, this.animator, this.xFlip);
				base.StopAllCoroutines();
				base.StartCoroutine(this.Broken(false));
				ManagerScript.instance.PlayShortSound(this.repairClip[this.soundSide], 0);
				if (this.isLate)
				{
					this.isLate = false;
					UnityEngine.Object.Destroy(this.lateSource.gameObject);
				}
			}
			else
			{
				AwariaInterface.MainInterface.Instance.partialFixGenerator(this.gameObject.name, this.want[0], this.want[1]);
				this.animator.SetTrigger("repair");
            }
		}
		this.Highlight(true);
	}

	// Token: 0x06000222 RID: 546 RVA: 0x000033C5 File Offset: 0x000015C5
	public void PostRepair()
	{
		this.junk[0].sprite = this.playerScript.junkSpriteArray[0];
		this.wanting = false;
		this.rotationScript.RepairFinished(this);
	}

	// Token: 0x040001E2 RID: 482
	private Animator animator;

	// Token: 0x040001E3 RID: 483
	public SpriteRenderer highlight;

	// Token: 0x040001E4 RID: 484
	public SpriteRenderer[] junk;

	// Token: 0x040001E5 RID: 485
	public Sprite checkerSprite;

	// Token: 0x040001E6 RID: 486
	public Animator[] junkAnimator;

	// Token: 0x040001E7 RID: 487
	public Transform barfillTransform;

	// Token: 0x040001E8 RID: 488
	public TMP_Text timerText;

	// Token: 0x040001E9 RID: 489
	public GameObject secondBubble;

	// Token: 0x040001EA RID: 490
	public BoxCollider2D boxCollider;

	// Token: 0x040001EB RID: 491
	public GameObject uiObject;

	// Token: 0x040001EC RID: 492
	public GameObject[] uiObjectCover;

	// Token: 0x040001ED RID: 493
	public ParticleSystem[] sparks;

	// Token: 0x040001EE RID: 494
	public RotationScript rotationScript;

	// Token: 0x040001EF RID: 495
	public Animator sparkAnimator;

	// Token: 0x040001F0 RID: 496
	public PlayerScript playerScript;

	// Token: 0x040001F1 RID: 497
	private float timerTic = 0.2f;

	// Token: 0x040001F2 RID: 498
	private float emptyBarPosition = -0.9f;

	// Token: 0x040001F3 RID: 499
	private float filledBarScale = 1.8f;

	// Token: 0x040001F4 RID: 500
	public int nextSparkTime = 5;

	// Token: 0x040001F5 RID: 501
	public int sparkIndex;

	// Token: 0x040001F6 RID: 502
	public int xFlip = 1;

	// Token: 0x040001F7 RID: 503
	public bool redBats;

	// Token: 0x040001F8 RID: 504
	private int[] want = new int[]
	{
		10,
		10
	};

	// Token: 0x040001F9 RID: 505
	private float requiredTime;

	// Token: 0x040001FA RID: 506
	private float timePassed;

	// Token: 0x040001FB RID: 507
	private bool interactible;

	// Token: 0x040001FC RID: 508
	private bool wanting;

	// Token: 0x040001FD RID: 509
	private string[] sparkTrigger = new string[]
	{
		"sparkA",
		"sparkB",
		"sparkC"
	};

	// Token: 0x040001FE RID: 510
	private int sparkTimer;

	// Token: 0x040001FF RID: 511
	public AudioClip[] dmgClip;

	// Token: 0x04000200 RID: 512
	public AudioClip[] exploClip;

	// Token: 0x04000201 RID: 513
	public AudioClip[] insertClip;

	// Token: 0x04000202 RID: 514
	public AudioClip[] viewClip;

	// Token: 0x04000203 RID: 515
	public AudioClip[] repairClip;

	// Token: 0x04000204 RID: 516
	public AudioClip[] lateClip;

	// Token: 0x04000205 RID: 517
	public AudioSource lateSource;

	// Token: 0x04000206 RID: 518
	private bool isLate;

	// Token: 0x04000207 RID: 519
	private int soundSide;
}
