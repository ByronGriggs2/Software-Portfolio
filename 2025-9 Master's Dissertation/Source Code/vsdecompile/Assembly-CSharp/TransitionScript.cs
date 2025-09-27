using System;
using UnityEngine;

// Token: 0x02000087 RID: 135
public class TransitionScript : MonoBehaviour
{
	// Token: 0x060002FB RID: 763 RVA: 0x00003A65 File Offset: 0x00001C65
	private void Start()
	{
		this.animator = base.GetComponent<Animator>();
		if (ManagerScript.instance.firstTransition)
		{
			this.animator.SetTrigger("noDoor");
			ManagerScript.instance.firstTransition = false;
			this.Starter();
		}
	}

	// Token: 0x060002FC RID: 764 RVA: 0x00003AA0 File Offset: 0x00001CA0
	public void Transition()
	{
		this.animator.SetTrigger("gameover");
		ManagerScript.instance.PlayLongSound(this.doorCloseSound);
	}

	// Token: 0x060002FD RID: 765 RVA: 0x00003AC2 File Offset: 0x00001CC2
	public void Loader()
	{
		ManagerScript.instance.Reload();
	}

	// Token: 0x060002FE RID: 766 RVA: 0x00003ACE File Offset: 0x00001CCE
	public void Starter()
	{
		ManagerScript.instance.backgroundSoundDisabled = false;
		Time.timeScale = 1f;
		this.playerScript.inputsBlocked = false;
	}

	// Token: 0x060002FF RID: 767 RVA: 0x00003AF1 File Offset: 0x00001CF1
	public void PlayOpen()
	{
		ManagerScript.instance.PlayLongSound(this.doorOpenSound);
	}

	// Token: 0x0400030C RID: 780
	public PlayerScript playerScript;

	// Token: 0x0400030D RID: 781
	public Animator animator;

	// Token: 0x0400030E RID: 782
	public AudioClip doorCloseSound;

	// Token: 0x0400030F RID: 783
	public AudioClip doorOpenSound;
}
