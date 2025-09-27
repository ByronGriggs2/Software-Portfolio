using System;
using TMPro;
using UnityEngine;
using UnityEngine.Events;

// Token: 0x02000088 RID: 136
public class UpscaleAnimatorScript : MonoBehaviour
{
	// Token: 0x06000301 RID: 769 RVA: 0x00003B03 File Offset: 0x00001D03
	private void Start()
	{
		this.animator = base.GetComponent<Animator>();
	}

	// Token: 0x06000302 RID: 770 RVA: 0x0000D174 File Offset: 0x0000B374
	public void Activator(string command, Transform target, int xFlip = 1)
	{
		if (command == "introStart")
		{
			this.ghostTier.text = ManagerScript.instance.menuTxt[this.tierNumber];
			ManagerScript.instance.PlayLongSound(this.ghostIntroAudio);
			ManagerScript.instance.MusicMuffle(20);
		}
		this.inIntro = false;
		this.flipSprite.localScale = new Vector3((float)xFlip, this.flipSprite.localScale.y);
		this.handle.position = target.position;
		this.animator.SetTrigger(command);
	}

	// Token: 0x06000303 RID: 771 RVA: 0x00003B11 File Offset: 0x00001D11
	public void Transition()
	{
		this.tranScript.Transition();
	}

	// Token: 0x06000304 RID: 772 RVA: 0x0000D20C File Offset: 0x0000B40C
	public void Victory()
	{
		Time.timeScale = 1f;
		this.genAnim.SetTrigger("fixed");
		this.screenAnim.SetTrigger("genFix");
		this.wipeEvent.Invoke();
		this.scoreObject.SetActive(false);
	}

	// Token: 0x06000305 RID: 773 RVA: 0x0000D25C File Offset: 0x0000B45C
	public void UnlockTime()
	{
		Time.timeScale = 1f;
		ManagerScript.instance.AudioPause(false);
		if (this.droneHit)
		{
			this.droneHit = false;
		}
		else
		{
			ManagerScript.instance.MusicMuffle(10);
		}
		this.playerScript.inputsBlocked = false;
		this.playerScript.rotationScript.AntiDronebug();
	}

	// Token: 0x06000306 RID: 774 RVA: 0x00003B1E File Offset: 0x00001D1E
	public void IntroLoop()
	{
		this.inIntro = true;
	}

	// Token: 0x06000307 RID: 775 RVA: 0x00003B27 File Offset: 0x00001D27
	public void GameoverAudio()
	{
		ManagerScript.instance.PlayLongSound(this.gameoverClip);
	}

	// Token: 0x04000310 RID: 784
	public Animator animator;

	// Token: 0x04000311 RID: 785
	public TransitionScript tranScript;

	// Token: 0x04000312 RID: 786
	public Transform flipSprite;

	// Token: 0x04000313 RID: 787
	public Transform handle;

	// Token: 0x04000314 RID: 788
	public Animator genAnim;

	// Token: 0x04000315 RID: 789
	public Animator screenAnim;

	// Token: 0x04000316 RID: 790
	public PlayerScript playerScript;

	// Token: 0x04000317 RID: 791
	public GameObject scoreObject;

	// Token: 0x04000318 RID: 792
	public bool droneHit;

	// Token: 0x04000319 RID: 793
	public AudioClip gameoverClip;

	// Token: 0x0400031A RID: 794
	public AudioClip ghostIntroAudio;

	// Token: 0x0400031B RID: 795
	public UnityEvent wipeEvent;

	// Token: 0x0400031C RID: 796
	public bool inIntro;

	// Token: 0x0400031D RID: 797
	public TMP_Text ghostTier;

	// Token: 0x0400031E RID: 798
	public int tierNumber;
}
