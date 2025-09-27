using System;
using System.Collections;
using UnityEngine;

// Token: 0x02000028 RID: 40
public class ThunderScript : MonoBehaviour
{
	// Token: 0x060000B7 RID: 183 RVA: 0x00002791 File Offset: 0x00000991
	public void Bolt()
	{
		base.transform.position = this.location[UnityEngine.Random.Range(0, 3)];
		this.animator.SetTrigger("activate");
		AwariaInterface.MainInterface.Instance.addHazard(this);
		base.StartCoroutine(byronThunderTimeout());
	}
	private IEnumerator byronThunderTimeout()
	{
		yield return new WaitForSeconds(1.6f);
		yield break;
	}
	private void OnDestroy()
	{
		base.StopAllCoroutines();
	}

	// Token: 0x04000087 RID: 135
	public Animator animator;

	// Token: 0x04000088 RID: 136
	public Vector2[] location;
}
