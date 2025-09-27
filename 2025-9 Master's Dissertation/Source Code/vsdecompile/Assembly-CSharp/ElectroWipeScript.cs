using System;
using UnityEngine;

// Token: 0x02000033 RID: 51
public class ElectroWipeScript : MonoBehaviour
{
	// Token: 0x060000E9 RID: 233 RVA: 0x0000296C File Offset: 0x00000B6C
	public void Activate(Vector2 origin)
	{
		base.transform.position = origin;
		base.gameObject.SetActive(true);
		this.animator.SetTrigger("wipe");
		this.boss.ElectroWipe();
	}

	// Token: 0x040000B9 RID: 185
	public Animator animator;

	// Token: 0x040000BA RID: 186
	public BigStrigaScript boss;
}
