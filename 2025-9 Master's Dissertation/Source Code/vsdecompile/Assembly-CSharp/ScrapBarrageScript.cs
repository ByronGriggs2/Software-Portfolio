using System;
using System.Collections;
using UnityEngine;

// Token: 0x0200001F RID: 31
public class ScrapBarrageScript : MonoBehaviour
{
    // Token: 0x06000083 RID: 131 RVA: 0x000025A3 File Offset: 0x000007A3
    public void Aim(Vector2 target)
	{
		base.transform.position = target;
		base.gameObject.SetActive(true);
		this.animator.SetTrigger("spawn");
		base.StartCoroutine(this.Drop());
		AwariaInterface.MainInterface.Instance.addHazard(this);
	}

	// Token: 0x06000084 RID: 132 RVA: 0x000025DF File Offset: 0x000007DF
	private IEnumerator Drop()
	{
		yield return new WaitForSeconds(this.dropTime);
		if (Vector2.Distance(this.player.transform.position, base.transform.position) < 0.5f)
		{
			this.player.Gameover();
		}
		this.nova.Spread(base.transform.position);
		ManagerScript.instance.PlayShortSound(this.impactClip[UnityEngine.Random.Range(0, this.impactClip.Length)], 0);
		yield break;
	}

	// Token: 0x06000085 RID: 133 RVA: 0x000048DC File Offset: 0x00002ADC
	public void Destruction(bool perma = true)
	{
		if (base.gameObject.activeSelf)
		{
			if (perma)
			{
				this.nova.gameObject.SetActive(false);
			}
			else
			{
				this.nova.Wipe();
			}
			base.StopAllCoroutines();
			base.gameObject.SetActive(false);
		}
	}

	// Token: 0x06000086 RID: 134 RVA: 0x000025EE File Offset: 0x000007EE
	public void Wipe()
	{
		if (base.gameObject.activeSelf)
		{
			UnityEngine.Object.Destroy(this.nova.gameObject);
			base.StopAllCoroutines();
			base.gameObject.SetActive(false);
		}
	}

	// Token: 0x0400005A RID: 90
	public BulletSpreadScript nova;

	// Token: 0x0400005B RID: 91
	public Animator animator;

	// Token: 0x0400005C RID: 92
	public PlayerScript player;

	// Token: 0x0400005D RID: 93
	public float dropTime = 0.5f;

	// Token: 0x0400005E RID: 94
	public AudioClip[] impactClip;
}
