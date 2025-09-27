using System;
using System.Collections;
using UnityEngine;

// Token: 0x02000061 RID: 97
public class AparatusScript : MonoBehaviour, IHighlightable
{
	// Token: 0x0600020D RID: 525 RVA: 0x00003328 File Offset: 0x00001528
	private void Start()
	{
		if (!this.junkReady)
			AwariaInterface.MainInterface.Instance.takeItem(this.gameObject.name);
        this.animator = base.GetComponent<Animator>();
		this.boxCollider = base.GetComponent<BoxCollider2D>();
	}

	// Token: 0x0600020E RID: 526 RVA: 0x00008EAC File Offset: 0x000070AC
	public void Highlight(bool active)
	{
		if (active && this.wanting && this.playerScript.hand[0] != this.want && this.playerScript.hand[1] != this.want)
		{
			active = false;
		}
		if (this.interactible != active)
		{
			for (int i = 0; i < this.highlights.Length; i++)
			{
				this.highlights[i].enabled = active;
			}
			this.interactible = active;
		}
	}

	// Token: 0x0600020F RID: 527 RVA: 0x00008F24 File Offset: 0x00007124
	public void Interaction()
	{
		if (!this.interactible)
		{
			return;
		}
		if (!this.junkReady)
		{
			this.animator.SetTrigger("activate");
			ManagerScript.instance.PlayShortSound(this.startClipArray[this.junk - 1], 0);
			this.boxCollider.enabled = false;
			this.Highlight(false);
			this.hourglass.SetActive(true);
			this.hourglassAnimator.SetTrigger("appear");
			base.StartCoroutine(this.Counter());
			AwariaInterface.MainInterface.Instance.placeItem(this.gameObject.name);
			if (this.want != 10)
			{
				int num = 0;
				if (this.playerScript.hand[0] != this.want)
				{
					num++;
				}
				this.playerScript.JunkDeposit(num);
				this.hintBubble.SetActive(false);
				this.wanting = false;
				return;
			}
		}
		else
		{
			if (this.junk == 4)
			{
				this.animator.SetTrigger("activate");
				ManagerScript.instance.PlayShortSound(this.startClipArray[this.junk - 1], 0);
				this.boxCollider.enabled = false;
			}
			else
			{
				this.JunkStateUpdate(false);
				AwariaInterface.MainInterface.Instance.takeItem(this.gameObject.name);
				if (this.want != 10)
				{
					this.hintBubble.SetActive(true);
					this.wanting = true;
				}
			}
			this.playerScript.JunkPickup(this.junk);
			ManagerScript.instance.PlayShortSound(this.pickupClipArray[this.junk - 1], 0);
			this.Highlight(true);
		}
	}

	// Token: 0x06000210 RID: 528 RVA: 0x00003342 File Offset: 0x00001542
	private IEnumerator Counter()
	{
		yield return new WaitForSeconds(this.timerTic);
		this.timePassed += this.timerTic;
		float num = Mathf.Clamp01(this.timePassed / this.requiredTime);
		this.barfillTransform.localPosition = new Vector3(this.barfillTransform.localPosition.x, (1f - num) * this.emptyBarPosition, 0f);
		this.barfillTransform.localScale = new Vector3(this.barfillTransform.localScale.x, num * this.filledBarScale, 0f);
		if (num != 1f)
		{
			base.StartCoroutine(this.Counter());
		}
		else
		{
			this.Forged();
		}
		yield break;
	}

	// Token: 0x06000211 RID: 529 RVA: 0x00009090 File Offset: 0x00007290
	public void Forged()
	{
		this.timePassed = 0f;
		this.hourglass.SetActive(false);
		this.barfillTransform.localScale = new Vector3(this.barfillTransform.localScale.x, 0f, 0f);
		this.JunkStateUpdate(true);
		AwariaInterface.MainInterface.Instance.finishedWorking(this.gameObject.name);
		this.animator.SetTrigger("finish");
		ManagerScript.instance.PlayShortSound(this.endClipArray[this.junk - 1], 0);
		this.boxCollider.enabled = true;
	}

	// Token: 0x06000212 RID: 530 RVA: 0x0000911C File Offset: 0x0000731C
	public void JunkStateUpdate(bool state = true)
	{
		this.junkSprite.enabled = state;
		this.junkReady = state;
		this.aparatusHlight.SetActive(!state);
		this.junkHlight.SetActive(state);
	}

	// Token: 0x06000213 RID: 531 RVA: 0x00003351 File Offset: 0x00001551
	public void Ready()
	{
		this.boxCollider.enabled = true;
	}

	// Token: 0x040001C6 RID: 454
	private Animator animator;

	// Token: 0x040001C7 RID: 455
	private BoxCollider2D boxCollider;

	// Token: 0x040001C8 RID: 456
	public int want = 10;

	// Token: 0x040001C9 RID: 457
	public SpriteRenderer junkSprite;

	// Token: 0x040001CA RID: 458
	public SpriteRenderer aparatusSprite;

	// Token: 0x040001CB RID: 459
	public GameObject hourglass;

	// Token: 0x040001CC RID: 460
	public Animator hourglassAnimator;

	// Token: 0x040001CD RID: 461
	public Transform barfillTransform;

	// Token: 0x040001CE RID: 462
	public SpriteRenderer[] highlights;

	// Token: 0x040001CF RID: 463
	public GameObject aparatusHlight;

	// Token: 0x040001D0 RID: 464
	public GameObject junkHlight;

	// Token: 0x040001D1 RID: 465
	public GameObject hintBubble;

	// Token: 0x040001D2 RID: 466
	public PlayerScript playerScript;

	// Token: 0x040001D3 RID: 467
	private float timerTic = 0.2f;

	// Token: 0x040001D4 RID: 468
	private float emptyBarPosition = 0.32f;

	// Token: 0x040001D5 RID: 469
	private float filledBarScale = 0.6f;

	// Token: 0x040001D6 RID: 470
	private float requiredTime = 5f;

	// Token: 0x040001D7 RID: 471
	public int junk;

	// Token: 0x040001D8 RID: 472
	public bool wanting = true;

	// Token: 0x040001D9 RID: 473
	public bool junkReady;

	// Token: 0x040001DA RID: 474
	private float timePassed;

	// Token: 0x040001DB RID: 475
	private bool interactible;

	// Token: 0x040001DC RID: 476
	public AudioClip[] startClipArray;

	// Token: 0x040001DD RID: 477
	public AudioClip[] endClipArray;

	// Token: 0x040001DE RID: 478
	public AudioClip[] pickupClipArray;
}
