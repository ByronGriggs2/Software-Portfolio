using System;
using UnityEngine;

// Token: 0x02000011 RID: 17
public class ElectroVfxScript : MonoBehaviour
{
	// Token: 0x0600004A RID: 74 RVA: 0x0000430C File Offset: 0x0000250C
	public void Discharge(Vector3 place, string trigger)
	{
		base.transform.position = new Vector3(place.x + UnityEngine.Random.Range(-0.1f, 0.1f), place.y + UnityEngine.Random.Range(-0.1f, 0.1f));
		base.gameObject.SetActive(true);
		this.animator.SetTrigger(trigger);
		base.Invoke("AnimationEnd", 0.4f);
	}

	// Token: 0x0600004B RID: 75 RVA: 0x0000217F File Offset: 0x0000037F
	public void AnimationEnd()
	{
		base.gameObject.SetActive(false);
	}

	// Token: 0x0400002B RID: 43
	public Animator animator;
}
