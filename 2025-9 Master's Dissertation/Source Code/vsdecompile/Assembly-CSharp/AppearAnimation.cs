using System;
using UnityEngine;

// Token: 0x02000003 RID: 3
public class AppearAnimation : MonoBehaviour
{
	// Token: 0x0600000B RID: 11 RVA: 0x000020D4 File Offset: 0x000002D4
	private void OnEnable()
	{
		this.animator.SetTrigger("appear");
	}

	// Token: 0x04000005 RID: 5
	public Animator animator;
}
