using System;
using System.Collections;
using UnityEngine;

// Token: 0x0200000C RID: 12
public class BulletSpreadScript : MonoBehaviour
{
	// Token: 0x06000036 RID: 54 RVA: 0x000040F0 File Offset: 0x000022F0
	private void Start()
	{
		this.allBullets = base.GetComponentsInChildren<BulletScript>();
		for (int i = 0; i < this.allBullets.Length; i++)
		{
			this.allBullets[i].PostAnim();
		}
	}

	// Token: 0x06000037 RID: 55 RVA: 0x0000412C File Offset: 0x0000232C
	public void Spread(Vector3 origin)
	{
		base.transform.position = origin;
		for (int i = 0; i < this.allBullets.Length; i++)
		{
			this.allBullets[i].Cast();
		}
	}

	// Token: 0x06000038 RID: 56 RVA: 0x0000228B File Offset: 0x0000048B
	public void WallSpread()
	{
		base.StartCoroutine(this.TimedSpread());
	}

	// Token: 0x06000039 RID: 57 RVA: 0x0000229A File Offset: 0x0000049A
	public IEnumerator TimedSpread()
	{
		int num;
		for (int i = 0; i < this.allBullets.Length; i = num + 1)
		{
			this.allBullets[i].Cast();
			yield return new WaitForSeconds(0.05f);
			num = i;
		}
		yield break;
	}

	// Token: 0x0600003A RID: 58 RVA: 0x00004168 File Offset: 0x00002368
	public void Wipe()
	{
		for (int i = 0; i < this.allBullets.Length; i++)
		{
			this.allBullets[i].Destruction();
		}
		base.StopAllCoroutines();
	}

	// Token: 0x04000021 RID: 33
	public BulletScript[] allBullets;

	// Token: 0x04000022 RID: 34
	public int bulletIndex;
}
